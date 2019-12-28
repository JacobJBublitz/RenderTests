#include "window.h"

#include <cstring>

namespace graphics::wayland {

WlWindow::WlWindow(WlDisplay &display, uint32_t width, uint32_t height)
    : display_(display) {
  surface_ = display_.GetCompositor()->CreateSurface();

  if (display_.GetXdgWmBase()) {
    auto base = display_.GetXdgWmBase();
    xdg_surface_ = base->GetXdgSurface(surface_);
    xdg_top_level_ = xdg_surface_->GetToplevel();
  } else {
    shell_surface_ = display_.GetShell()->GetShellSurface(surface_);
  }

  framebuffer_mem_ = std::make_unique<posix::SharedMemory>(width * height * 4);

  fb_pool_ = display_.GetShm()->CreatePool(framebuffer_mem_->GetFd(),
                                           framebuffer_mem_->GetSize());
  framebuffer_ = fb_pool_->CreateBuffer(0, width, height, width * 4,
                                        internal::ShmFormat::kXrgb8888);

  for (auto i = 0u; i < width; i++)
    for (auto j = 0u; j < height; j++)
      reinterpret_cast<uint32_t *>(
          framebuffer_mem_->GetPointer())[j * width + i] = rand();

  surface_->Attach(framebuffer_, 0, 0);
  surface_->Commit();
}

WlWindow::~WlWindow() {
  if (shell_surface_) shell_surface_->Destroy();

  if (xdg_top_level_) xdg_top_level_->Destroy();
  if (xdg_surface_) xdg_surface_->Destroy();

  framebuffer_->Destroy();
  fb_pool_->Destroy();
  surface_->Destroy();
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

void WlWindow::SetVisible(bool visible) {
  if (shell_surface_) shell_surface_->SetToplevel();
}

bool WlWindow::IsVisible() const noexcept {
  // TODO: Implement
  return false;
}

}  // namespace graphics::wayland