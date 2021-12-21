#include "tc.h"
TestClass& TestClass::g()
{
    static TestClass t;
    return t;
}
TestClass::TestClass()

{
    new int;
    cout << "\n!!!!ctor Testclass";
}
TestClass::~TestClass() { cout << "\n!!!dtor TestClass"; }
