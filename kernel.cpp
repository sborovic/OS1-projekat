#include "kernel.h"
#include "timer.h"
#include "PCB.h"
#include "list.h"
#include "debug.h"
#include "SCHEDULE.H"

int userMain(int, char*[]);
/*
 * Klasa Kernel
 */
Kernel::Kernel(int argc, char* argv[]) :
		argc(argc),
		argv(argv),
		ret(0),
		locked(0),
		mainPCB(new PCB()),
		PCBsById(new List<PCB>),
		idleThread(new IdleThread()),
		userMainThread(new UserMainThread(argc, argv, ret)),
		context_switch_on_demand(0),
		running(mainPCB),
		dispatched(0) {
	userMainThread->start();
	oldISR = timerInit();
}

Kernel::~Kernel() {
	delete userMainThread;
	delete idleThread;
	delete PCBsById;
	delete mainPCB;
}

Kernel& Kernel::getInstance(int argc, char* argv[]) {
	static Kernel instance(argc, argv);
	return instance;
}

Kernel::IdleThread* Kernel::getIdleThread() const {
	return idleThread;
}

PCB* Kernel::getMainPCB() const {
	return mainPCB;
}

Thread* Kernel::getUserMainThread() const {
	return userMainThread;
}

int Kernel::getReturnValue() const {
	TRACE(("\nKernel::getReturnValue()"));
	userMainThread->waitToComplete();
	timerRestore(oldISR);
	return ret;
}

void Kernel::lock() {
	++locked;
}
void Kernel::unlock() {
	--locked;
}

int Kernel::isLocked() const {
	return locked != 0;
}

void Kernel::nextRunning() {
	PCB* next = Scheduler::get();
	if (next == 0) next = idleThread->getPCB();
	running = next;
}

/*
 * Klasa IdleThread
 */
Kernel::IdleThread::IdleThread() : Thread(4096, 1) {}

Kernel::IdleThread::~IdleThread() {}

void Kernel::IdleThread::run() {
	while(1) {
		//TRACE(("\nUNUTAR IDLE THREAD!!!!!!!!!!"));
	}
}

/*
 * Klasa UserMainThread
 */
Kernel::UserMainThread::UserMainThread(int argc, char** argv, int& ret)
	: Thread(), argc(argc), argv(argv), ret(ret) {
	TRACE(("\nKernel::UserMainThread()"));
}
Kernel::UserMainThread::~UserMainThread() {
	waitToComplete();
}

void Kernel::UserMainThread::run() {
	ret = userMain(argc, argv);
}
