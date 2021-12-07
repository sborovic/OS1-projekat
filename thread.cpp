#include "thread.h"
#include "SCHEDULE.H"
#include "kernel.h"
#include "PCB.h"
#include "debug.h"

// Definicije

Thread::Thread(StackSize stackSize, Time timeSlice) {
	myPCB = new PCB(this, stackSize/sizeof(unsigned), timeSlice);
}

Thread::~Thread() {
	delete myPCB;
}

void Thread::start() {
	if (myPCB->state != PCB::initial) return;
	myPCB->state = PCB::ready;
	Scheduler::put(myPCB);
}

PCB* Thread::getPCB() {
	return myPCB;
}

Thread * Thread::getThreadById(ID id) {
	List<PCB>::Iterator it = Kernel::getInstance().PCBsById->begin(),
			end = Kernel::getInstance().PCBsById->end();
	for (; it != end && (*it)->localId != id; ++it);
	if (it == end) TRACE(("\ngetThreadById ERROR!"));
	return (*it)->myThread;
}

void Thread::waitToComplete() {
	if (myPCB->state == PCB::finished)  {
		return;
	}
	TRACE(("\nU Thread::waitToComplete()"));
	LOCK
	PCB* running = Kernel::getInstance().running;
	running->state = PCB::blocked;
	myPCB->waitingToComplete.add(running);
	TRACE(("\nThread::waitToComplete(): id = %d, a u wtc ubacena nit sa id %d", getId(), running->getLocalId()));
	UNLOCK
	dispatch();

}

ID Thread::getId() {
	return myPCB->getLocalId();
}

ID Thread::getRunningId() {
	LOCK
	ID id = Kernel::getInstance().running->getLocalId();
	UNLOCK
	return id;
}

void dispatch() {
	Kernel::getInstance().lock();
	TRACE(("\ndispatch(): posle lock()"));
	Kernel::getInstance().context_switch_on_demand = 1;
	Kernel::getInstance().dispatched = 1;
	TRACE(("\ndispatch(): posle unlock()"));
	Kernel::getInstance().unlock();
	timer();
}
