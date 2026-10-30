#include "transfer/backup_engine.hpp"
#include "transfer/gui_model.hpp"
#include "transfer/platform.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace transfer;
static void usage() {
    std::cout <<
    "PS4 to PS5 Transfer Tool v0.1.0 (host prototype)\n\n"
    "Commands:\n"
    "  transfer_tool --help\n"
    "  transfer_tool backup <source_dir> <backup_dir> <category>\n"
    "  transfer_tool verify <backup_dir>\n"
    "  transfer_tool restore <backup_dir> <destination_dir> [--overwrite]\n\n"
    "Categories: savedata | trophies | av_contents | user_selected\n"
    "This Windows executable is a test harness, not a console PKG.\n";
}
static Category parse_category(const std::string& s) {
    if(s=="savedata") return Category::SaveData;
    if(s=="trophies") return Category::Trophies;
    if(s=="av_contents") return Category::AVContents;
    return Category::UserSelected;
}
int main(int argc,char** argv) {
    if(argc<2 || std::string(argv[1])=="--help") { usage(); return 0; }
    BackupEngine engine;
    std::string cmd=argv[1];
    OperationResult r;
    if(cmd=="backup" && argc==5) {
        BackupMetadata meta;
        meta.source_console="host-test";
        meta.local_user_id="user-selected";
        r=engine.create_backup({SourceItem{argv[2],parse_category(argv[4])}},argv[3],meta);
    } else if(cmd=="verify" && argc==3) {
        r=engine.verify_backup(argv[2]);
    } else if(cmd=="restore" && (argc==4 || argc==5)) {
        bool overwrite=argc==5 && std::string(argv[4])=="--overwrite";
        r=engine.restore_backup(argv[2],argv[3],overwrite);
    } else { usage(); return 2; }
    std::cout<<(r.ok?"SUCCESS: ":"ERROR: ")<<r.message
             <<" Files processed: "<<r.files_processed<<"\n";
    return r.ok?0:1;
}
