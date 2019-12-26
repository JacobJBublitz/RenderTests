#pragma once

#include "internal-core.h"
#include "util/library.h"

namespace graphics::wayland::internal::cursor {

namespace types {

struct CursorTheme;

struct CursorImage {
  uint32_t Width;
  uint32_t Height;
  uint32_t HotspotX;
  uint32_t HotspotY;
  uint32_t Delay;
};

struct Cursor {
  unsigned int ImageCount;
  CursorImage **Images;
  char *Name;
};

}  // namespace types

namespace functions {

namespace names {

constexpr const char *CursorThemeLoadProcName = "wl_cursor_theme_load";
constexpr const char *CursorThemeDestroyProcName = "wl_cursor_theme_destroy";
constexpr const char *CursorThemeGetCursorProcName =
    "wl_cursor_theme_get_cursor";
constexpr const char *CursorImageGetBufferProcName =
    "wl_cursor_image_get_buffer";
constexpr const char *CursorFrameProcName = "wl_cursor_frame";
constexpr const char *CursorFrameAndDurationProcName =
    "wl_cursor_frame_and_duration";

}  // namespace names

namespace types {

using CursorThemeLoadProc =
    cursor::types::CursorTheme *(*)(const char *name, int size,
                                    internal::types::Shm *shm);
using CursorThemeDestroyProc = void (*)(cursor::types::CursorTheme *theme);
using CursorThemeGetCursorProc =
    cursor::types::Cursor *(*)(cursor::types::CursorTheme *theme,
                               const char *name);
using CursorImageGetBufferProc =
    internal::types::Buffer *(*)(cursor::types::CursorImage *image);
using CursorFrameProc = int (*)(cursor::types::Cursor *cursor, uint32_t time);
using CursorFrameAndDurationProc = int (*)(cursor::types::Cursor *cursor,
                                           uint32_t time, uint32_t *duration);

}  // namespace types

extern types::CursorThemeLoadProc CursorThemeLoad;
extern types::CursorThemeDestroyProc CursorThemeDestroy;
extern types::CursorThemeGetCursorProc CursorThemeGetCursor;
extern types::CursorImageGetBufferProc CursorImageGetBuffer;
extern types::CursorFrameProc CursorFrame;
extern types::CursorFrameAndDurationProc CursorFrameAndDuration;

}  // namespace functions

class Cursor;
class CursorImage;
class CursorTheme;

class Cursor final {
  friend class CursorTheme;

  int Frame(uint32_t time) noexcept;

  int FrameAndDuration(uint32_t time, uint32_t *duration) noexcept;

  CursorImage *GetImage(size_t index);

  size_t ImageCount() const noexcept;

 private:
  Cursor(types::Cursor *handle) noexcept;

  types::Cursor *handle_;
};

class CursorImage final {
  friend class Cursor;

  internal::types::Buffer *GetBuffer();

 private:
  CursorImage(types::CursorImage *handle) noexcept;

  types::CursorImage *handle_;
};

class CursorTheme final {
  ~CursorTheme();

  static CursorTheme *Load(const char *name, int size,
                           internal::types::Shm *shm);

  Cursor *GetCursor(const char *name);

 private:
  CursorTheme(types::CursorTheme *handle) noexcept;

  types::CursorTheme *handle_;
};

void LoadFunctions(util::Library &library);

}  // namespace graphics::wayland::internal::cursor

#include "internal-cursor.inl"
