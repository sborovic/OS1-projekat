#include "../h/event.h"
#include "../h/kernelev.h"

Event::Event(IVTNo ivtNo)
{
    myImpl = new KernelEv(ivtNo);
}

Event::~Event()
{
    delete myImpl;
}

void Event::wait()
{
    myImpl->wait();
}

void Event::signal()
{
    myImpl->signal();
}
