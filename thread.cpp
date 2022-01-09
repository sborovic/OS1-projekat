#include "thread.h"

#include "PCB.h"
#include "SCHEDULE.H"
#include "debug.h"
#include "kernel.h"

Thread::Thread(StackSize stackSize, Time timeSlice)
{
    myPCB = new PCB(this, stackSize / sizeof(unsigned), timeSlice);
}

Thread::~Thread() { delete myPCB; }

void Thread::start()
{
    if (myPCB->state != PCB::initial)
        return;
    myPCB->state = PCB::ready;
    Scheduler::put(myPCB);
}

PCB* Thread::getPCB() { return myPCB; }

Thread* Thread::getThreadById(ID id)
{
    List<PCB>::Iterator it = Kernel::getInstance().PCBsById->begin(),
                        end = Kernel::getInstance().PCBsById->end();
    for (; it != end && (*it)->localId != id; ++it)
        ;
    if (it == end)
        TRACE(("\ngetThreadById ERROR!"));
    return (*it)->myThread;
}

void Thread::waitToComplete()
{
    if (myPCB->state == PCB::finished) {
        return;
    }
    TRACE(("\nU Thread::waitToComplete()"));
    Kernel::getInstance().lock();
    PCB* running = Kernel::getInstance().running;
    running->state = PCB::blocked;
    myPCB->waitingToComplete.add(running);
    Kernel::getInstance().unlock();
    TRACE(("\nThread::waitToComplete(): id = %d, a u wtc ubacena nit sa id %d",
        getId(), running->getLocalId()));
    dispatch();
}

ID Thread::getId() { return myPCB->getLocalId(); }

ID Thread::getRunningId()
{
    Kernel::getInstance().lock();
    ID id = Kernel::getInstance().running->getLocalId();
    Kernel::getInstance().unlock();
    return id;
}

void dispatch()
{
    Kernel::getInstance().lock();
    TRACE(("\ndispatch(): posle lock()"));
    Kernel::getInstance().context_switch_on_demand = 1;
    Kernel::getInstance().dispatched = 1;
    TRACE(("\ndispatch(): posle unlock()"));
    Kernel::getInstance().unlock();
    timer();
}

// Realizacija racvanja niti
ID Thread::fork() {

	PCB* running = Kernel::getInstance().running;
	Thread* childThread = running->myThread->clone();
	PCB* childPCB = childThread->getPCB();
	// copyStack();
	// izmena list bpova



}

void Thread::exit() {

}
void Thread::waitForChildren() {

}

Thread* Thread::clone() const {
	return 0;
}
