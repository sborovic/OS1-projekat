#include "kernsem.h"

#include "PCB.h"
#include "SCHEDULE.H"
#include "debug.h"
#include "kernel.h"
#include "list.h"

/*
 * Klasa BaseDecorator
 */
KernelSem::BaseDecorator::BaseDecorator(PCB* running)
    : running(running)
{
    TRACE(("\nU Base decorator ubacujem PCB sa id = %d", running->getLocalId()));
}

KernelSem::BaseDecorator::~BaseDecorator()
{
    // TRACE(("\nulaz u dtor BaseDecorator, operisem sa running sa id = %d",
    // running->getLocalId()));

    this->running->state = PCB::ready;
    Scheduler::put(this->running);
    TRACE(("\nizlaz iz dtor BaseDecorator"));
}

/*
 * Klasa AlertDecorator
 */
KernelSem::AlertDecorator::AlertDecorator(PCB* running)
    : BaseDecorator(running)
{
    running->state = PCB::blocked;
    TRACE(("ctor AlertDecorator"));
}
int KernelSem::AlertDecorator::tick() { return 0; }

/*
 * Klasa SleepyDecorator
 */
KernelSem::SleepyDecorator::SleepyDecorator(PCB* running, Time timeToWait,
    int* returnValue)
    : BaseDecorator(running)
    , timeToWait(timeToWait)
    , returnValue(returnValue)
{
    running->state = PCB::sleeping;
    TRACE(("ctor Sleepy DEcorator"));
}
int KernelSem::SleepyDecorator::tick()
{
    TRACE(("\nU SleepyDEc TICKU!!"));
    TRACE(("\nu sldec:tick:timetowait = %d", timeToWait));
    if (--timeToWait == 0) {
        *returnValue = 0;
        return 1;
    } else
        return 0;
}

/*
 * Klasa KernelSem
 */

KernelSem::KernelSem(int init)
{
    val = init;
    // blockedOnSemaphore = new List<BaseDecorator>;
    TRACE(("\nDodaje u semaphores listu this/...."));
    Kernel::getInstance().semaphores->add(this);
}

KernelSem::~KernelSem()
{
    // delete blockedOnSemaphore;
    List<KernelSem>::Iterator it = Kernel::getInstance().semaphores->begin(),
                              end = Kernel::getInstance().semaphores->end();
    for (; it != end && (*it) != this; ++it)
        ;
    Kernel::getInstance().semaphores->remove(it);
}

int KernelSem::wait(Time maxTimeToWait)
{
    Kernel::getInstance().lock();
    TRACE(("\npocetak KernelSem::wait(), val = %d, maxTimeToWait = %d", val,
        maxTimeToWait));
    int returnValue = 1;
    if (--val < 0) {
        PCB* running = Kernel::getInstance().running;
        TRACE(
            ("\nubacujem u blockedOnSemaphore sa id = %d", running->getLocalId()));
        if (maxTimeToWait == 0) {
            blockedOnSemaphore.add(new AlertDecorator(running));
        } else {
            blockedOnSemaphore.add(
                new SleepyDecorator(running, maxTimeToWait, &returnValue));
        }
        Kernel::getInstance().unlock();
        dispatch();
        TRACE(("\nuwait posle dispatch!!!!!!!"));

    } else
        Kernel::getInstance().unlock();
    TRACE(("\nu wait pre return..., returnValue = %d", returnValue));
    return returnValue;
}

void KernelSem::unblock()
{
    TRACE(("\nUzimam iz blockedOnSemaphore...."));
    List<BaseDecorator>::Iterator it = blockedOnSemaphore.begin();
    delete *it;
    blockedOnSemaphore.remove(it);
}

void KernelSem::signal()
{
    Kernel::getInstance().lock();
    TRACE(("\npocetak KernelSem::signal(), val = %d", val));
    if (++val <= 0) {
        unblock();
    }
    Kernel::getInstance().unlock();
}

void KernelSem::tick()
{
    TRACE(("u KernelSem::tick"));
    List<BaseDecorator>::Iterator it = blockedOnSemaphore.begin();
    while (it != blockedOnSemaphore.end()) {
        if ((*it)->tick() == 1) {
            TRACE(("\nUnutar kernsem tick, pre delte it...."));
            delete *it;
            ++val;
            it = blockedOnSemaphore.remove(it);
        } else
            ++it;
    }
}

void KernelSem::tickSemaphores()
{
    //	Kernel::getInstance().lock();
    TRACE(("\nulazim u tickSemaphores"));
    List<KernelSem>::Iterator it = Kernel::getInstance().semaphores->begin();
    for (; it != Kernel::getInstance().semaphores->end(); ++it)
        (*it)->tick();
    TRACE(("\nizlazim iz tickSemaphores"));
    // Kernel::getInstance().unlock();
}
