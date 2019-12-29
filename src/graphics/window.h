#pragma once

#include <cstdint>
#include <functional>
#include <string>

namespace graphics {

class Display;
class Window;

class Display {
 public:
  virtual ~Display();

  virtual Window *CreateWindow(const std::string &title, uint32_t width,
                               uint32_t height) = 0;

  virtual void HandleEvents() = 0;
};

class Window {
 public:
  using CloseCallback = std::function<void()>;

  virtual ~Window();

  void SetCloseCallback(CloseCallback callback);

  virtual void SetPosition(int32_t x, int32_t y) = 0;
  virtual void GetPosition(int32_t &x, int32_t &y) const noexcept = 0;

  virtual void SetSize(uint32_t width, uint32_t height) = 0;
  virtual void GetSize(uint32_t &width, uint32_t &height) const noexcept = 0;

 protected:
  void CallCloseCallback();

 private:
  CloseCallback close_callback_;
};

}  // namespace graphics
