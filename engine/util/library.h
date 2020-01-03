#pragma once

#include <string>

namespace util {

class Library {
 public:
  Library(const std::string &name) : Library(name.c_str()) {}
  Library(const char *name);
  ~Library();

  template <typename T>
  T GetSymbolAddress(const char *symbol_name) {
    return reinterpret_cast<T>(GetSymbolAddressRaw(symbol_name));
  }

 private:
  void *handle_;

  void *GetSymbolAddressRaw(const char *symbol_name);
};

}  // namespace util
