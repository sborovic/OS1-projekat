#include "kernel.h"
#include "utility.h"
#include "iostream.h"
#include "thr_mem.h"

void* ThrMem::operator new(size_t size) {
	LOCK
	void* ptr = ::operator new(size);
	UNLOCK
	return ptr;
}
void ThrMem::operator delete(void* ptr) {
	LOCK
	::operator delete(ptr);
	UNLOCK
}
