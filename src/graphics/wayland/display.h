#pragma once

#include "graphics/wayland/internal/wayland.h"
#include "graphics/window.h"

namespace graphics::wayland {

class WlDisplay final : public Display {
 public:
  WlDisplay(const WlDisplay &) = delete;  // No copy

  ~WlDisplay() final;

  static WlDisplay Connect(const std::string &name);
  static WlDisplay ConnectToDefault();
  static WlDisplay ConnectToFileDescriptor(int fd);

  Window *CreateWindow(const std::string &title, uint32_t width,
                       uint32_t height) final;

  void HandleEvents() final;

 private:
  internal::Display *handle_;
  internal::Compositor *compositor_;

  WlDisplay(internal::Display *handle);

  static void RegistryGlobal(void *data, internal::Registry *registry,
                             uint32_t name, const char *interface,
                             uint32_t version) noexcept;
  static void RegistryGlobalRemove(void *data, internal::Registry *registry,
                                   uint32_t name) noexcept;
};

}  // namespace graphics::wayland
