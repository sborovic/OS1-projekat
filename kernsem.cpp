#include "kernsem.h"
#include "PCB.h"
#include "kernel.h"
#include "list.h"
#include "SCHEDULE.H"
#include "debug.h"
#include "thread.h"

KernelSem::KernelSem(int init) {
	val = init;
	blockedOnSemaphore = new List<PCB>;
}

KernelSem::~KernelSem() {
	delete blockedOnSemaphore;
}

int KernelSem::wait(Time maxTimeToWait) {
	Kernel::getInstance().lock();
	TRACE(("\npocetak KernelSem::wait(), val = %d", val));
	if (--val < 0) {
		PCB* running = Kernel::getInstance().running;
		running->state = PCB::blocked;
		TRACE(("\nubacujem u blockedOnSemaphore sa id = %d", running->getLocalId()));
		blockedOnSemaphore->add(running);
		Kernel::getInstance().unlock();
		dispatch();
	} else Kernel::getInstance().unlock();

}

void KernelSem::signal() {
	Kernel::getInstance().lock();
	TRACE(("\npocetak KernelSem::signal(), val = %d", val));
	if (++val <= 0) {
		List<PCB>::Iterator it = blockedOnSemaphore->begin();
		PCB* next = *it;
		blockedOnSemaphore->remove(it);
		next->state = PCB::ready;
		TRACE(("\nUzimam iz blockedOnSemaphore, U unblock(), next ima id = %d", next->getLocalId()));
		Scheduler::put(next);
	}
	Kernel::getInstance().unlock();
}
