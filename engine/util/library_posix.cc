#include "library.h"

extern "C" {
#include <dlfcn.h>
}

#include <stdexcept>

namespace util {

Library::Library(const char *name) {
  handle_ = dlopen(name, RTLD_NOW | RTLD_LOCAL);
  if (!handle_) {
    throw std::runtime_error("Unable to open library");
  }
}

Library::~Library() { dlclose(handle_); }

void *Library::GetSymbolAddressRaw(const char *symbol_name) {
  dlerror();
  void *symbol = dlsym(handle_, symbol_name);
  if (symbol == nullptr) {
    // symbol can actually be null so check if there is an error
    char *error = dlerror();
    if (error) {
      // An error occured
      throw std::runtime_error(error);
    }
  }

  return symbol;
}

}  // namespace util
