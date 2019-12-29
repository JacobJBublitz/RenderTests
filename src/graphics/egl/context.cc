#include "context.h"

namespace graphics::egl {

EglContext::EglContext(EglDisplay &display, EglSurface &surface, uint32_t major,
                       uint32_t minor, bool core_context,
                       bool forward_compatible)
    : display_(display), surface_(surface) {
  std::vector<int32_t> attrib_list = {
      internal::kContextMajorVersion,
      static_cast<int32_t>(major),
      internal::kContextMinorVersion,
      static_cast<int32_t>(minor),
      internal::kContextOpenGlProfileMask,
      static_cast<int32_t>(
          core_context ? internal::kContextOpenGlCoreProfileBit
                       : internal::kContextOpenGlCompatibilityProfileBit),
      internal::kContextOpenGlForwardCompatible,
      static_cast<int32_t>(forward_compatible ? internal::kTrue
                                              : internal::kFalse),
      internal::kNone};
  handle_ = internal::CreateContext(display_, display_.GetBestConfig(), nullptr,
                                    attrib_list.data());
}

EglContext::~EglContext() { internal::DestroyContext(display_, handle_); }

}  // namespace graphics::egl
