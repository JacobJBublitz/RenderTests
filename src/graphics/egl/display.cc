#include "display.h"

#include <glog/logging.h>

namespace graphics::egl {

EglDisplay::EglDisplay(internal::Display *handle) : handle_(handle) {
  CHECK_NOTNULL(handle_);

  int32_t major, minor;
  if (!internal::Initialize(handle_, &major, &minor))
    throw std::runtime_error("Unable to initialize egl display");

  LOG_IF(FATAL, major < 1 || (major == 1 && minor < 5))
      << "The minimum required version of EGL is 1.5. Got " << major << "."
      << minor;
  LOG(INFO) << "Initialized EGL version " << major << "." << minor;
}

EglDisplay::~EglDisplay() { internal::Terminate(handle_); }

EglDisplay EglDisplay::FromNative(EglNativeDisplay &native_display) {
  std::vector<internal::Attrib> attribs;
  native_display.PopulateEglAttribList(attribs);
  attribs.push_back(internal::kNone);

  auto handle = internal::GetPlatformDisplay(
      native_display.GetEglPlatform(),
      native_display.GetEglNativeDisplayHandle(), attribs.data());
  if (!handle) {
    auto error = internal::GetError();
    if (error == internal::kBadParameter)
      throw std::runtime_error(
          "Unable to create EGL display. Native display specified an invalid "
          "platform.");
    else
      throw std::runtime_error(
          "Unable to create an EGL display. Unspecified error.");
  }

  return EglDisplay(handle);
}

}  // namespace graphics::egl
