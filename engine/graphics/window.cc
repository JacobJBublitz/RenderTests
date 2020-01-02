#include "window.h"

namespace graphics {

Display::~Display() = default;

Window::~Window() = default;

void Window::CallCloseCallback() {
  if (close_callback_) close_callback_();
}

void Window::SetCloseCallback(Window::CloseCallback callback) {
  close_callback_ = std::move(callback);
}

}  // namespace graphics
