#ifndef _KERNSEM_H_
#define _KERNSEM_H_

#include "list.h"
#include "semaphor.h"
class PCB;

class KernelSem {
public:
	int wait(Time maxTimeToWait);
	void signal();
	~KernelSem();
	int decrement();
	static void interrupt decrementSemaphores();
private:
	void unblock();
	friend class Semaphore;
	// KernelSem je moguce konstruisati samo unutar Semaphore
	KernelSem(int init);
	// Zabranjeno je kopiranje KernelSem
	KernelSem(KernelSem const&);
	KernelSem& operator=(KernelSem const&);
	int volatile val;
	class BaseDecorator {
	public:
		BaseDecorator(PCB* runnning);
		virtual void tick();
		PCB* running;
	};
	class AlertDecorator {
	public:
		AlertDecorator(PCB* running);
		virtual void tick();
	};
	class SleepyDecorator {
		SleepyDecorator(PCB* running, Time timeToWait);
		virtual void tick();
		Time timeToWait;
	};
	List<PCB>* blockedOnSemaphore;
};

#endif /* _KERNSEM_H_ */
