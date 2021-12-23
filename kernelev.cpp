#include "kernelev.h"

#include "PCB.h"
#include "kernel.h"
#include "SCHEDULE.H"
#include "utility.h"

KernelEv::KernelEv(IVTNo ivtNo) : ivtNo(ivtNo), val(0), blocked(0), constructedBy(Kernel::getInstance().running) {
    IVTEntry::getObject(ivtNo)->setKernelEv(this);
}


void KernelEv::wait()
{
	Kernel::getInstance().lock();
	syncPrintf("\nu kernelev:wait, val = %d", val);
	PCB* running = Kernel::getInstance().running;
	if(running == constructedBy) {
		if (val == 1) {
			val = 0;
			Kernel::getInstance().unlock();
		} else {
			running->state = PCB::blocked;
			blocked = running;
			Kernel::getInstance().unlock();
			dispatch();
		}
	} else Kernel::getInstance().unlock();
}

void KernelEv::signal()
{
	Kernel::getInstance().lock();
	syncPrintf("\nukernelev singal...");
	if (blocked == 0) val = 1;
	else {
		blocked->state = PCB::ready;
		Scheduler::put(blocked);
		blocked = 0;
	}
	Kernel::getInstance().unlock();
}
