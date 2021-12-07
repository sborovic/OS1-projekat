#include "semaphor.h"
#include "list.h"
#include "PCB.h"

class KernelSem: public Semaphore {
public:
	virtual int wait(Time maxTimeToWait);
	virtual void signal();
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
	List<PCB>* blocked;

};
