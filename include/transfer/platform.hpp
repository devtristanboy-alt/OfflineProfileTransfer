#pragma once
#include "transfer/types.hpp"
#include <string>
#include <vector>

namespace transfer {
struct UserDescriptor {
    std::string display_name;
    std::string local_id;
};

class PlatformAdapter {
public:
    virtual ~PlatformAdapter() = default;
    virtual ConsoleType console_type() const = 0;
    virtual bool enumerate_users(std::vector<UserDescriptor>& users,
                                 std::string& error) = 0;
    virtual bool list_removable_storage(std::vector<std::string>& devices,
                                        std::string& error) = 0;
};

// Host implementation exists only to exercise the shared engine.
// Real PS4/PS5 adapters must be implemented against the documented,
// legally obtained SDK/toolchain APIs for the target environment.
class HostPlatformAdapter final : public PlatformAdapter {
public:
    ConsoleType console_type() const override { return ConsoleType::Unknown; }
    bool enumerate_users(std::vector<UserDescriptor>& users,
                         std::string& error) override;
    bool list_removable_storage(std::vector<std::string>& devices,
                                std::string& error) override;
};
}
