#include "monitor.h"

#include <glog/logging.h>

namespace graphics::wayland {

using namespace internal;

WlMonitor::WlMonitor(Output *handle) : handle_(handle) {
  handle_->SetUserData(this);
  handle_->AddListener(
      {
          .Geometry = [](void *data, Output *output, int32_t x, int32_t y,
                         int32_t physical_width, int32_t physical_height,
                         OutputSubpixel subpixel, const char *make,
                         const char *model, OutputTransform transform) {},
          .Mode =
              [](void *data, Output *output, OutputMode flags, int32_t width,
                 int32_t height, int32_t refresh_rate) {
                LOG(INFO) << width << "x" << height << "@" << refresh_rate;
              },
          .Done = [](void *data, Output *output) {},
          .Scale = [](void *data, Output *output, int32_t factor) {},
      },
      this);
}

}  // namespace graphics::wayland
