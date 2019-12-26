#pragma once

#include "internal-core.h"
#include "util/library.h"

namespace graphics::wayland::internal::egl {

namespace types {

using EglWindow = void *;

}

namespace functions {

namespace names {

constexpr const char *EglWindowCreateProcName = "wl_egl_window_create";
constexpr const char *EglWindowDestroyProcName = "wl_egl_window_destroy";
constexpr const char *EglWindowResizeProcName = "wl_egl_window_resize";
constexpr const char *EglWindowGetAttachedSizeProcName =
    "wl_egl_window_get_attached_size";

}  // namespace names

namespace types {

using EglWindowCreateProc =
    egl::types::EglWindow *(*)(internal::types::Surface *surface, int width,
                               int height);
using EglWindowDestroyProc = void (*)(egl::types::EglWindow *window);
using EglWindowResizeProc = void (*)(egl::types::EglWindow *window, int width,
                                     int height, int dx, int dy);
using EglWindowGetAttachedSizeProc = void (*)(egl::types::EglWindow *window,
                                              int *width, int *height);

}  // namespace types

extern types::EglWindowCreateProc EglWindowCreate;
extern types::EglWindowDestroyProc EglWindowDestroy;
extern types::EglWindowResizeProc EglWindowResize;
extern types::EglWindowGetAttachedSizeProc EglWindowGetAttachedSize;

}  // namespace functions

class EglWindow final {
 public:
  ~EglWindow();

  static EglWindow *Create(internal::types::Surface *surface, int width,
                           int height);

  void Resize(int width, int height, int dx, int dy) noexcept;

  void GetAttachedSize(int *width, int *height) noexcept;

 private:
  EglWindow(types::EglWindow *handle) noexcept;

  types::EglWindow *handle_;
};

void LoadFunctions(util::Library &library);

}  // namespace graphics::wayland::internal::egl

#include "internal-egl.inl"
