#include "transfer/platform.hpp"
namespace transfer {
bool HostPlatformAdapter::enumerate_users(std::vector<UserDescriptor>& users,
                                          std::string& error) {
    users.clear();
    error = "User discovery is unavailable in the Windows host prototype.";
    return false;
}
bool HostPlatformAdapter::list_removable_storage(std::vector<std::string>& devices,
                                                 std::string& error) {
    devices.clear();
    error = "Use an explicit destination path in the host prototype.";
    return false;
}
}
