#ifndef _tc_h_
#define _tc_h_

#include "iostream.h"

#include "thr_mem.h"
class TestClass : ThrMem {
public:
	TestClass();
	int a;
	~TestClass();
	static TestClass& g();
};
#endif
