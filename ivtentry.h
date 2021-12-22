#ifndef _ivtentry_h_
#define _ivtentry_h_


typedef unsigned char IVTNo;
typedef void interrupt (*pInterrupt)(...);
class KernelEv;

class IVTEntry {
public:
	IVTEntry(IVTNo ivtNo, pInterrupt newISR);
	~IVTEntry();
	void signal();
	void setKernelEv(KernelEv* myKernelEv);
	static IVTEntry* getObject(IVTNo ivtNo);
private:
	IVTEntry(IVTEntry const&);
	IVTEntry& operator=(IVTEntry const&);
	IVTNo ivtNo;
	KernelEv* myKernelEv;
	static IVTEntry* registry[256];
public:
	pInterrupt oldISR;

};

#endif /* _ivtentry_h_ */
