#include "egl.h"

namespace graphics::wayland::internal {

EglWindow::EglWindow(Surface *surface, int width, int height)
    : version_(kVersion), width_(width), height_(height), surface_(surface) {
  if (width <= 0 || height <= 0)
    // TODO: ERror
    throw "Bad";
}

EglWindow::~EglWindow() {
  if (destroy_window_callback_) destroy_window_callback_(driver_private_);
}

void EglWindow::GetAttachedSize(int &width, int &height) const noexcept {
  width = attached_width_;
  height = attached_height_;
}

void EglWindow::Resize(int width, int height, int dx, int dy) {
  if (width <= 0 || height <= 0)
    // TODO: Error
    throw "Bad";

  width_ = width;
  height_ = height;
  dx_ = dx;
  dy_ = dy;

  if (resize_callback_) resize_callback_(this, driver_private_);
}

}  // namespace graphics::wayland::internal