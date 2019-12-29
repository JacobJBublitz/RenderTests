#pragma once

#include "graphics/egl/internal.h"

namespace graphics::egl {

class EglNativeWindow {
 public:
  virtual ~EglNativeWindow() = default;

  virtual internal::Surface *CreateEglSurface(internal::Display *display,
                                              internal::Config *config) = 0;
};

}  // namespace graphics::egl
