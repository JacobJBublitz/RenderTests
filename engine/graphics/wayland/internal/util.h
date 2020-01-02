#pragma once

#include <cstdarg>
#include <cstddef>
#include <cstdint>

namespace graphics::wayland::internal {

union Argument;
class Array;
struct Fixed;
struct Interface;
struct Message;
struct Object {};

using DispatcherFunc = int (*)(const void *dispatcher_data, void *target,
                               uint32_t opcode, const Message *message,
                               Argument *args);
using LogFunc = void (*)(const char *message, va_list);

class Array {
 public:
  Array();
  ~Array();

  void *Add(size_t size);

  int CopyTo(Array &dest);

  size_t GetSize() const noexcept { return size_; }

  void *GetData() noexcept { return data_; }

 private:
  size_t size_;
  size_t alloc_;
  void *data_;
};

struct Fixed {
 public:
  double ToDouble() {
    union {
      double d;
      int64_t i;
    } u;

    u.i = ((1023LL + 44LL) << 52) + (1LL << 51) + private_;

    return u.d - (3LL << 43);
  }

  static Fixed FromDouble(double d) {
    union {
      double d;
      int64_t i;
    } u;

    u.d = d + (3LL << (51 - 8));

    return {static_cast<int32_t>(u.i)};
  }

  int ToInt() { return private_ / 256; }

  static Fixed FromInt(int i) { return {i * 256}; }

 private:
  Fixed(int32_t p) : private_(p) {}

  int32_t private_;
};
static_assert(sizeof(Fixed) == sizeof(int32_t));

union Argument {
  int32_t Int;
  uint32_t UInt;
  internal::Fixed Fixed;
  const char *String;
  internal::Object *Object;
  uint32_t NewId;
  internal::Array *Array;
  int32_t Fd;

  Argument() : Object(nullptr) {}

  Argument(int32_t val) : Int(val) {}
  Argument(uint32_t val) : UInt(val) {}
  Argument(internal::Fixed val) : Fixed(val) {}
  Argument(const char *val) : String(val) {}
  Argument(internal::Object *val) : Object(val) {}
  Argument(std::nullptr_t) : Object(nullptr) {}
};

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

}  // namespace graphics::wayland::internal
