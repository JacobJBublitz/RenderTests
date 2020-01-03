#ifndef ENGINE_GRAPHICS_WINDOWS_WINDOW_H_
#define ENGINE_GRAPHICS_WINDOWS_WINDOW_H_

#define UNICODE
extern "C" {
#include <windows.h>
}

#include "engine/graphics/window.h"

namespace engine::graphics::windows {

class WinDisplay final : public Display {
 public:
  WinDisplay();
  ~WinDisplay() final;

  void HandleEvents() final;

  void PostEmptyEvent();

  ::HINSTANCE GetInstanceHandle() noexcept { return hinstance_; }

  ::ATOM GetWindowClassAtom() noexcept { return wnd_class_atom_; }

 private:
  ::HINSTANCE hinstance_;
  ::ATOM wnd_class_atom_;

  static ::LRESULT CALLBACK WindowProc(::HWND handle, ::UINT msg,
                                       ::WPARAM wparam,
                                       ::LPARAM lparam) noexcept;
};

class WinWindow final : public Window {
  friend class WinDisplay;

 public:
  WinWindow(WinDisplay &display, const std::string &title, uint32_t width,
            uint32_t height);
  ~WinWindow() final;

  void SetPosition(int32_t x, int32_t y) final;
  void GetPosition(int32_t &x, int32_t &y) const noexcept final;

  void SetSize(uint32_t width, uint32_t height) final;
  void GetSize(uint32_t &width, uint32_t &height) const noexcept final;

 private:
  ::HWND handle_;
  ::HDC device_context_;

  ::LRESULT WindowProc(::UINT msg, ::WPARAM wparam, ::LPARAM lparam) noexcept;
};

}  // namespace engine::graphics::windows

#endif
