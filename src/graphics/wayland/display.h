#pragma once

#include "graphics/wayland/internal/wayland.h"
#include "graphics/wayland/internal/xdg_shell.h"
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

  internal::Compositor *GetCompositor() noexcept { return compositor_; }

  internal::Shell *GetShell() noexcept { return shell_; }

  internal::Shm *GetShm() noexcept { return shm_; }

  internal::XdgWmBase *GetXdgWmBase() noexcept { return xdg_wm_base_; }

 private:
  static const internal::Registry::Listener RegistryListener;
  static const internal::XdgWmBase::Listener XdgWmBaseListener;

  internal::Display *handle_;
  internal::Compositor *compositor_;
  internal::Shell *shell_;
  internal::Shm *shm_;
  internal::XdgWmBase *xdg_wm_base_;

  WlDisplay(internal::Display *handle);

  static void RegistryGlobal(void *data, internal::Registry *registry,
                             uint32_t name, const char *interface,
                             uint32_t version) noexcept;
  static void RegistryGlobalRemove(void *data, internal::Registry *registry,
                                   uint32_t name) noexcept;

  static void XdgWmBasePing(void *data, internal::XdgWmBase *xdg_wm_base,
                            uint32_t serial) noexcept;
};

}  // namespace graphics::wayland
