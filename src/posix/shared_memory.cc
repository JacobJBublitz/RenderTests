#include "shared_memory.h"

extern "C" {
#include <sys/mman.h>
#include <unistd.h>
}

namespace posix {

SharedMemory::SharedMemory(size_t size) {
  size_ = size;
  fd_ = memfd_create("shm", 0);
  ftruncate(fd_, size);

  data_ = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_, 0);
}

SharedMemory::~SharedMemory() {
  munmap(data_, size_);
  close(fd_);
}

}  // namespace posix