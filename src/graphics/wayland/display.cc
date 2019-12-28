#include "display.h"

#include <cmath>
#include <cstring>
#include <iostream>
#include <stdexcept>

#include "graphics/wayland/window.h"

namespace graphics::wayland {

const internal::Registry::Listener WlDisplay::RegistryListener{
    .Global = RegistryGlobal,
    .GlobalRemove = RegistryGlobalRemove,
};

const internal::XdgWmBase::Listener WlDisplay::XdgWmBaseListener{
    .Ping = XdgWmBasePing,
};

WlDisplay::WlDisplay(internal::Display *handle) : handle_(handle) {
  if (!handle) {
    throw std::runtime_error("Invalid wayland display handle");
  }
  internal::Registry *registry = handle->GetRegistry();
  registry->AddListener(RegistryListener, this);
  handle_->Dispatch();
  handle_->Roundtrip();
  registry->Destroy();

  if (!compositor_)
    throw std::runtime_error("Unable to get a wayland compositor");
  if (!shell_) throw std::runtime_error("Unable to get a wayland shell");
  if (!shm_) throw std::runtime_error("Unable to get wayland shm");

  if (xdg_wm_base_) xdg_wm_base_->AddListener(XdgWmBaseListener, this);
}

WlDisplay::~WlDisplay() {
  if (compositor_) compositor_->Destroy();
  if (shell_) shell_->Destroy();
  if (shm_) shm_->Destroy();
  if (xdg_wm_base_) xdg_wm_base_->Destroy();
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
  return new WlWindow(*this, width, height);
}

void WlDisplay::HandleEvents() {
  while (handle_->Dispatch() != -1)
    ;
}

void WlDisplay::RegistryGlobal(void *data, internal::Registry *registry,
                               uint32_t name, const char *interface,
                               uint32_t version) noexcept {
  WlDisplay *this_ptr = reinterpret_cast<WlDisplay *>(data);

  if (strcmp(interface, internal::CompositorInterface.Name) == 0)
    this_ptr->compositor_ = reinterpret_cast<internal::Compositor *>(
        registry->Bind(name, &internal::CompositorInterface, version));
  else if (strcmp(interface, internal::ShellInterface.Name) == 0)
    this_ptr->shell_ = reinterpret_cast<internal::Shell *>(
        registry->Bind(name, &internal::ShellInterface, version));
  else if (strcmp(interface, internal::ShmInterface.Name) == 0)
    this_ptr->shm_ = reinterpret_cast<internal::Shm *>(
        registry->Bind(name, &internal::ShmInterface, version));
  else if (strcmp(interface, internal::XdgWmBaseInterface.Name) == 0)
    this_ptr->xdg_wm_base_ = reinterpret_cast<internal::XdgWmBase *>(
        registry->Bind(name, &internal::XdgWmBaseInterface, version));
  std::cout << interface << " version: " << version << "\n";
}

void WlDisplay::RegistryGlobalRemove(void *data, internal::Registry *registry,
                                     uint32_t name) noexcept {}

void WlDisplay::XdgWmBasePing(void *data, internal::XdgWmBase *xdg_wm_base,
                              uint32_t serial) noexcept {
  xdg_wm_base->Pong(serial);
}

}  // namespace graphics::wayland
