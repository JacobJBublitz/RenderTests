#pragma once

#include <memory>
#include "graphics/wayland/display.h"
#include "graphics/window.h"
#include "posix/shared_memory.h"

namespace graphics::wayland {

class WlWindow : public Window {
 public:
  WlWindow(WlDisplay &display, uint32_t width, uint32_t height);
  ~WlWindow();

  void SetPosition(int32_t x, int32_t y) final;
  void GetPosition(int32_t &x, int32_t &y) const noexcept final;

  void SetSize(uint32_t width, uint32_t height) final;
  void GetSize(uint32_t &width, uint32_t &height) const noexcept final;

  void SetVisible(bool visible) final;
  bool IsVisible() const noexcept final;

 private:
  static const internal::XdgSurface::Listener XdgSurfaceListener;
  static const internal::XdgToplevel::Listener XdgTopLevelListener;

  WlDisplay &display_;
  internal::Surface *surface_;
  std::unique_ptr<posix::SharedMemory> framebuffer_mem_;
  internal::ShmPool *fb_pool_;
  internal::Buffer *framebuffer_;

  internal::XdgSurface *xdg_surface_ = nullptr;
  internal::XdgToplevel *xdg_top_level_ = nullptr;

  internal::ShellSurface *shell_surface_;

  static void XdgSurfaceConfigure(void *data, internal::XdgSurface *surface,
                                  uint32_t serial) noexcept;
  static void XdgToplevelConfigure(void *data, internal::XdgToplevel *toplevel,
                                   int32_t width, int32_t height,
                                   internal::Array *states) noexcept;
  static void XdgToplevelClose(void *data,
                               internal::XdgToplevel *toplevel) noexcept;
};

}  // namespace graphics::wayland