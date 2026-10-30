#pragma once
#include <string>
#include <vector>

namespace transfer {
enum class Screen { Home, Backup, Restore, Verify, Settings, About };
struct GuiState {
    Screen screen{Screen::Home};
    std::string status{"Ready"};
    float progress{0.0f};
    std::vector<std::string> messages;
};

// UI-independent model. A native PS4/PS5 renderer can bind buttons,
// lists, dialogs and progress bars to this state.
class GuiModel {
public:
    const GuiState& state() const { return state_; }
    void navigate(Screen screen);
    void set_status(std::string status);
    void set_progress(float progress);
    void log(std::string message);
private:
    GuiState state_;
};
}
