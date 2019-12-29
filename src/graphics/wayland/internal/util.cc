#include "util.h"

extern "C" {
#include <wayland-util.h>
}

namespace graphics::wayland::internal {

static_assert(sizeof(Array) == sizeof(wl_array));

Array::Array() { wl_array_init(reinterpret_cast<wl_array *>(this)); }

Array::~Array() { wl_array_release(reinterpret_cast<wl_array *>(this)); }

void *Array::Add(size_t size) {
  return wl_array_add(reinterpret_cast<wl_array *>(this), size);
}

int Array::CopyTo(Array &dest) {
  return wl_array_copy(reinterpret_cast<wl_array *>(&dest),
                       reinterpret_cast<wl_array *>(this));
}

}  // namespace graphics::wayland::internal
