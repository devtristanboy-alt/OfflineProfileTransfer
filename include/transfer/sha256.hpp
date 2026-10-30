#pragma once
#include <filesystem>
#include <string>
namespace transfer {
std::string sha256_file(const std::filesystem::path& file);
}
