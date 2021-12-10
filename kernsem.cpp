#include "kernsem.h"
#include "PCB.h"
#include "kernel.h"
#include "list.h"
#include "SCHEDULE.H"
#include "debug.h"

KernelSem::KernelSem(int init) {
	val = init;
	blockedOnSemaphore = new List<PCB>;
}

KernelSem::~KernelSem() {
	delete blockedOnSemaphore;
}

void KernelSem::block() {
	if (setjmp(Kernel::getInstance().running->context) == 0) {
		blockedOnSemaphore->add(Kernel::getInstance().running);
		Kernel::getInstance().running = Scheduler::get();
		longjmp(Kernel::getInstance().running->context, 1);
	} else return;
}

void KernelSem::unblock() {
	List<PCB>::Iterator it = blockedOnSemaphore->begin();
	PCB* next = *blockedOnSemaphore->remove(it);
	TRACE(("U unblock(), next ima id = ", next->getLocalId()));
	Scheduler::put(next);
}

int KernelSem::wait(Time maxTimeToWait) {
	Kernel::getInstance().lock();
	if (--val < 0) block();
	Kernel::getInstance().unlock();

}

void KernelSem::signal() {
	Kernel::getInstance().lock();
	if (++val <= 0) unblock();
	Kernel::getInstance().unlock();

}
