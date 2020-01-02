#include "monitor.h"

#include <glog/logging.h>

namespace graphics::wayland {

using namespace internal;

const internal::Output::Listener WlMonitor::kOutputListener{
    .Geometry = [](void *data, Output *output, int32_t x, int32_t y,
                   int32_t physical_width, int32_t physical_height,
                   OutputSubpixel subpixel, const char *make, const char *model,
                   OutputTransform transform) {},
    .Mode =
        [](void *data, Output *output, OutputMode flags, int32_t width,
           int32_t height, int32_t refresh_rate) {
          LOG(INFO) << width << "x" << height << "@" << refresh_rate / 1000;
        },
    .Done = [](void *data, Output *output) {},
    .Scale = [](void *data, Output *output, int32_t factor) {},
};

WlMonitor::WlMonitor(Output *handle) : handle_(handle) {
  handle_->SetUserData(this);
  handle_->AddListener(kOutputListener, this);
}

}  // namespace graphics::wayland
