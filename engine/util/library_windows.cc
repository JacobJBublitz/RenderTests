#include "library.h"

#define WIN32_LEAN_AND_MEAN
extern "C" {
#include <windows.h>
}

#include <system_error>

namespace util {

Library::Library(const char *name) {
  if (handle_ = LoadLibraryA(name); !handle_) {
    auto err = GetLastError();
    std::error_code ec{static_cast<int>(err), std::system_category()};
    throw std::system_error(ec, "Unable to load library");
  }
}

Library::~Library() { FreeLibrary(reinterpret_cast<HMODULE>(handle_)); }

void *Library::GetSymbolAddressRaw(const char *symbol_address) {
  auto addr =
      GetProcAddress(reinterpret_cast<HMODULE>(handle_), symbol_address);
  if (!addr) {
    auto err = GetLastError();
    std::error_code ec{static_cast<int>(err), std::system_category()};
    throw std::system_error(ec, "Unable to get symbol address");
  }
  
  return addr;
}

}  // namespace util
