#include "display.h"

#include <cmath>
#include <cstring>
#include <iostream>
#include <stdexcept>

namespace graphics::wayland {

WlDisplay::WlDisplay(internal::Display *handle) : handle_(handle) {
  if (!handle) {
    throw std::runtime_error("Invalid wayland display handle");
  }

  internal::Registry::Listener listener{.Global = RegistryGlobal,
                                        .GlobalRemove = RegistryGlobalRemove};

  internal::ScopedProxy<internal::Registry> registry = handle->GetRegistry();
  registry->AddListener(listener, this);
  handle_->Roundtrip();
}

WlDisplay::~WlDisplay() {
  compositor_->Destroy();
  handle_->Disconnect();
}

WlDisplay WlDisplay::Connect(const std::string &name) {
  auto handle = internal::DisplayBase::Connect(name.c_str());

  return WlDisplay(handle);
}

WlDisplay WlDisplay::ConnectToDefault() {
  auto handle = internal::DisplayBase::Connect(nullptr);

  return WlDisplay(handle);
}

WlDisplay WlDisplay::ConnectToFileDescriptor(int fd) {
  auto handle = internal::DisplayBase::ConnectToFd(fd);

  return WlDisplay(handle);
}

Window *WlDisplay::CreateWindow(const std::string &title, uint32_t width,
                                uint32_t height) {
  return nullptr;
}

void WlDisplay::HandleEvents() {}

void WlDisplay::RegistryGlobal(void *data, internal::Registry *registry,
                               uint32_t name, const char *interface,
                               uint32_t version) noexcept {
  WlDisplay *this_ptr = reinterpret_cast<WlDisplay *>(data);

  if (strcmp(interface, internal::CompositorInterface.Name) == 0) {
    this_ptr->compositor_ =
        reinterpret_cast<internal::Compositor *>(registry->Bind(
            name, &internal::CompositorInterface, std::min(version, 4u)));
  } else {
    std::cout << interface << "\n";
  }
}

void WlDisplay::RegistryGlobalRemove(void *data, internal::Registry *registry,
                                     uint32_t name) noexcept {}

}  // namespace graphics::wayland
