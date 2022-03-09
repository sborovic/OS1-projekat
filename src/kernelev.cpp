#include "../h/kernelev.h"

#include <dos.h>
#include "../h/PCB.h"
#include "../h/kernel.h"
#include "../h/SCHEDULE.H"
#include "../h/utility.h"

KernelEv::KernelEv(IVTNo ivtNo) : ivtNo(ivtNo), val(0), blocked(0), constructedBy(Kernel::getInstance().running) {
	IVTEntry* ivtEntry = IVTEntry::getObject(ivtNo);
    ivtEntry->setKernelEv(this);
	ivtEntry->oldISR = getvect(ivtNo);
	setvect(ivtNo, ivtEntry->newISR);
}

KernelEv::~KernelEv() {
	setvect(ivtNo, IVTEntry::getObject(ivtNo)->oldISR);
}


void KernelEv::wait()
{
	Kernel::getInstance().lock();
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
	if (blocked == 0) val = 1;
	else {
		blocked->state = PCB::ready;
		Scheduler::put(blocked);
		blocked = 0;
	}
	Kernel::getInstance().unlock();
}
