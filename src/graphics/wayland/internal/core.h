#pragma once

#include <cstdint>
#include <vector>

#include "util.h"

namespace graphics::wayland::internal {

class Display;
class DisplayBase;
class EventQueue;
class Proxy;

class EventQueue {
 public:
  void Destroy();
};
static_assert(sizeof(EventQueue) == 1);

class Proxy {
 public:
  Proxy() = delete;
  ~Proxy() = delete;

  Proxy *Create(const Interface *interface);
  Proxy *CreateWrapper();

  void Destroy();
  void WrapperDestroy();

  int AddListener(void (**implementation)(void), void *data);
  const void *GetListener();

  int AddDispatcher(DispatcherFunc dispatcher_func, const void *dispatcher_data,
                    void *data);

  void SetUserData(void *user_data);
  void *GetUserData();

  uint32_t GetVersion();
  uint32_t GetId();
  const char *GetClass();

  void SetQueue(EventQueue *queue);

  template <typename T, typename... Args>
  void Marshal(uint32_t opcode, T arg, Args... args) {
    std::vector<Argument> arg_array;
    PopulateArgumentArray(arg_array, arg, args...);
    MarshalArray(opcode, arg_array.data());
  }
  void Marshal(uint32_t opcode) { MarshalArray(opcode, nullptr); }
  void MarshalArray(uint32_t opcode, Argument *args);
  template <typename... Args>
  Proxy *MarshalConstructor(uint32_t opcode, const Interface *interface,
                            Args... args) {
    std::vector<Argument> arg_array;
    PopulateArgumentArray(arg_array, args...);
    return MarshalArrayConstructor(opcode, arg_array.data(), interface);
  }
  template <typename... Args>
  Proxy *MarshalConstructorVersioned(uint32_t opcode,
                                     const Interface *interface,
                                     uint32_t version, Args... args) {
    std::vector<Argument> arg_array;
    PopulateArgumentArray(arg_array, args...);
    return MarshalArrayConstructorVersioned(opcode, arg_array.data(), interface,
                                            version);
  }
  Proxy *MarshalArrayConstructor(uint32_t opcode, Argument *args,
                                 const Interface *interface);
  Proxy *MarshalArrayConstructorVersioned(uint32_t opcode, Argument *args,
                                          const Interface *interface,
                                          uint32_t version);

 private:
  template <typename T, typename... Args>
  void PopulateArgumentArray(std::vector<Argument> &arg_array, T arg,
                             Args &&... args) {
    Argument a;
    arg_array.emplace_back(arg);
    PopulateArgumentArray(arg_array, args...);
  }
  void PopulateArgumentArray(std::vector<Argument> &arg_array) {}
};
static_assert(sizeof(Proxy) == 1);

class DisplayBase : public Proxy {
 public:
  static Display *Connect(const char *name);
  static Display *ConnectToFd(int fd);

  void Disconnect();

  EventQueue *CreateQueue();

  int Flush();

  int Dispatch();
  int DispatchQueue(EventQueue *queue);
  int DispatchQueuePending(EventQueue *queue);
  int DispatchPending();

  int PrepareRead();
  int PrepareReadQueue(EventQueue *queue);
  int CancelRead();
  int ReadEvents();

  int Roundtrip();
  int RoundtripQueue(EventQueue *queue);

  int GetError();
  uint32_t GetProtocolError(const Interface **interface, uint32_t *id);
  int GetFd();
};
static_assert(sizeof(DisplayBase) == 1);

}  // namespace graphics::wayland::internal
