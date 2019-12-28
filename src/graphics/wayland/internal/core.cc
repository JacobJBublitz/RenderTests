#include "core.h"

#include <locale>
#include <vector>

extern "C" {
#include <wayland-client-core.h>
}
#include <wayland-client-protocol.h>

namespace graphics::wayland::internal {

Display *DisplayBase::Connect(const char *name) {
  return reinterpret_cast<Display *>(wl_display_connect(name));
}

Display *DisplayBase::ConnectToFd(int fd) {
  return reinterpret_cast<Display *>(wl_display_connect_to_fd(fd));
}

void DisplayBase::Disconnect() {
  wl_display_disconnect(reinterpret_cast<wl_display *>(this));
}

int DisplayBase::Dispatch() {
  return wl_display_dispatch(reinterpret_cast<wl_display *>(this));
}

int DisplayBase::GetError() {
  return wl_display_get_error(reinterpret_cast<wl_display *>(this));
}

int DisplayBase::Roundtrip() {
  return wl_display_roundtrip(reinterpret_cast<wl_display *>(this));
}

void Proxy::Destroy() { wl_proxy_destroy(reinterpret_cast<wl_proxy *>(this)); }

int Proxy::AddListener(void (**implementation)(void), void *data) {
  return wl_proxy_add_listener(reinterpret_cast<wl_proxy *>(this),
                               implementation, data);
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

}  // namespace graphics::wayland::internal
