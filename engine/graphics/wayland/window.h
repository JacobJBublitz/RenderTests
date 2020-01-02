#pragma once

#include <list>
#include <memory>

#include "engine/graphics/egl/surface.h"
#include "engine/graphics/wayland/display.h"
#include "engine/graphics/wayland/internal/egl.h"
#include "engine/graphics/window.h"

namespace graphics::wayland {

class WlSurface {
 public:
  WlSurface(internal::Compositor *compositor)
      : WlSurface(compositor->CreateSurface()) {}
  WlSurface(internal::Surface *handle);
  WlSurface(const WlSurface &) = delete;  // No copy
  virtual ~WlSurface();

  virtual void HandlePointerMotion(uint32_t time, double x, double y);

  internal::Surface *GetSurfaceHandle() noexcept { return handle_; }

  operator internal::Surface *() noexcept { return handle_; }

 private:
  static const internal::Surface::Listener kSurfaceListener;

  internal::Surface *handle_;

  std::list<internal::Output *> outputs_;
};

class WlWindow : public Window, public WlSurface, public egl::EglNativeWindow {
 public:
  WlWindow(WlDisplay &display, const std::string &title, uint32_t width,
           uint32_t height);
  ~WlWindow();

  void *GetEglNativeWindowHandle() override { return &egl_surface_; }
  void PopulateEglAttribList(
      std::vector<egl::internal::Attrib> &attrib_list) override {}

  void SetPosition(int32_t x, int32_t y) final;
  void GetPosition(int32_t &x, int32_t &y) const noexcept final;

  void SetSize(uint32_t width, uint32_t height) final;
  void GetSize(uint32_t &width, uint32_t &height) const noexcept final;

 private:
  static const internal::XdgSurface::Listener XdgSurfaceListener;
  static const internal::XdgToplevel::Listener XdgToplevelListener;

  internal::EglWindow egl_surface_;

  internal::XdgSurface *xdg_surface_ = nullptr;
  internal::XdgToplevel *xdg_toplevel_ = nullptr;

  static void XdgSurfaceConfigureEvent(void *data,
                                       internal::XdgSurface *surface,
                                       uint32_t serial) noexcept;
  static void XdgToplevelConfigureEvent(void *data,
                                        internal::XdgToplevel *toplevel,
                                        int32_t width, int32_t height,
                                        internal::Array *states) noexcept;
  static void XdgToplevelCloseEvent(void *data,
                                    internal::XdgToplevel *toplevel) noexcept;
};

}  // namespace graphics::wayland
