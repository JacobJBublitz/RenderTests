#include "display.h"

#include <glog/logging.h>

#include <list>
#include <vector>

namespace graphics::egl {

namespace {

std::vector<internal::Config *> GetConfigs(internal::Display *display) {
  std::vector<int32_t> attrib_list = {internal::kRedSize,
                                      8,
                                      internal::kGreenSize,
                                      8,
                                      internal::kBlueSize,
                                      8,
                                      internal::kRenderableType,
                                      internal::kOpenGlBit,
                                      internal::kSurfaceType,
                                      internal::kWindowBit,
                                      internal::kNone};

  int32_t count = 0;
  internal::ChooseConfig(display, attrib_list.data(), nullptr, 0, &count);

  std::vector<internal::Config *> configs(count);
  internal::ChooseConfig(display, attrib_list.data(), configs.data(),
                         configs.size(), &count);

  return configs;
}

}  // namespace

EglDisplay::EglDisplay(internal::Display *handle) : handle_(handle) {
  CHECK_NOTNULL(handle_);

  int32_t major, minor;
  if (!internal::Initialize(handle_, &major, &minor))
    throw std::runtime_error("Unable to initialize EGL display");
  internal::BindApi(internal::kOpenGlApi);

  LOG_IF(FATAL, major < 1 || (major == 1 && minor < 5))
      << "The minimum required version of EGL is 1.5. Got " << major << "."
      << minor;
  LOG(INFO) << "Initialized EGL version " << major << "." << minor;

  {
    auto configs = GetConfigs(handle_);
    if (configs.size() == 0)
      throw std::runtime_error("No suitable EGL framebuffer configs found.");

    best_config_ = configs[0];
  }
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
