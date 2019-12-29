#include "window.h"

#include <fmt/format.h>
#include <glog/logging.h>

#include <cstring>
#include <iostream>

namespace graphics::wayland {

WlSurface::WlSurface(internal::Surface *handle) : handle_(handle) {
  LOG_IF(FATAL, handle == nullptr) << "Surface handle is null";

  handle_->SetUserData(this);

  handle_->AddListener(
      {
          .Enter =
              [](void *data, internal::Surface *surface,
                 internal::Output *output) {
                WlSurface *this_ptr = reinterpret_cast<WlSurface *>(data);

                this_ptr->outputs_.push_back(output);
              },
          .Leave =
              [](void *data, internal::Surface *surface,
                 internal::Output *output) {
                WlSurface *this_ptr = reinterpret_cast<WlSurface *>(data);

                this_ptr->outputs_.remove(output);
              },
      },
      this);
}

WlSurface::~WlSurface() { handle_->Destroy(); }

void WlSurface::HandlePointerMotion(uint32_t time, double x, double y) {
  LOG(INFO) << "X: " << x << ", Y: " << y;
}

const internal::XdgSurface::Listener WlWindow::XdgSurfaceListener{
    .Configure = XdgSurfaceConfigureEvent,
};
const internal::XdgToplevel::Listener WlWindow::XdgToplevelListener{
    .Configure = XdgToplevelConfigureEvent,
    .Close = XdgToplevelCloseEvent,
};

WlWindow::WlWindow(WlDisplay &display, const std::string &title, uint32_t width,
                   uint32_t height)
    : WlSurface(display.GetCompositor()),
      egl_surface_(GetSurfaceHandle(), static_cast<int>(width),
                   static_cast<int>(height)) {
  xdg_surface_ = display.GetXdgWmBase()->GetXdgSurface(GetSurfaceHandle());
  xdg_surface_->AddListener(XdgSurfaceListener, this);

  xdg_toplevel_ = xdg_surface_->GetToplevel();
  xdg_toplevel_->AddListener(XdgToplevelListener, this);
  xdg_toplevel_->SetTitle(title.c_str());

  // framebuffer_mem_ = std::make_unique<posix::SharedMemory>(width * height *
  // 4);

  // fb_pool_ = display.GetShm()->CreatePool(framebuffer_mem_->GetFd(),
  //                                          framebuffer_mem_->GetSize());
  // framebuffer_ = fb_pool_->CreateBuffer(0, width, height, width * 4,
  //                                       internal::ShmFormat::kXrgb8888);

  // for (auto i = 0u; i < height; i++) {
  //   uint32_t val = rand();
  //   for (auto j = 0u; j < width; j++)
  //     reinterpret_cast<uint32_t *>(
  //         framebuffer_mem_->GetPointer())[i * width + j] = val;
  // }

  // GetSurfaceHandle()->Attach(framebuffer_, 0, 0);
  // GetSurfaceHandle()->Commit();
}

WlWindow::~WlWindow() {
  xdg_toplevel_->Destroy();
  xdg_surface_->Destroy();

  // framebuffer_->Destroy();
  // fb_pool_->Destroy();
}

void WlWindow::SetPosition(int32_t x, int32_t y) {
  // TODO: Implement
}

void WlWindow::GetPosition(int32_t &x, int32_t &y) const noexcept {
  // TODO: Implement
}

void WlWindow::SetSize(uint32_t width, uint32_t height) {
  // TODO: Implement
}

void WlWindow::GetSize(uint32_t &width, uint32_t &height) const noexcept {
  // TODO: Implement
}

void WlWindow::XdgSurfaceConfigureEvent(void *data,
                                        internal::XdgSurface *surface,
                                        uint32_t serial) noexcept {
  surface->AckConfigure(serial);
}
void WlWindow::XdgToplevelConfigureEvent(void *data,
                                         internal::XdgToplevel *toplevel,
                                         int32_t width, int32_t height,
                                         internal::Array *states) noexcept {}
void WlWindow::XdgToplevelCloseEvent(void *data,
                                     internal::XdgToplevel *toplevel) noexcept {
  WlWindow *this_ptr = reinterpret_cast<WlWindow *>(data);

  this_ptr->CallCloseCallback();
}

}  // namespace graphics::wayland
