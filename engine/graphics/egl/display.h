#pragma once

#include <vector>

#include "engine/graphics/egl/internal.h"

namespace graphics::egl {

class EglNativeDisplay {
 public:
  virtual ~EglNativeDisplay() = default;

  virtual internal::Enum GetEglPlatform() = 0;
  virtual void *GetEglNativeDisplayHandle() = 0;
  virtual void PopulateEglAttribList(
      std::vector<internal::Attrib> &attrib_list) = 0;
};

class EglDisplay final {
 public:
  EglDisplay(const EglDisplay &) = delete;
  ~EglDisplay();

  static EglDisplay FromNative(EglNativeDisplay &native_display);

  internal::Config *GetBestConfig() noexcept { return best_config_; }

  operator internal::Display *() noexcept { return handle_; }

 private:
  EglDisplay(internal::Display *handle);

  internal::Display *handle_;
  internal::Config *best_config_;
};

}  // namespace graphics::egl
