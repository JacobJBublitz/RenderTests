
#include <iostream>
#include <memory>

#include "engine/graphics/egl/context.h"
#include "engine/graphics/egl/display.h"
#include "engine/graphics/egl/surface.h"
#include "engine/graphics/vulkan/renderer.h"
#include "engine/graphics/wayland/display.h"
#include "engine/graphics/wayland/window.h"
#include "engine/util/library.h"
#include "gflags/gflags.h"
#include "glog/logging.h"
// #include "engine/graphics/windows/window.h"

namespace {

bool Running = true;
}

int main(int argc, char *argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  FLAGS_alsologtostderr = true;
  FLAGS_colorlogtostderr = true;
  google::InitGoogleLogging(argv[0]);

  try {
    auto display = graphics::wayland::WlDisplay::ConnectToDefault();

    // auto window =
    //     engine::graphics::windows::WinWindow(display, "Voxel", 1080, 720);
    auto window = graphics::wayland::WlWindow(display, "Voxel", 1080, 720);
    window.SetCloseCallback([]() { Running = false; });

    engine::graphics::vulkan::VkRenderer renderer{display, window};

    // while (Running) display.HandleEvents();
  } catch (std::exception &e) {
    LOG(FATAL) << e.what();
  } catch (...) {
    LOG(FATAL) << "Caught an unknown exception";
  }
}
