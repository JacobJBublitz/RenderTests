#include "core.h"

#include <locale>
#include <vector>

extern "C" {
#include <wayland-client-core.h>
}
#include <wayland-client-protocol.h>

namespace graphics::wayland::internal {

void DisplayBase::CancelRead() {
  return wl_display_cancel_read(reinterpret_cast<wl_display *>(this));
}

Display *DisplayBase::Connect(const char *name) {
  return reinterpret_cast<Display *>(wl_display_connect(name));
}

Display *DisplayBase::ConnectToFd(int fd) {
  return reinterpret_cast<Display *>(wl_display_connect_to_fd(fd));
}

EventQueue *DisplayBase::CreateQueue() {
  return reinterpret_cast<EventQueue *>(
      wl_display_create_queue(reinterpret_cast<wl_display *>(this)));
}

void DisplayBase::Disconnect() {
  wl_display_disconnect(reinterpret_cast<wl_display *>(this));
}

int DisplayBase::Dispatch() {
  return wl_display_dispatch(reinterpret_cast<wl_display *>(this));
}

int DisplayBase::DispatchPending() {
  return wl_display_dispatch_pending(reinterpret_cast<wl_display *>(this));
}

int DisplayBase::DispatchQueue(EventQueue *queue) {
  return wl_display_dispatch_queue(reinterpret_cast<wl_display *>(this),
                                   reinterpret_cast<wl_event_queue *>(queue));
}

int DisplayBase::DispatchQueuePending(EventQueue *queue) {
  return wl_display_dispatch_queue_pending(
      reinterpret_cast<wl_display *>(this),
      reinterpret_cast<wl_event_queue *>(queue));
}

int DisplayBase::Flush() {
  return wl_display_flush(reinterpret_cast<wl_display *>(this));
}

int DisplayBase::GetError() {
  return wl_display_get_error(reinterpret_cast<wl_display *>(this));
}

int DisplayBase::GetFd() {
  return wl_display_get_fd(reinterpret_cast<wl_display *>(this));
}

int DisplayBase::PrepareRead() {
  return wl_display_prepare_read(reinterpret_cast<wl_display *>(this));
}

int DisplayBase::PrepareReadQueue(EventQueue *queue) {
  return wl_display_prepare_read_queue(
      reinterpret_cast<wl_display *>(this),
      reinterpret_cast<wl_event_queue *>(queue));
}

int DisplayBase::ReadEvents() {
  return wl_display_read_events(reinterpret_cast<wl_display *>(this));
}

int DisplayBase::Roundtrip() {
  return wl_display_roundtrip(reinterpret_cast<wl_display *>(this));
}

void Proxy::Destroy() { wl_proxy_destroy(reinterpret_cast<wl_proxy *>(this)); }

int Proxy::AddListener(void (**implementation)(void), void *data) {
  return wl_proxy_add_listener(reinterpret_cast<wl_proxy *>(this),
                               implementation, data);
}

void *Proxy::GetUserData() {
  return wl_proxy_get_user_data(reinterpret_cast<wl_proxy *>(this));
}

void Proxy::MarshalArray(uint32_t opcode, Argument *args) {
  return wl_proxy_marshal_array(reinterpret_cast<wl_proxy *>(this), opcode,
                                reinterpret_cast<wl_argument *>(args));
}

Proxy *Proxy::MarshalArrayConstructor(uint32_t opcode, Argument *args,
                                      const Interface *interface) {
  return reinterpret_cast<Proxy *>(wl_proxy_marshal_array_constructor(
      reinterpret_cast<wl_proxy *>(this), opcode,
      reinterpret_cast<wl_argument *>(args),
      reinterpret_cast<const wl_interface *>(interface)));
}

Proxy *Proxy::MarshalArrayConstructorVersioned(uint32_t opcode, Argument *args,
                                               const Interface *interface,
                                               uint32_t version) {
  return reinterpret_cast<Proxy *>(wl_proxy_marshal_array_constructor_versioned(
      reinterpret_cast<wl_proxy *>(this), opcode,
      reinterpret_cast<wl_argument *>(args),
      reinterpret_cast<const wl_interface *>(interface), version));
}

void Proxy::SetUserData(void *user_data) {
  wl_proxy_set_user_data(reinterpret_cast<wl_proxy *>(this), user_data);
}

}  // namespace graphics::wayland::internal
