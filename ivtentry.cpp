#include "ivtentry.h"
#include "kernelev.h"
#include <dos.h>

IVTEntry* IVTEntry::registry[256];

IVTEntry::IVTEntry(IVTNo ivtNo, pInterrupt newISR)
: ivtNo(ivtNo), newISR(newISR), myKernelEv(0)
{

	registry[ivtNo] = this;
}

IVTEntry::~IVTEntry() {
	//setvect(ivtNo, oldISR);
}

IVTEntry* IVTEntry::getObject(IVTNo ivtNo) {
	return registry[ivtNo];
}

void IVTEntry::signal() {
	if(myKernelEv != 0) myKernelEv->signal();
}

void IVTEntry::setKernelEv(KernelEv* myKernelEv) {
	this->myKernelEv = myKernelEv;
}
