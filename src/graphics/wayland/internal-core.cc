#include "internal-core.h"

namespace graphics::wayland::internal::functions {

types::WlDisplayConnectProc WlDisplayConnect = wl_display_connect;
types::WlDisplayConnectToFdProc WlDisplayConnectToFd = wl_display_connect_to_fd;
types::WlDisplayDisconnectProc WlDisplayDisconnect = wl_display_disconnect;

}  // namespace graphics::wayland::internal::functions
