#include "core.h"

#include <cstdarg>
#include <locale>
#include <vector>

extern "C" {
#include <wayland-client-core.h>
}

namespace graphics::wayland::internal {

namespace {

std::vector<Argument> VaListToArguments(const Message &message, va_list vl) {
  std::vector<Argument> args;

  for (const char *c = message.Signature; *c; c++) {
    // If it's a number skip it
    if (std::isdigit(*c)) continue;

    // If it's a question mark skip it
    if (*c == '?') continue;

    Argument arg;

    switch (*c) {
      case 'i':
        arg.Int = va_arg(vl, int32_t);
        break;
      case 'u':
        arg.UInt = va_arg(vl, uint32_t);
        break;
      case 'f':
        arg.Fixed = va_arg(vl, Fixed);
        break;
      case 's':
        arg.String = va_arg(vl, const char *);
        break;
      case 'o':
        arg.Object = va_arg(vl, Object *);
        break;
      case 'n':
        arg.NewId = va_arg(vl, uint32_t);
        break;
      case 'a':
        arg.Array = va_arg(vl, Array *);
        break;
      case 'h':
        arg.Fd = va_arg(vl, int);
        break;
    }

    args.push_back(arg);
  }

  return args;
}

}  // namespace

Display *DisplayBase::Connect(const char *name) {
  return reinterpret_cast<Display *>(wl_display_connect(name));
}

Display *DisplayBase::ConnectToFd(int fd) {
  return reinterpret_cast<Display *>(wl_display_connect_to_fd(fd));
}

void DisplayBase::Disconnect() {
  wl_display_disconnect(reinterpret_cast<wl_display *>(this));
}

int DisplayBase::Roundtrip() {
  return wl_display_roundtrip(reinterpret_cast<wl_display *>(this));
}

void Proxy::Destroy() { wl_proxy_destroy(reinterpret_cast<wl_proxy *>(this)); }

int Proxy::AddListener(void (**implementation)(void), void *data) {
  return wl_proxy_add_listener(reinterpret_cast<wl_proxy *>(this),
                               implementation, data);
}

Proxy *Proxy::MarshalConstructor(uint32_t opcode, const Interface *interface,
                                 ...) {
  va_list vl;
  va_start(vl, interface);
  auto args = VaListToArguments(interface->Methods[opcode], vl);
  va_end(vl);

  return MarshalArrayConstructor(opcode, args.data(), interface);
}

Proxy *Proxy::MarshalArrayConstructor(uint32_t opcode, Argument *args,
                                      const Interface *interface) {
  return reinterpret_cast<Proxy *>(wl_proxy_marshal_array_constructor(
      reinterpret_cast<wl_proxy *>(this), opcode,
      reinterpret_cast<wl_argument *>(args),
      reinterpret_cast<const wl_interface *>(interface)));
}

Proxy *Proxy::MarshalConstructorVersioned(uint32_t opcode,
                                          const Interface *interface,
                                          uint32_t version, ...) {
  va_list vl;
  va_start(vl, version);
  auto args = VaListToArguments(interface->Methods[opcode], vl);
  va_end(vl);

  return MarshalArrayConstructorVersioned(opcode, args.data(), interface,
                                          version);
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
