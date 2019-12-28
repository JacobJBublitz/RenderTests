#pragma once

#include <stddef.h>

namespace posix {

class SharedMemory final {
 public:
  SharedMemory(size_t size);
  ~SharedMemory();

  int GetFd() { return fd_; }
  size_t GetSize() { return size_; }
  void *GetPointer() { return data_; }

 private:
  int fd_;
  size_t size_;
  void *data_;
};

}  // namespace posix