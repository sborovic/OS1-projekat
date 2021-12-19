#include "kernsem.h"
#include "PCB.h"
#include "kernel.h"
#include "list.h"
#include "SCHEDULE.H"
#include "debug.h"
#include "thread.h"

/*
 * Klasa BaseDecorator
 */
KernelSem::BaseDecorator::BaseDecorator(PCB* running) : running(running) {}

/*
 * Klasa AlertDecorator
 */
KernelSem::AlertDecorator::AlertDecorator(PCB* running) : BaseDecorator(running) {}

/*
 * Klasa SleepyDecorator
 */
KernelSem::SleepyDecorator::SleepyDecorator(PCB* running, Time timeToWait) : BaseDecorator(running), timeToWait(timeToWait){}
KernelSem::SleepyDecorator::tick() {
	if (--timeToWait == 0)
}

/*
 * Klasa KernelSem
 */


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
		if (maxTimeToWait == 0) {
			running->state = PCB::blocked;
		} else {
			running->state = PCB::sleeping;
			running->timeToWait = maxTimeToWait;

		}

		TRACE(("\nubacujem u blockedOnSemaphore sa id = %d", running->getLocalId()));
		blockedOnSemaphore->add(running);
		Kernel::getInstance().unlock();
		dispatch();
		TRACE(("\nuwait posle dispatch!!!!!!!"));

	} else Kernel::getInstance().unlock();
	TRACE(("\nu wait pre return 1"));
	return 1;
}

void KernelSem::unblock() {
	List<PCB>::Iterator it = blockedOnSemaphore->begin();
	PCB* next = *it;
	blockedOnSemaphore->remove(it);
	next->state = PCB::ready;
	TRACE(("\nUzimam iz blockedOnSemaphore, U unblock(), next ima id = %d", next->getLocalId()));
	Scheduler::put(next);
}

void KernelSem::signal() {
	Kernel::getInstance().lock();
	TRACE(("\npocetak KernelSem::signal(), val = %d", val));
	if (++val <= 0) {
		unblock();
	}
	Kernel::getInstance().unlock();
}

int KernelSem::decrement() {
	//TRACE(("u KernelSem::decrement"));
	Kernel::getInstance().lock();
	syncPrintf("u decreemnt, time = %d\n", time);
	if (--time == 0) {
		unblock();
		Kernel::getInstance().unlock();
		return 1;
	} else {
		Kernel::getInstance().unlock();
		return 0;
	}

}

void interrupt KernelSem::decrementSemaphores() {
	List<KernelSem>::Iterator it = Kernel::getInstance().semaphores->begin();
	for (; it != Kernel::getInstance().semaphores->end(); ++it) {
		syncPrintf("\n\nvrednost time je: %d\n", (*it)->time);
		if ((*it)->decrement() == 1) Kernel::getInstance().semaphores->remove(it);
	}
}
