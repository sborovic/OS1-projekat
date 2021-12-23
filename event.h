#ifndef _event_h_
#define _event_h_

#include "ivtentry.h"
#include "utility.h"
#define PREPAREENTRY(numEntry, callOld)\
	void interrupt inter##numEntry(...);\
	IVTEntry newEntry##numEntry(numEntry, inter##numEntry);\
	void interrupt inter##numEntry(...) {\
		syncPrintf("\nu interrupt rutini!!!!");\
		newEntry##numEntry.signal();\
		if (callOld == 1)\
			newEntry##numEntry.oldISR();\
	}

typedef unsigned char IVTNo;
class KernelEv;
class Event {
public:
    Event(IVTNo ivtNo);
    ~Event();
    void wait();

protected:
    friend class KernelEv;
    void signal(); // can call KernelEv
private:
    KernelEv* myImpl;
};
#endif /* _event_h_ */
