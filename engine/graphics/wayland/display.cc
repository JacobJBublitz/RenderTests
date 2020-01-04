#include "display.h"

#include <glog/logging.h>

#include <cmath>
#include <cstring>
#include <iostream>
#include <stdexcept>

#include "engine/graphics/wayland/window.h"

extern "C" {
#include <poll.h>
}

namespace graphics::wayland {

namespace {

std::list<const char *> kRequiredVulkanExtensions = {
    VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME,
};

}

using namespace internal;

const internal::Pointer::Listener WlDisplay::kPointerListener{
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
const internal::Seat::Listener WlDisplay::kSeatListener{
    .Capabilities = SeatCapabilitiesHandler,
    .Name = SeatNameHandler,
};
const internal::XdgWmBase::Listener WlDisplay::kXdgWmBaseListener{
    .Ping = [](void *, XdgWmBase *xdg_wm_base,
               uint32_t serial) { xdg_wm_base->Pong(serial); },
};

WlDisplay::WlDisplay(internal::Display *handle) : handle_(handle) {
  if (!handle_) {
    throw std::runtime_error("Invalid wayland display handle");
  }
  internal::Registry *registry = handle_->GetRegistry();
  registry->AddListener(
      {
          .Global =
              [](void *data, internal::Registry *registry, uint32_t name,
                 const char *interface, uint32_t version) {
                WlDisplay *this_ptr = reinterpret_cast<WlDisplay *>(data);
                std::string interface_str = interface;

                if (interface_str == CompositorInterface.Name) {
                  this_ptr->compositor_ = reinterpret_cast<Compositor *>(
                      registry->Bind(name, &CompositorInterface, version));
                } else if (interface_str == OutputInterface.Name) {
                  this_ptr->monitors_.emplace_back(reinterpret_cast<Output *>(
                      registry->Bind(name, &OutputInterface, version)));
                } else if (interface_str == ShellInterface.Name) {
                  this_ptr->shell_ = reinterpret_cast<Shell *>(
                      registry->Bind(name, &ShellInterface, version));
                } else if (interface_str == ShmInterface.Name) {
                  this_ptr->shm_ = reinterpret_cast<Shm *>(
                      registry->Bind(name, &ShmInterface, version));
                } else if (interface_str == SeatInterface.Name) {
                  this_ptr->seat_ = reinterpret_cast<Seat *>(
                      registry->Bind(name, &SeatInterface, version));
                  this_ptr->seat_->AddListener(kSeatListener, this_ptr);
                } else if (interface_str == XdgWmBaseInterface.Name) {
                  this_ptr->xdg_wm_base_ = reinterpret_cast<XdgWmBase *>(
                      registry->Bind(name, &XdgWmBaseInterface, version));
                  this_ptr->xdg_wm_base_->AddListener(kXdgWmBaseListener,
                                                      nullptr);
                } else if (interface_str ==
                           ZxdgDecorationManagerV1Interface.Name) {
                  this_ptr->zxdg_decoration_manager_v1_ =
                      reinterpret_cast<ZxdgDecorationManagerV1 *>(
                          registry->Bind(name,
                                         &ZxdgDecorationManagerV1Interface,
                                         version));
                }
              },
          .GlobalRemove = [](void *, internal::Registry *, uint32_t) {},
      },
      this);
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
  LOG(INFO) << "Found " << monitors_.size() << " displays";
}

WlDisplay::~WlDisplay() {
  monitors_.clear();

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

bool WlDisplay::DoesVkQueueSupportPresentation(
    vk::PhysicalDevice physical_device, uint32_t queue_family_index) {
  return physical_device.getWaylandPresentationSupportKHR(
             queue_family_index, reinterpret_cast<wl_display *>(handle_)) ==
         VK_TRUE;
}

const std::list<const char *> &WlDisplay::GetRequiredVkExtensions() {
  return kRequiredVulkanExtensions;
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

void WlDisplay::SeatCapabilitiesHandler(void *data, internal::Seat *seat,
                                        internal::SeatCapability capabilities) {
  WlDisplay *this_ptr = reinterpret_cast<WlDisplay *>(data);

  LOG(INFO) << "Seat has pointer: "
            << (capabilities & internal::SeatCapability::kPointer);
  if (capabilities & internal::SeatCapability::kPointer) {
    this_ptr->pointer_ = seat->GetPointer();
    this_ptr->pointer_->AddListener(kPointerListener, this_ptr);
  } else if (this_ptr->pointer_) {
    this_ptr->pointer_->Release();
    this_ptr->pointer_ = nullptr;
  }
}

void WlDisplay::SeatNameHandler(void *data, internal::Seat *seat,
                                const char *name) {}

}  // namespace graphics::wayland
