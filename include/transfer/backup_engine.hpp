#pragma once
#include "transfer/types.hpp"
#include <filesystem>
#include <string>
#include <vector>

namespace transfer {
class BackupEngine {
public:
    OperationResult create_backup(
        const std::vector<SourceItem>& sources,
        const std::filesystem::path& backup_root,
        const BackupMetadata& metadata);

    OperationResult verify_backup(const std::filesystem::path& backup_root);

    OperationResult restore_backup(
        const std::filesystem::path& backup_root,
        const std::filesystem::path& destination_root,
        bool overwrite_existing = false);

private:
    static std::string utc_now();
    static std::string json_escape(const std::string& value);
    static bool safe_relative_path(const std::filesystem::path& path);
    static void write_manifest(const std::filesystem::path& path,
                               const BackupMetadata& metadata);
};
}
