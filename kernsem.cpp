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

KernelSem::BaseDecorator::~BaseDecorator() {
	running->state = PCB::ready;
	Scheduler::put(running);
}

/*
 * Klasa AlertDecorator
 */
KernelSem::AlertDecorator::AlertDecorator(PCB* running) : BaseDecorator(running) {
	running->state = PCB::blocked;
}
int KernelSem::AlertDecorator::tick() {
	return 0;
}

/*
 * Klasa SleepyDecorator
 */
KernelSem::SleepyDecorator::SleepyDecorator(PCB* running, Time timeToWait, int* returnValue)
: BaseDecorator(running), timeToWait(timeToWait), returnValue(returnValue) {
	running->state = PCB::sleeping;
}
int KernelSem::SleepyDecorator::tick() {
	if (--timeToWait == 0) {
		*returnValue = 0;
		return 1;
	}
	else return 0;
}

/*
 * Klasa KernelSem
 */


KernelSem::KernelSem(int init) {
	val = init;
	blockedOnSemaphore = new List<BaseDecorator>;
}

KernelSem::~KernelSem() {
	delete blockedOnSemaphore;
}

int KernelSem::wait(Time maxTimeToWait) {
	Kernel::getInstance().lock();
	TRACE(("\npocetak KernelSem::wait(), val = %d", val));
	int returnValue = 1;
	if (--val < 0) {
		PCB* running = Kernel::getInstance().running;
 		TRACE(("\nubacujem u blockedOnSemaphore sa id = %d", running->getLocalId()));
		if (maxTimeToWait == 0) {
			blockedOnSemaphore->add(new AlertDecorator(running));
		} else {
			blockedOnSemaphore->add(new SleepyDecorator(running, maxTimeToWait, &returnValue));

		}
		Kernel::getInstance().unlock();
		dispatch();
		TRACE(("\nuwait posle dispatch!!!!!!!"));

	} else Kernel::getInstance().unlock();
	TRACE(("\nu wait pre return..., returnValue = %d", returnValue));
	return returnValue;
}

void KernelSem::unblock() {
	TRACE(("\nUzimam iz blockedOnSemaphore...."));
	List<BaseDecorator>::Iterator it = blockedOnSemaphore->begin();
	delete *it;
	blockedOnSemaphore->remove(it);
}

void KernelSem::signal() {
	Kernel::getInstance().lock();
	TRACE(("\npocetak KernelSem::signal(), val = %d", val));
	if (++val <= 0) {
		unblock();
	}
	Kernel::getInstance().unlock();
}

void KernelSem::tick() {
	//TRACE(("u KernelSem::decrement"));
	List<BaseDecorator>::Iterator it = blockedOnSemaphore->begin();
	for (; it != blockedOnSemaphore->end(); ++it){
		if ((*it)->tick() == 1) {
			delete *it;
			it = blockedOnSemaphore->remove(it);
		}
	}
}

void KernelSem::tickSemaphores() {
	List<KernelSem>::Iterator it = Kernel::getInstance().semaphores->begin();
	for (; it != Kernel::getInstance().semaphores->end(); ++it) (*it)->tick();
}
