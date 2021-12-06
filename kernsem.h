#include "semaphor.h"
#include "list.h"
#include "PCB.h"

class KernelSem: public Semaphore {
public:
	virtual void wait();
	virtual void signal();
private:
	void block();
	void unblock();
	// KernelSem je moguce konstruisati samo unutar Semaphore
	KernelSem(int init);
	// Zabranjeno je kopiranje KernelSem
	KernelSem(KernelSem const&);
	KernelSem& operator=(KernelSem const&);
	int val;
	List<PCB>* blocked;

};
