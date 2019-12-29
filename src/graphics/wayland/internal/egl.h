#pragma once

#include "graphics/wayland/internal/core.h"
#include "graphics/wayland/internal/wayland.h"

namespace graphics::wayland::internal {

class EglWindow final {
 public:
  static constexpr intptr_t kVersion = 3;

  EglWindow(Surface *surface, int width, int height);
  EglWindow(const EglWindow &) = delete;
  ~EglWindow();

  void Resize(int width, int height, int dx, int dy);

  void GetAttachedSize(int &width, int &height) const noexcept;

  intptr_t GetVersion() const noexcept { return version_; }

  Surface *GetSurface() noexcept { return surface_; }

 private:
  const intptr_t version_;

  int width_;
  int height_;
  int dx_;
  int dy_;

  int attached_width_;
  int attached_height_;

  void *driver_private_;
  void (*resize_callback_)(EglWindow *, void *) = nullptr;
  void (*destroy_window_callback_)(void *) = nullptr;

  Surface *surface_;
};
static_assert(sizeof(EglWindow) == sizeof(intptr_t) + sizeof(int) * 6 +
                                       sizeof(void *) + sizeof(void (*)()) * 2 +
                                       sizeof(Surface *));

}  // namespace graphics::wayland::internal
