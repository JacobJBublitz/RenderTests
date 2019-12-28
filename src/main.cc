
#include <iostream>
#include <memory>

#include "graphics/wayland/display.h"

int main() {
  auto display = graphics::wayland::WlDisplay::ConnectToDefault();
  std::shared_ptr<graphics::Window> window(
      display.CreateWindow("Voxel", 1080, 720));
  window->SetVisible(true);

  display.HandleEvents();
}
