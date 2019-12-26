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

struct Fixed {
 private:
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
