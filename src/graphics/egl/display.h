#pragma once

#include "graphics/egl/internal.h"
#include "graphics/egl/native_display.h"

namespace graphics::egl {

class EglDisplay final {
 public:
  ~EglDisplay();

  static EglDisplay FromNative(EglNativeDisplay &native_display);

 private:
  EglDisplay(internal::Display *handle);

  internal::Display *handle_;
};

}  // namespace graphics::egl
