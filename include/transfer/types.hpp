#pragma once
#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

namespace transfer {
enum class ConsoleType { Unknown, PS4, PS5 };
enum class Category { SaveData, Trophies, AVContents, UserSelected };

inline std::string category_name(Category c) {
    switch (c) {
        case Category::SaveData: return "savedata";
        case Category::Trophies: return "trophies";
        case Category::AVContents: return "av_contents";
        default: return "user_selected";
    }
}

struct SourceItem {
    std::filesystem::path source;
    Category category{Category::UserSelected};
};

struct ManifestEntry {
    std::string category;
    std::string relative_path;
    std::uint64_t size{};
    std::string sha256;
};

struct BackupMetadata {
    std::string app_version{"0.1.0"};
    std::string source_console{"unknown"};
    std::string firmware{"not-detected"};
    std::string local_user_id{"not-detected"};
    std::string created_utc;
    std::vector<ManifestEntry> files;
};

struct OperationResult {
    bool ok{false};
    std::string message;
    std::size_t files_processed{0};
};
}
