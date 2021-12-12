#ifndef _pcb_h_
#define _pcb_h_

#include "thread.h"
#include "list.h"
#include "thr_mem.h"

class PCB : public ThrMem {
public:
	void print();
	PCB();
	~PCB();
	ID getLocalId() const;
	static void wrapper();
	enum State {
		initial, ready, blocked, finished
	};
	State state;
	unsigned sp, ss, bp;
	Time timeSlice;
	int unlimited;
	List<PCB> waitingToComplete;
private:
	PCB(PCB const&);
	PCB& operator=(PCB const&);
	// Navedeni privatni konstruktor bice dostupan samo klasi Thread
	friend class Thread;
	PCB(Thread* myThread, StackSize stackSize, Time timeSlice);
	unsigned* stack;
	Thread* myThread;
	static ID globalId;
	ID localId;
};

#endif /* _pcb_h_ */
