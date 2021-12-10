#ifndef _KERNSEM_H_
#define _KERNSEM_H_

#include "semaphor.h"
#include "list.h"
class PCB;

class KernelSem: public Semaphore {
public:
	virtual int wait(Time maxTimeToWait);
	virtual void signal();
	~KernelSem();
private:
	void block();
	void unblock();
	friend class Semaphore;
	// KernelSem je moguce konstruisati samo unutar Semaphore
	KernelSem(int init);
	// Zabranjeno je kopiranje KernelSem
	KernelSem(KernelSem const&);
	KernelSem& operator=(KernelSem const&);
	int val;
	List<PCB>* blockedOnSemaphore;
};

#endif /* _KERNSEM_H_ */
