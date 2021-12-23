#ifndef _kernelev_h_
#define _kernelev_h_

#include "event.h"
class PCB;

class KernelEv {
private:
    friend class Event;
    friend class IVTEntry;
    KernelEv(IVTNo ivtNo);
    ~KernelEv();
    void wait();
    void signal();
    IVTNo ivtNo;
    int val;
    PCB* blocked;
    PCB* constructedBy;
};

#endif /* _kernelev_h_ */
