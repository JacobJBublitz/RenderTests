#pragma once

#include <wayland-client-protocol.h>

namespace graphics::wayland::internal {

namespace types {

struct Message;

using Buffer = ::wl_buffer;
using Compositor = ::wl_compositor;
using Display = ::wl_display;
struct Interface {
  const char *Name;
  int Version;
  int MethodCount;
  const Message *Methods;
  int EventCount;
  const Message *Events;
};
struct Message {
  const char *Name;
  const char *Signature;
  const Interface **Types;
};
using Registry = ::wl_registry;
struct RegistryListener {
  void (*Global)(void *data, Registry *registry, uint32_t name,
                 const char *interface, uint32_t version);
  void (*GlobalRemove)(void *data, Registry *registry, uint32_t name);
};
using Shm = ::wl_shm;
using Surface = ::wl_surface;

}  // namespace types

namespace functions {

namespace types {

using WlDisplayConnectProc = internal::types::Display *(*)(const char *name);
using WlDisplayConnectToFdProc = internal::types::Display *(*)(int fd);
using WlDisplayDisconnectProc = void (*)(internal::types::Display *display);
using WlDisplayGetRegistryProc =
    internal::types::Registry *(*)(internal::types::Display *display);
using WlDisplayRoundtripProc = int (*)(internal::types::Display *display);
using WlRegistryAddListenerProc =
    int (*)(internal::types::Registry *registry,
            const internal::types::RegistryListener *listener, void *data);
using WlRegistryDestroyProc = void (*)(internal::types::Registry *registry);

}  // namespace types

extern types::WlDisplayConnectProc WlDisplayConnect;
extern types::WlDisplayConnectToFdProc WlDisplayConnectToFd;
extern types::WlDisplayDisconnectProc WlDisplayDisconnect;
static types::WlDisplayGetRegistryProc WlDisplayGetRegistry =
    wl_display_get_registry;
static types::WlDisplayRoundtripProc WlDisplayRoundtrip = wl_display_roundtrip;
static types::WlRegistryAddListenerProc WlRegistryAddListener =
    reinterpret_cast<types::WlRegistryAddListenerProc>(
        wl_registry_add_listener);
static types::WlRegistryDestroyProc WlRegistryDestroy = wl_registry_destroy;

}  // namespace functions

}  // namespace graphics::wayland::internal
