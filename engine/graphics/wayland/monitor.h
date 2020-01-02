#pragma once

#include <string>
#include <vector>

#include "engine/graphics/wayland/internal/wayland.h"

namespace graphics::wayland {

class WlMonitor {
 public:
  struct Mode {
    internal::OutputMode Flags;
    uint32_t Width;
    uint32_t Height;
    uint32_t RefreshRate;
  };

  WlMonitor(internal::Output *handle);
  WlMonitor(const WlMonitor &) = delete;
  ~WlMonitor() { handle_->Release(); }

  const std::vector<Mode> &GetAvailableModes() noexcept { return modes_; }

  Mode GetCurrentMode() noexcept { return *modes_.end(); }

 private:
  static const internal::Output::Listener kOutputListener;

  internal::Output *handle_ = nullptr;

  int32_t x_position_ = 0;
  int32_t y_position_ = 0;
  uint32_t physical_width_ = 0;
  uint32_t physical_height_ = 0;
  internal::OutputSubpixel subpixel_ = internal::OutputSubpixel::kUnknown;
  std::string make_;
  std::string model_;
  internal::OutputTransform transform_ = internal::OutputTransform::kNormal;

  std::vector<Mode> modes_;

  uint32_t scaling_factor_ = 1;
};

}  // namespace graphics::wayland
