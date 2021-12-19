#include "utility.h"
#include "thread.h"
#include "semaphor.h"
#include <stdlib.h>
#include <setjmp.h>
#include "kernel.h"
#include "PCB.h"
#include <iostream.h>
#include "list.h"
/*
 	 Test 9: Semafori sa spavanjem
*/

const int n = 1;
int t=-1;

Semaphore s(0);

class TestThread : public Thread
{
public:

	TestThread(): Thread(){}
	~TestThread()
	{
		waitToComplete();
	}
protected:

	void run();

};

void TestThread::run()
{
	syncPrintf("Thread waits for 10 units of time...\n");
	t=0;
	s.wait(1);
	syncPrintf("Thread finished.\n");
	s.signal();
}

void tick()
{
	t++;
	if(t)
		syncPrintf("%d\n",t);
}

int userMain(int argc, char** argv)
{
	List<int> test;
	syncPrintf("Test starts.\n");
	TestThread t[n];
	int i;
	for(i=0;i<n;i++)
	{
		t[i].start();
	}
	for(i=0;i<n;i++)
	{
		t[i].waitToComplete();
	}
	syncPrintf("Test ends.\n");
	return 0;
}


