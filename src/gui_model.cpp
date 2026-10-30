#include "transfer/gui_model.hpp"
#include <algorithm>
#include <utility>
namespace transfer {
void GuiModel::navigate(Screen screen) { state_.screen = screen; }
void GuiModel::set_status(std::string status) { state_.status = std::move(status); }
void GuiModel::set_progress(float progress) {
    state_.progress = std::clamp(progress, 0.0f, 1.0f);
}
void GuiModel::log(std::string message) { state_.messages.push_back(std::move(message)); }
}
