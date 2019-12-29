#include "surface.h"

#include <stdexcept>

namespace graphics::egl {

EglSurface::EglSurface(EglDisplay &display, internal::Surface *handle)
    : display_(display), handle_(handle) {}

EglSurface EglSurface::FromNative(EglDisplay &display,
                                  EglNativeWindow &native_window) {
  std::vector<internal::Attrib> attrib_list;
  native_window.PopulateEglAttribList(attrib_list);
  attrib_list.push_back(internal::kNone);

  auto handle = internal::CreatePlatformWindowSurface(
      display, display.GetBestConfig(),
      native_window.GetEglNativeWindowHandle(), attrib_list.data());
  if (handle == internal::kNoSurface) {
    throw std::runtime_error("Unable to create EGL surface");
  }

  return EglSurface(display, handle);
}

}  // namespace graphics::egl
