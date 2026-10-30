#include "transfer/backup_engine.hpp"
#include "transfer/sha256.hpp"
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace fs = std::filesystem;
namespace transfer {

std::string BackupEngine::utc_now() {
    auto now=std::chrono::system_clock::now();
    std::time_t t=std::chrono::system_clock::to_time_t(now);
    std::tm tm{};
#ifdef _WIN32
    gmtime_s(&tm,&t);
#else
    gmtime_r(&t,&tm);
#endif
    std::ostringstream s; s<<std::put_time(&tm,"%Y-%m-%dT%H:%M:%SZ"); return s.str();
}
std::string BackupEngine::json_escape(const std::string& v) {
    std::ostringstream o;
    for(unsigned char c:v) {
        switch(c) {
            case '"': o<<"\\\""; break; case '\\': o<<"\\\\"; break;
            case '\n': o<<"\\n"; break; case '\r': o<<"\\r"; break; case '\t': o<<"\\t"; break;
            default: if(c<0x20) { o<<"\\u"<<std::hex<<std::setw(4)<<std::setfill('0')<<int(c); }
                     else o<<c;
        }
    }
    return o.str();
}
bool BackupEngine::safe_relative_path(const fs::path& p) {
    if(p.empty() || p.is_absolute()) return false;
    for(const auto& part:p) if(part==".." || part==".") return false;
    return true;
}
void BackupEngine::write_manifest(const fs::path& path,const BackupMetadata& m) {
    std::ofstream o(path,std::ios::binary);
    if(!o) throw std::runtime_error("Cannot write manifest");
    o<<"{\n  \"format\": \"ps4-ps5-transfer-backup\",\n"
     <<"  \"format_version\": 1,\n"
     <<"  \"app_version\": \""<<json_escape(m.app_version)<<"\",\n"
     <<"  \"source_console\": \""<<json_escape(m.source_console)<<"\",\n"
     <<"  \"firmware\": \""<<json_escape(m.firmware)<<"\",\n"
     <<"  \"local_user_id\": \""<<json_escape(m.local_user_id)<<"\",\n"
     <<"  \"created_utc\": \""<<json_escape(m.created_utc)<<"\",\n"
     <<"  \"files\": [\n";
    for(std::size_t i=0;i<m.files.size();++i) {
        const auto& f=m.files[i];
        o<<"    {\"category\":\""<<json_escape(f.category)<<"\",\"path\":\""
         <<json_escape(f.relative_path)<<"\",\"size\":"<<f.size
         <<",\"sha256\":\""<<f.sha256<<"\"}";
        if(i+1<m.files.size()) o<<",";
        o<<"\n";
    }
    o<<"  ]\n}\n";
    if(!o) throw std::runtime_error("Manifest write failed");
}
OperationResult BackupEngine::create_backup(const std::vector<SourceItem>& sources,
                                              const fs::path& backup_root,
                                              const BackupMetadata& input) {
    try {
        if(sources.empty()) return {false,"No source directories selected.",0};
        if(fs::exists(backup_root) && !fs::is_empty(backup_root))
            return {false,"Backup destination is not empty. Choose a new folder.",0};
        fs::create_directories(backup_root);
        BackupMetadata meta=input; meta.created_utc=utc_now();
        std::size_t count=0;
        for(const auto& item:sources) {
            if(!fs::exists(item.source) || !fs::is_directory(item.source))
                return {false,"Source is not an existing directory: "+item.source.string(),count};
            const std::string cat=category_name(item.category);
            const fs::path target=backup_root/cat;
            fs::create_directories(target);
            for(fs::recursive_directory_iterator it(item.source),end;it!=end;++it) {
                if(it->is_symlink()) continue; // Never follow symlinks outside selected tree.
                if(!it->is_regular_file()) continue;
                auto rel=fs::relative(it->path(),item.source);
                if(!safe_relative_path(rel)) return {false,"Unsafe relative path rejected.",count};
                auto out=target/rel;
                fs::create_directories(out.parent_path());
                fs::copy_file(it->path(),out,fs::copy_options::overwrite_existing);
                meta.files.push_back({cat,(fs::path(cat)/rel).generic_string(),
                                      fs::file_size(out),sha256_file(out)});
                ++count;
            }
        }
        write_manifest(backup_root/"manifest.json",meta);
        std::ofstream log(backup_root/"backup.log",std::ios::app);
        log<<meta.created_utc<<" backup complete; files="<<count<<"\n";
        return {true,"Backup created and manifest written.",count};
    } catch(const std::exception& e) { return {false,e.what(),0}; }
}
OperationResult BackupEngine::verify_backup(const fs::path& root) {
    try {
        std::ifstream in(root/"manifest.json");
        if(!in) return {false,"manifest.json not found.",0};
        // Deliberately use a strict line-oriented manifest parser for this v1 format.
        // A full JSON parser will be added before accepting untrusted third-party manifests.
        std::string text((std::istreambuf_iterator<char>(in)),{});
        std::size_t pos=0,count=0;
        while((pos=text.find("\"path\":\"",pos))!=std::string::npos) {
            pos+=8; auto end=text.find('"',pos);
            if(end==std::string::npos) return {false,"Malformed manifest path.",count};
            fs::path rel=text.substr(pos,end-pos); pos=end+1;
            if(!safe_relative_path(rel)) return {false,"Unsafe path in manifest.",count};
            auto sizeKey=text.find("\"size\":",pos);
            auto hashKey=text.find("\"sha256\":\"",pos);
            if(sizeKey==std::string::npos || hashKey==std::string::npos)
                return {false,"Malformed manifest entry.",count};
            sizeKey+=7; auto sizeEnd=text.find_first_of(",}",sizeKey);
            hashKey+=10; auto hashEnd=text.find('"',hashKey);
            if(sizeEnd==std::string::npos || hashEnd==std::string::npos)
                return {false,"Malformed manifest entry values.",count};
            auto file=root/rel;
            if(!fs::is_regular_file(file)) return {false,"Missing file: "+rel.string(),count};
            auto expectedSize=std::stoull(text.substr(sizeKey,sizeEnd-sizeKey));
            auto expectedHash=text.substr(hashKey,hashEnd-hashKey);
            if(fs::file_size(file)!=expectedSize || sha256_file(file)!=expectedHash)
                return {false,"Integrity mismatch: "+rel.string(),count};
            ++count; pos=hashEnd+1;
        }
        return {true,"Backup integrity verified.",count};
    } catch(const std::exception& e) { return {false,e.what(),0}; }
}
OperationResult BackupEngine::restore_backup(const fs::path& root,const fs::path& dest,
                                              bool overwrite) {
    auto check=verify_backup(root);
    if(!check.ok) return check;
    try {
        std::ifstream in(root/"manifest.json");
        std::string text((std::istreambuf_iterator<char>(in)),{});
        fs::create_directories(dest);
        std::size_t pos=0,count=0;
        while((pos=text.find("\"path\":\"",pos))!=std::string::npos) {
            pos+=8; auto end=text.find('"',pos);
            if(end==std::string::npos) return {false,"Malformed manifest.",count};
            fs::path rel=text.substr(pos,end-pos); pos=end+1;
            if(!safe_relative_path(rel)) return {false,"Unsafe path rejected.",count};
            auto from=root/rel, to=dest/rel;
            fs::create_directories(to.parent_path());
            if(fs::exists(to) && !overwrite) return {false,"Destination exists; no files overwritten: "+to.string(),count};
            fs::copy_file(from,to,overwrite?fs::copy_options::overwrite_existing:fs::copy_options::none);
            ++count;
        }
        std::ofstream log(dest/"restore.log",std::ios::app);
        log<<utc_now()<<" restore complete; files="<<count<<"\n";
        return {true,"Restore completed.",count};
    } catch(const std::exception& e) { return {false,e.what(),0}; }
}
}
