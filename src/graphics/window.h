#pragma once

#include <cstdint>
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
  virtual ~Window();

  virtual void SetPosition(int32_t x, int32_t y) = 0;
  virtual void Position(int32_t &x, int32_t y) const noexcept = 0;

  virtual void SetSize(uint32_t width, uint32_t height) = 0;
  virtual void Size(uint32_t &width, uint32_t &height) const noexcept = 0;

  virtual void SetVisible(bool visible) = 0;
  virtual bool Visible() const noexcept;
};

}  // namespace graphics
