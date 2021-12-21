#ifndef _kernelev_h_
#define _kernelev_h_

#include "event.h"
#include "semaphor.h"

class KernelEv {
private:
    friend class Event;
    KernelEv(IVTNo ivtNo);
    ~KernelEv();
    void wait();
    void signal(); // can call KernelEv
};

#endif /* _kernelev_h_ */
