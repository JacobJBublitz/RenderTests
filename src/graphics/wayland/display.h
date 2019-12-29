#pragma once

#include "graphics/egl/display.h"
#include "graphics/wayland/internal/wayland.h"
#include "graphics/wayland/internal/xdg_decoration_unstable_v1.h"
#include "graphics/wayland/internal/xdg_shell.h"
#include "graphics/window.h"

namespace graphics::wayland {

class WlDisplay final : public Display, public egl::EglNativeDisplay {
 public:
  WlDisplay(const WlDisplay &) = delete;  // No copy

  ~WlDisplay() final;

  static WlDisplay Connect(const std::string &name);
  static WlDisplay ConnectToDefault();
  static WlDisplay ConnectToFileDescriptor(int fd);

  void HandleEvents() final;

  egl::internal::Enum GetEglPlatform() final {
    return egl::internal::extensions::KHR_platform_wayland::PlatformWaylandKhr;
  }
  void *GetEglNativeDisplayHandle() final { return handle_; }
  void PopulateEglAttribList(
      std::vector<egl::internal::Attrib> &attrib_list) final {}

  internal::Compositor *GetCompositor() noexcept { return compositor_; }

  internal::Shell *GetShell() noexcept { return shell_; }

  internal::Shm *GetShm() noexcept { return shm_; }

  internal::XdgWmBase *GetXdgWmBase() noexcept { return xdg_wm_base_; }

  internal::ZxdgDecorationManagerV1 *GetZxdgDecorationManagerV1() noexcept {
    return zxdg_decoration_manager_v1_;
  }

 private:
  static const internal::Pointer::Listener PointerListener;
  static const internal::Registry::Listener RegistryListener;
  static const internal::Seat::Listener SeatListener;
  static const internal::XdgWmBase::Listener XdgWmBaseListener;

  internal::Display *handle_ = nullptr;
  internal::Compositor *compositor_ = nullptr;
  internal::Shell *shell_ = nullptr;
  internal::Shm *shm_ = nullptr;
  internal::Seat *seat_ = nullptr;
  internal::XdgWmBase *xdg_wm_base_ = nullptr;
  internal::ZxdgDecorationManagerV1 *zxdg_decoration_manager_v1_ = nullptr;

  internal::Pointer *pointer_ = nullptr;
  internal::Surface *pointer_focus_ = nullptr;

  WlDisplay(internal::Display *handle);

  static void PointerEnterHandler(void *data, internal::Pointer *pointer,
                                  uint32_t serial, internal::Surface *surface,
                                  internal::Fixed x,
                                  internal::Fixed y) noexcept;
  static void PointerLeaveHandler(void *data, internal::Pointer *pointer,
                                  uint32_t serial,
                                  internal::Surface *surface) noexcept;
  static void PointerMotionHandler(void *data, internal::Pointer *pointer,
                                   uint32_t time, internal::Fixed x,
                                   internal::Fixed y) noexcept;
  static void PointerButtonHandler(void *data, internal::Pointer *pointer,
                                   uint32_t serial, uint32_t time,
                                   uint32_t button,
                                   internal::PointerButtonState state) noexcept;
  static void PointerAxisHandler(void *data, internal::Pointer *pointer,
                                 uint32_t time, internal::PointerAxis axis,
                                 internal::Fixed value) noexcept;
  static void PointerFrameHandler(void *data,
                                  internal::Pointer *pointer) noexcept;
  static void PointerAxisSourceHandler(
      void *data, internal::Pointer *pointer,
      internal::PointerAxisSource source) noexcept;
  static void PointerAxisStopHandler(void *data, internal::Pointer *pointer,
                                     uint32_t time,
                                     internal::PointerAxis axis) noexcept;
  static void PointerAxisDiscreteHandler(void *data, internal::Pointer *pointer,
                                         internal::PointerAxis axis,
                                         int32_t discrete) noexcept;

  static void RegistryGlobal(void *data, internal::Registry *registry,
                             uint32_t name, const char *interface,
                             uint32_t version) noexcept;
  static void RegistryGlobalRemove(void *data, internal::Registry *registry,
                                   uint32_t name) noexcept;
  static void SeatCapabilitiesHandler(void *data, internal::Seat *seat,
                                      internal::SeatCapability capabilities);
  static void SeatNameHandler(void *data, internal::Seat *seat,
                              const char *name);
  static void XdgWmBasePing(void *data, internal::XdgWmBase *xdg_wm_base,
                            uint32_t serial) noexcept;
};

}  // namespace graphics::wayland
