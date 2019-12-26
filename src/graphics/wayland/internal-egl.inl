namespace graphics::wayland::internal::egl {

inline EglWindow::EglWindow(types::EglWindow *handle) noexcept
    : handle_(handle) {}

inline EglWindow::~EglWindow() { functions::EglWindowDestroy(handle_); }

inline EglWindow *EglWindow::Create(internal::types::Surface *surface,
                                    int width, int height) {
  auto handle = functions::EglWindowCreate(surface, width, height);
  if (!handle) {
    // TODO: Exception
  }
  return new EglWindow(handle);
}

inline void EglWindow::Resize(int width, int height, int dx, int dy) noexcept {
  functions::EglWindowResize(handle_, width, height, dx, dy);
}

inline void EglWindow::GetAttachedSize(int *width, int *height) noexcept {
  functions::EglWindowGetAttachedSize(handle_, width, height);
}

}  // namespace graphics::wayland::internal::egl
