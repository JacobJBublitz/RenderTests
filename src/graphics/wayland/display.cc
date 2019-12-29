#include "display.h"

#include <glog/logging.h>

#include <cmath>
#include <cstring>
#include <iostream>
#include <stdexcept>

#include "graphics/wayland/window.h"

extern "C" {
#include <poll.h>
}

namespace graphics::wayland {

namespace {

struct PointerFrameState {};

}  // namespace

const internal::Pointer::Listener WlDisplay::PointerListener{
    .Enter = PointerEnterHandler,
    .Leave = PointerLeaveHandler,
    .Motion = PointerMotionHandler,
    .Button = PointerButtonHandler,
    .Axis = PointerAxisHandler,
    .Frame = PointerFrameHandler,
    .AxisSource = PointerAxisSourceHandler,
    .AxisStop = PointerAxisStopHandler,
    .AxisDiscrete = PointerAxisDiscreteHandler,
};
const internal::Registry::Listener WlDisplay::RegistryListener{
    .Global = RegistryGlobal,
    .GlobalRemove = RegistryGlobalRemove,
};
const internal::Seat::Listener WlDisplay::SeatListener{
    .Capabilities = SeatCapabilitiesHandler,
    .Name = SeatNameHandler,
};
const internal::XdgWmBase::Listener WlDisplay::XdgWmBaseListener{
    .Ping = XdgWmBasePing,
};

WlDisplay::WlDisplay(internal::Display *handle) : handle_(handle) {
  if (!handle) {
    throw std::runtime_error("Invalid wayland display handle");
  }
  internal::Registry *registry = handle->GetRegistry();
  registry->AddListener(RegistryListener, this);
  handle_->Dispatch();
  handle_->Roundtrip();
  registry->Destroy();

  LOG_IF(FATAL, !compositor_)
      << "The required interface '" << internal::CompositorInterface.Name
      << "' was not exposed by the server.";
  LOG_IF(FATAL, !shell_) << "The required interface '"
                         << internal::ShellInterface.Name
                         << "' was not exposed by the server.";
  LOG_IF(FATAL, !shm_) << "The required interface '"
                       << internal::ShmInterface.Name
                       << "' was not exposed by the server.";
  LOG_IF(FATAL, !seat_) << "The required interface '"
                        << internal::SeatInterface.Name
                        << "' was not exposed by the server.";

  LOG_IF(INFO, !zxdg_decoration_manager_v1_)
      << "Optional wayland interface '"
      << internal::ZxdgDecorationManagerV1Interface.Name
      << "' was not found. Window decorations will be done on the "
         "client side\n";
}

WlDisplay::~WlDisplay() {
  if (pointer_) pointer_->Release();

  if (compositor_) compositor_->Destroy();
  if (shell_) shell_->Destroy();
  if (shm_) shm_->Destroy();
  if (seat_) seat_->Release();
  if (xdg_wm_base_) xdg_wm_base_->Destroy();
  if (zxdg_decoration_manager_v1_) zxdg_decoration_manager_v1_->Destroy();
  handle_->Disconnect();
}

WlDisplay WlDisplay::Connect(const std::string &name) {
  auto handle = internal::DisplayBase::Connect(name.c_str());

  return WlDisplay(handle);
}

WlDisplay WlDisplay::ConnectToDefault() {
  auto handle = internal::DisplayBase::Connect(nullptr);

  return WlDisplay(handle);
}

WlDisplay WlDisplay::ConnectToFileDescriptor(int fd) {
  auto handle = internal::DisplayBase::ConnectToFd(fd);

  return WlDisplay(handle);
}

Window *WlDisplay::CreateWindow(const std::string &title, uint32_t width,
                                uint32_t height) {
  return new WlWindow(*this, title, width, height);
}

void WlDisplay::HandleEvents() {
  pollfd fds[] = {
      {handle_->GetFd(), POLLIN, 0},
  };

  while (handle_->PrepareRead() != 0) handle_->DispatchPending();

  if (handle_->Flush() < 0 && errno != EAGAIN) {
    LOG(ERROR) << "Unable to flush wayland display. We may have disconnected "
                  "from the session.";

    // TODO: Tell all windows to close

    handle_->CancelRead();
  }

  if (poll(fds, 1, -1) > 0) {
    if (fds[0].revents & POLLIN) {
      handle_->ReadEvents();
      handle_->DispatchPending();
    } else {
      handle_->CancelRead();
    }
  } else {
    handle_->CancelRead();
  }
}

void WlDisplay::PointerEnterHandler(void *data, internal::Pointer *pointer,
                                    uint32_t serial, internal::Surface *surface,
                                    internal::Fixed x,
                                    internal::Fixed y) noexcept {
  WlDisplay *this_ptr = reinterpret_cast<WlDisplay *>(data);

  this_ptr->pointer_focus_ = surface;
}
void WlDisplay::PointerLeaveHandler(void *data, internal::Pointer *pointer,
                                    uint32_t serial,
                                    internal::Surface *surface) noexcept {
  WlDisplay *this_ptr = reinterpret_cast<WlDisplay *>(data);
  this_ptr->pointer_focus_ = nullptr;
}
void WlDisplay::PointerMotionHandler(void *data, internal::Pointer *pointer,
                                     uint32_t time, internal::Fixed x,
                                     internal::Fixed y) noexcept {
  WlDisplay *this_ptr = reinterpret_cast<WlDisplay *>(data);
  if (this_ptr->pointer_focus_) {
    WlSurface *surface =
        reinterpret_cast<WlSurface *>(this_ptr->pointer_focus_->GetUserData());
    surface->HandlePointerMotion(time, x.ToDouble(), y.ToDouble());
  }
}
void WlDisplay::PointerButtonHandler(
    void *data, internal::Pointer *pointer, uint32_t serial, uint32_t time,
    uint32_t button, internal::PointerButtonState state) noexcept {}
void WlDisplay::PointerAxisHandler(void *data, internal::Pointer *pointer,
                                   uint32_t time, internal::PointerAxis axis,
                                   internal::Fixed value) noexcept {}
void WlDisplay::PointerFrameHandler(void *data,
                                    internal::Pointer *pointer) noexcept {}
void WlDisplay::PointerAxisSourceHandler(
    void *data, internal::Pointer *pointer,
    internal::PointerAxisSource source) noexcept {}
void WlDisplay::PointerAxisStopHandler(void *data, internal::Pointer *pointer,
                                       uint32_t time,
                                       internal::PointerAxis axis) noexcept {}
void WlDisplay::PointerAxisDiscreteHandler(void *data,
                                           internal::Pointer *pointer,
                                           internal::PointerAxis axis,
                                           int32_t discrete) noexcept {}

void WlDisplay::RegistryGlobal(void *data, internal::Registry *registry,
                               uint32_t name, const char *interface,
                               uint32_t version) noexcept {
  WlDisplay *this_ptr = reinterpret_cast<WlDisplay *>(data);

  if (strcmp(interface, internal::CompositorInterface.Name) == 0)
    this_ptr->compositor_ = reinterpret_cast<internal::Compositor *>(
        registry->Bind(name, &internal::CompositorInterface, version));
  else if (strcmp(interface, internal::ShellInterface.Name) == 0)
    this_ptr->shell_ = reinterpret_cast<internal::Shell *>(
        registry->Bind(name, &internal::ShellInterface, version));
  else if (strcmp(interface, internal::ShmInterface.Name) == 0)
    this_ptr->shm_ = reinterpret_cast<internal::Shm *>(
        registry->Bind(name, &internal::ShmInterface, version));
  else if (strcmp(interface, internal::SeatInterface.Name) == 0) {
    this_ptr->seat_ = reinterpret_cast<internal::Seat *>(
        registry->Bind(name, &internal::SeatInterface, version));
    this_ptr->seat_->AddListener(SeatListener, this_ptr);
  } else if (strcmp(interface, internal::XdgWmBaseInterface.Name) == 0) {
    this_ptr->xdg_wm_base_ = reinterpret_cast<internal::XdgWmBase *>(
        registry->Bind(name, &internal::XdgWmBaseInterface, version));
    this_ptr->xdg_wm_base_->AddListener(XdgWmBaseListener, this_ptr);
  } else if (strcmp(interface,
                    internal::ZxdgDecorationManagerV1Interface.Name) == 0)
    this_ptr->zxdg_decoration_manager_v1_ =
        reinterpret_cast<internal::ZxdgDecorationManagerV1 *>(registry->Bind(
            name, &internal::ZxdgDecorationManagerV1Interface, version));
}

void WlDisplay::RegistryGlobalRemove(void *data, internal::Registry *registry,
                                     uint32_t name) noexcept {}

void WlDisplay::SeatCapabilitiesHandler(void *data, internal::Seat *seat,
                                        internal::SeatCapability capabilities) {
  WlDisplay *this_ptr = reinterpret_cast<WlDisplay *>(data);

  LOG(INFO) << "Seat has pointer: "
            << (capabilities & internal::SeatCapability::kPointer);
  if (capabilities & internal::SeatCapability::kPointer) {
    this_ptr->pointer_ = seat->GetPointer();
    this_ptr->pointer_->AddListener(PointerListener, this_ptr);
  } else if (this_ptr->pointer_) {
    this_ptr->pointer_->Release();
    this_ptr->pointer_ = nullptr;
  }
}

void WlDisplay::SeatNameHandler(void *data, internal::Seat *seat,
                                const char *name) {}

void WlDisplay::XdgWmBasePing(void *data, internal::XdgWmBase *xdg_wm_base,
                              uint32_t serial) noexcept {
  xdg_wm_base->Pong(serial);
}

}  // namespace graphics::wayland
