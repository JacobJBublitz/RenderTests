#include "internal-egl.h"

#include <exception>
#include <iostream>

namespace graphics::wayland::internal::egl {
namespace functions {
namespace {

[[noreturn]] void NotLoadedFunction() {
  std::cerr << "Attempted to call a wayland function that is not loaded"
            << std::endl;
  std::terminate();
}

}  // namespace

types::EglWindowCreateProc EglWindowCreate =
    reinterpret_cast<types::EglWindowCreateProc>(NotLoadedFunction);
types::EglWindowDestroyProc EglWindowDestroy =
    reinterpret_cast<types::EglWindowDestroyProc>(NotLoadedFunction);
types::EglWindowResizeProc EglWindowResize =
    reinterpret_cast<types::EglWindowResizeProc>(NotLoadedFunction);
types::EglWindowGetAttachedSizeProc EglWindowGetAttachedSize =
    reinterpret_cast<types::EglWindowGetAttachedSizeProc>(NotLoadedFunction);

}  // namespace functions

void LoadFunctions(util::Library &library) {
  using namespace functions;
  using namespace functions::names;
  using namespace functions::types;

  EglWindowCreate =
      library.GetSymbolAddress<EglWindowCreateProc>(EglWindowCreateProcName);
  EglWindowDestroy =
      library.GetSymbolAddress<EglWindowDestroyProc>(EglWindowDestroyProcName);
  EglWindowResize =
      library.GetSymbolAddress<EglWindowResizeProc>(EglWindowResizeProcName);
  EglWindowGetAttachedSize =
      library.GetSymbolAddress<EglWindowGetAttachedSizeProc>(
          EglWindowGetAttachedSizeProcName);
}

}  // namespace graphics::wayland::internal::egl
