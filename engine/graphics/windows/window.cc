#include "window.h"

#include <system_error>
#define GLOG_NO_ABBREVIATED_SEVERITIES
#include "glog/logging.h"

namespace engine::graphics::windows {

namespace {

constexpr wchar_t *kWindowClassName = L"EngineWindowClass";

constexpr char *kWindowClassRegisterFailure = "Failed to register window class";
constexpr char *kWindowClassUnregisterFailure =
    "Failed to unregister window class";
}  // namespace

WinDisplay::WinDisplay() {
  hinstance_ = ::GetModuleHandleW(nullptr);

  ::WNDCLASSEXW wc = {};
  wc.cbSize = sizeof wc;
  wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc = WindowProc;
  wc.hInstance = hinstance_;
  wc.hIcon = ::LoadIconW(nullptr, IDI_APPLICATION);
  wc.hCursor = ::LoadCursorW(nullptr, IDC_ARROW);
  wc.lpszClassName = kWindowClassName;

  wnd_class_atom_ = ::RegisterClassExW(&wc);
  if (wnd_class_atom_ == 0) {
    auto err = ::GetLastError();
    std::error_code ec{static_cast<int>(err), std::system_category()};
    throw std::system_error(ec, kWindowClassRegisterFailure);
  }
}

WinDisplay::~WinDisplay() {
  ::UnregisterClassW(reinterpret_cast<LPCWSTR>(wnd_class_atom_), hinstance_);
}

void WinDisplay::HandleEvents() {
  ::WaitMessage();

  ::MSG msg;
  while (::PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
    ::TranslateMessage(&msg);
    ::DispatchMessageW(&msg);
  }
}

void WinDisplay::PostEmptyEvent() { ::PostMessageW(nullptr, WM_NULL, 0, 0); }

::LRESULT CALLBACK WinDisplay::WindowProc(::HWND handle, ::UINT msg,
                                          ::WPARAM wparam,
                                          ::LPARAM lparam) noexcept {
  WinWindow *window =
      reinterpret_cast<WinWindow *>(::GetWindowLongPtrW(handle, GWLP_USERDATA));

  if (window == nullptr && msg == WM_NCCREATE) {
    CREATESTRUCTW *cs = reinterpret_cast<CREATESTRUCTW *>(lparam);
    ::SetWindowLongPtrW(handle, GWLP_USERDATA,
                        reinterpret_cast<LONG_PTR>(cs->lpCreateParams));
    return TRUE;
  } else if (window) {
    return window->WindowProc(msg, wparam, lparam);
  }

  return DefWindowProcW(handle, msg, wparam, lparam);
}

WinWindow::WinWindow(WinDisplay &display, const std::string &title,
                     uint32_t width, uint32_t height) {
  handle_ = ::CreateWindowExW(
      WS_EX_APPWINDOW,
      reinterpret_cast<::LPCWSTR>(display.GetWindowClassAtom()), L"",
      WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_SYSMENU | WS_MINIMIZEBOX |
          WS_CAPTION | WS_MAXIMIZEBOX | WS_THICKFRAME,
      CW_USEDEFAULT, CW_USEDEFAULT, width, height, nullptr, nullptr,
      display.GetInstanceHandle(), this);
  if (!handle_) {
    auto err = ::GetLastError();
    std::error_code ec{static_cast<int>(err), std::system_category()};
    throw std::system_error(ec, "Failed to create window");
  }

  ::ShowWindow(handle_, SW_SHOWNA);

  device_context_ = ::GetDC(handle_);
}

WinWindow::~WinWindow() {
  ::ReleaseDC(handle_, device_context_);
  ::DestroyWindow(handle_);
}

void WinWindow::SetPosition(int32_t x, int32_t y) {
  ::RECT rect = {x, y, x, y};

  ::AdjustWindowRectEx(&rect, ::GetWindowLongW(handle_, GWL_STYLE), FALSE,
                       ::GetWindowLongW(handle_, GWL_EXSTYLE));
  ::SetWindowPos(handle_, nullptr, rect.left, rect.top, 0, 0,
                 SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE);
}

void WinWindow::GetPosition(int32_t &x, int32_t &y) const noexcept {
  ::POINT pos;
  ::ClientToScreen(handle_, &pos);

  x = pos.x;
  y = pos.y;
}

void WinWindow::SetSize(uint32_t width, uint32_t height) {
  ::RECT rect = {0, 0, static_cast<LONG>(width), static_cast<LONG>(height)};
  ::AdjustWindowRectEx(&rect, ::GetWindowLongW(handle_, GWL_STYLE), FALSE,
                       ::GetWindowLongW(handle_, GWL_EXSTYLE));
  ::SetWindowPos(
      handle_, nullptr, 0, 0, rect.right - rect.left, rect.bottom - rect.top,
      SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOMOVE | SWP_NOZORDER);
}

void WinWindow::GetSize(uint32_t &width, uint32_t &height) const noexcept {
  ::RECT area;
  ::GetClientRect(handle_, &area);

  width = area.right;
  height = area.bottom;
}

::LRESULT WinWindow::WindowProc(::UINT msg, ::WPARAM wparam,
                                ::LPARAM lparam) noexcept {
  switch (msg) {
    case WM_CLOSE:
      CallCloseCallback();
      return 0;
    default:
      return DefWindowProcW(handle_, msg, wparam, lparam);
  }
}

}  // namespace engine::graphics::windows
