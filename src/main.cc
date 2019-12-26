#define EGL_EGL_PROTOTYPES 0
extern "C" {
#include <X11/Xlib.h>
}

#include <fmt/format.h>

#include <iostream>
#include <vector>

#include "graphics/wayland/display.h"

int main() {
  auto display = graphics::wayland::WlDisplay::ConnectToDefault();
  std::shared_ptr<graphics::Window> window(
      display.CreateWindow("Voxel", 1080, 720));
  // window->SetVisible(true);

  display.HandleEvents();
}
