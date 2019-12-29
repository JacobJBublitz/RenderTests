#pragma once

#include "graphics/egl/display.h"
#include "graphics/egl/surface.h"

namespace graphics::egl {

class EglContext {
 public:
  EglContext(EglDisplay &display, EglSurface &surface, uint32_t major,
             uint32_t minor, bool core_context = false,
             bool forward_compatible = false);
  EglContext(const EglContext &) = delete;
  ~EglContext();

  void MakeCurrent() {
    internal::MakeCurrent(display_, surface_, surface_, handle_);
  }

  void MakeNoneCurrent() {
    internal::MakeCurrent(display_, nullptr, nullptr, nullptr);
  }

  void SwapBuffers() { internal::SwapBuffers(display_, surface_); }

 private:
  EglDisplay &display_;
  EglSurface &surface_;
  internal::Context *handle_;
};

}  // namespace graphics::egl
