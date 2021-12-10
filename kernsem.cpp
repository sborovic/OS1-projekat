#include "kernsem.h"
#include "PCB.h"
#include "kernel.h"
#include "list.h"
#include "SCHEDULE.H"

KernelSem::KernelSem(int init) {
	val = init;
	blockedOnSemaphore = new List<PCB>;
}

KernelSem::~KernelSem() {
	delete blockedOnSemaphore;
}

void KernelSem::block() {
	PCB* running = Kernel::getInstance().running;
	if (setjmp(running->context) == 0) {
		blockedOnSemaphore->add(running);
		Kernel::getInstance().running = Scheduler::get();
		longjmp(Kernel::getInstance().running->context, 1);
	} else return;
}

void KernelSem::unblock() {
	List<PCB>::Iterator it = blockedOnSemaphore->begin();
	PCB* next = *blockedOnSemaphore->remove(it);
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
