#include "internal-cursor.h"

#include <exception>
#include <iostream>

namespace graphics::wayland::internal::cursor {
namespace functions {
namespace {

[[noreturn]] void NotLoadedFunction() {
  std::cerr << "Attempted to call a wayland function that is not loaded"
            << std::endl;
  std::terminate();
}

}  // namespace

types::CursorThemeLoadProc CursorThemeLoad =
    reinterpret_cast<types::CursorThemeLoadProc>(NotLoadedFunction);
types::CursorThemeDestroyProc CursorThemeDestroy =
    reinterpret_cast<types::CursorThemeDestroyProc>(NotLoadedFunction);
types::CursorThemeGetCursorProc CursorThemeGetCursor =
    reinterpret_cast<types::CursorThemeGetCursorProc>(NotLoadedFunction);
types::CursorImageGetBufferProc CursorImageGetBuffer =
    reinterpret_cast<types::CursorImageGetBufferProc>(NotLoadedFunction);
types::CursorFrameProc CursorFrame =
    reinterpret_cast<types::CursorFrameProc>(NotLoadedFunction);
types::CursorFrameAndDurationProc CursorFrameAndDuration =
    reinterpret_cast<types::CursorFrameAndDurationProc>(NotLoadedFunction);

}  // namespace functions

void LoadFunctions(util::Library &library) {
  using namespace functions;
  using namespace functions::names;
  using namespace functions::types;

  CursorThemeLoad =
      library.GetSymbolAddress<CursorThemeLoadProc>(CursorThemeLoadProcName);
  CursorThemeDestroy = library.GetSymbolAddress<CursorThemeDestroyProc>(
      CursorThemeDestroyProcName);
  CursorThemeGetCursor = library.GetSymbolAddress<CursorThemeGetCursorProc>(
      CursorThemeGetCursorProcName);
  CursorImageGetBuffer = library.GetSymbolAddress<CursorImageGetBufferProc>(
      CursorImageGetBufferProcName);
  CursorFrame = library.GetSymbolAddress<CursorFrameProc>(CursorFrameProcName);
  CursorFrameAndDuration = library.GetSymbolAddress<CursorFrameAndDurationProc>(
      CursorFrameAndDurationProcName);
}

}  // namespace graphics::wayland::internal::cursor
