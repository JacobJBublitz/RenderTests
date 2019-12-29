#pragma once

#include "graphics/egl/display.h"
#include "graphics/egl/internal.h"

namespace graphics::egl {

class EglNativeWindow {
 public:
  virtual ~EglNativeWindow() = default;

  virtual void *GetEglNativeWindowHandle() = 0;
  virtual void PopulateEglAttribList(
      std::vector<internal::Attrib> &attrib_list) = 0;
};

class EglSurface {
 public:
  EglSurface(const EglSurface &) = delete;
  ~EglSurface() { internal::DestroySurface(display_, handle_); }

  static EglSurface FromNative(EglDisplay &display,
                               EglNativeWindow &native_window);

  operator internal::Surface *() noexcept { return handle_; }

 private:
  EglSurface(EglDisplay &display, internal::Surface *handle);

  EglDisplay &display_;
  internal::Surface *handle_;
};

}  // namespace graphics::egl
