#pragma once

#include <string>

namespace util {

class Library {
 public:
  Library(const std::string &name) : Library(name.c_str()) {}
  Library(const char *name);
  ~Library();

  template <typename T>
  T GetSymbolAddress(const char *symbol_address) {
    return reinterpret_cast<T>(GetSymbolAddressRaw(symbol_address));
  }

 private:
  void *handle_;

  void *GetSymbolAddressRaw(const char *symbol_address);
};

}  // namespace util
