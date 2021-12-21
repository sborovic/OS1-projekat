#include "safemem.h"

#include "kernel.h"

void *SafeMemory::operator new(size_t size) {
  LOCK
  void *ptr = ::operator new(size);
  UNLOCK
  return ptr;
}
void SafeMemory::operator delete(void *ptr) {
  LOCK
  ::operator delete(ptr);
  UNLOCK
}
