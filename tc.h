#ifndef _tc_h_
#define _tc_h_

#include "iostream.h"
#include "safemem.h"
class TestClass : SafeMemory {
public:
    TestClass();
    int a;
    ~TestClass();
    static TestClass& g();
};
#endif
