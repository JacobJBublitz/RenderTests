#pragma once

#include <vector>

#include "graphics/egl/internal.h"

namespace graphics::egl {

class EglNativeDisplay {
 public:
  virtual ~EglNativeDisplay() = default;

  virtual internal::Enum GetEglPlatform() = 0;
  virtual void *GetEglNativeDisplayHandle() = 0;
  virtual void PopulateEglAttribList(
      std::vector<internal::Attrib> &attrib_list) = 0;
};

}  // namespace graphics::egl
