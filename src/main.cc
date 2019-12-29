
#include <glog/logging.h>

#include <iostream>
#include <memory>

#include "graphics/egl/display.h"
#include "graphics/wayland/display.h"

namespace {

bool Running = true;

}

int main(int argc, char *argv[]) {
  FLAGS_alsologtostderr = true;
  FLAGS_colorlogtostderr = true;
  google::InitGoogleLogging(argv[0]);

  util::Library egl_library("libEGL.so");
  graphics::egl::internal::LoadFunctions(egl_library);

  auto display = graphics::wayland::WlDisplay::ConnectToDefault();
  auto egl_display = graphics::egl::EglDisplay::FromNative(display);
  
  std::shared_ptr<graphics::Window> window(
      display.CreateWindow("Voxel", 1080, 720));
  window->SetCloseCallback([]() { Running = false; });

  while (Running) display.HandleEvents();
}
