#ifndef _pcb_h_
#define _pcb_h_

#include "list.h"
#include "safemem.h"
#include "thread.h"

class PCB : public SafeMemory {
public:
    PCB(); // Poziva se iz klase Kernel (da bi se konstruisao mainPCB)
    ~PCB();
    ID getLocalId() const;
    static void wrapper();
    enum State { initial,
        ready,
        blocked,
        finished,
        sleeping };
    State state;
    unsigned sp, ss, bp;
    Time timeSlice;
    int unlimited;
    List<PCB> waitingToComplete;

private:
    // Navedeni privatni konstruktori bice dostupni samo klasi Thread
    friend class Thread;
    PCB(Thread* myThread, StackSize stackSize, Time timeSlice);
    PCB(PCB const&);
    PCB& operator=(PCB const&);
    unsigned* stack;
    Thread* myThread;
    static ID globalId;
    ID localId;
};

#endif /* _pcb_h_ */
