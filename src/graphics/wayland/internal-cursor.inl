namespace graphics::wayland::internal::cursor {

inline Cursor::Cursor(types::Cursor *handle) noexcept : handle_(handle) {}

inline int Cursor::Frame(uint32_t time) noexcept {
  return functions::CursorFrame(handle_, time);
}

inline int Cursor::FrameAndDuration(uint32_t time,
                                    uint32_t *duration) noexcept {
  return functions::CursorFrameAndDuration(handle_, time, duration);
}

inline CursorImage *Cursor::GetImage(size_t index) {
  return new CursorImage(handle_->Images[index]);
}

inline size_t Cursor::ImageCount() const noexcept {
  return handle_->ImageCount;
}

inline CursorImage::CursorImage(types::CursorImage *handle) noexcept
    : handle_(handle) {}

inline internal::types::Buffer *CursorImage::GetBuffer() {
  // TODO: Handle errors
  return functions::CursorImageGetBuffer(handle_);
}

inline CursorTheme::CursorTheme(types::CursorTheme *handle) noexcept
    : handle_(handle) {}

inline CursorTheme::~CursorTheme() { functions::CursorThemeDestroy(handle_); }

inline Cursor *CursorTheme::GetCursor(const char *name) {
  // TODO: Handle errors
  return new Cursor(functions::CursorThemeGetCursor(handle_, name));
}

inline CursorTheme *CursorTheme::Load(const char *name, int size,
                                      internal::types::Shm *shm) {
  // TODO: Handle errors
  return new CursorTheme(functions::CursorThemeLoad(name, size, shm));
}

}  // namespace graphics::wayland::internal::cursor
