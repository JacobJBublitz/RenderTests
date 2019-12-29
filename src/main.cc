
#include <gflags/gflags.h>
#include <glog/logging.h>

#include <iostream>
#include <memory>

#include "graphics/egl/context.h"
#include "graphics/egl/display.h"
#include "graphics/egl/surface.h"
#include "graphics/wayland/display.h"
#include "graphics/wayland/window.h"

extern "C" {
#include <GL/gl.h>
}

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

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  egl_context.SwapBuffers();

  while (Running) display.HandleEvents();

  egl_context.MakeNoneCurrent();
}
