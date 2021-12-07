#include "kernsem.h"
#include "semaphor.h"

KernelSem::KernelSem(int init) {
	val = init;
}
int KernelSem::wait(Time maxTimeToWait) {

}

void KernelSem::signal() {

}
