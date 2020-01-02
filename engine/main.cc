
#include <iostream>
#include <memory>

#include "engine/graphics/egl/context.h"
#include "engine/graphics/egl/display.h"
#include "engine/graphics/egl/surface.h"
#include "engine/graphics/wayland/display.h"
#include "engine/graphics/wayland/window.h"
#include "gflags/gflags.h"
// #include "glbinding/gl45core/gl.h"
// #include "glbinding/glbinding.h"
#include "glog/logging.h"

namespace {

bool Running = true;

}

int main(int argc, char *argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  FLAGS_alsologtostderr = true;
  FLAGS_colorlogtostderr = true;
  google::InitGoogleLogging(argv[0]);

  util::Library egl_library("libEGL.so");
  graphics::egl::internal::LoadFunctions(egl_library);

  auto display = graphics::wayland::WlDisplay::ConnectToDefault();
  auto egl_display = graphics::egl::EglDisplay::FromNative(display);

  auto window = graphics::wayland::WlWindow(display, "Voxel", 1080, 720);
  window.SetCloseCallback([]() { Running = false; });
  auto egl_surface = graphics::egl::EglSurface::FromNative(egl_display, window);

  auto egl_context =
      graphics::egl::EglContext(egl_display, egl_surface, 4, 5, true, true);

  egl_context.MakeCurrent();

  // glbinding::initialize([&egl_context](const char *name) {
  //   return egl_context.GetProcAddress(name);
  // });

  // gl45core::glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
  // gl45core::glClear(gl45core::GL_COLOR_BUFFER_BIT |
  //                   gl45core::GL_DEPTH_BUFFER_BIT);

  egl_context.SwapBuffers();

  while (Running) display.HandleEvents();

  egl_context.MakeNoneCurrent();
}