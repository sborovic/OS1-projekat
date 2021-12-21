#ifndef _kernsem_h_
#define _kernsem_h_

#include "list.h"
#include "semaphor.h"
class PCB;

class KernelSem {
 public:
  static void tickSemaphores(); // poziva se unutar timer(...) prekida
 private:
  int wait(Time maxTimeToWait);
  void signal();
  ~KernelSem();
  void tick();

  void unblock();
  friend class Semaphore;
  // KernelSem je moguce konstruisati samo unutar Semaphore
  KernelSem(int init);
  KernelSem(KernelSem const &);
  KernelSem &operator=(KernelSem const &);
  int volatile val;
  class BaseDecorator {
   public:
    BaseDecorator(PCB *runnning);
    virtual ~BaseDecorator();
    virtual int tick() = 0;
    PCB *running;
  };
  class AlertDecorator : public BaseDecorator {
   public:
    AlertDecorator(PCB *running);
    virtual int tick();
  };
  class SleepyDecorator : public BaseDecorator {
  public:
    SleepyDecorator(PCB *running, Time timeToWait, int *returnValue);
    virtual int tick();
    Time timeToWait;
    int *returnValue;
  };
  List<BaseDecorator> blockedOnSemaphore;
};

#endif /* _kernsem_h_ */
