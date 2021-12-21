#ifndef _kernel_h_
#define _kernel_h_

// Zabranjivanje i dozvoljavanje prekida sa ugnezdjavanjem - na nivou I bita
#define LOCK asm {\
 		pushf;\
 		cli;}
#define UNLOCK asm popf;

#include "list.h"
#include "thread.h"
#include "timer.h"
class PCB;
class KernelSem;

// Klasa Kernel implementirana je kao Unikat
class Kernel {
 private:
  Kernel(int argc, char *argv[]);
  Kernel(Kernel const &);
  void operator=(Kernel const &);
  class IdleThread : public Thread {
   public:
    IdleThread();
    virtual void run();
    virtual ~IdleThread();

   private:
    IdleThread(IdleThread const &);
    IdleThread &operator=(IdleThread const &);
  };
  class UserMainThread : public Thread {
   public:
    UserMainThread(int argc, char **argv, int &ret);
    virtual void run();
    virtual ~UserMainThread();
    int &ret;
    int argc;
    char **argv;

   private:
    UserMainThread(UserMainThread const &);
    UserMainThread &operator=(UserMainThread const &);
  };
  int argc;
  char **argv;
  int ret;
  int volatile locked;
  PCB *mainPCB;  // Ovaj PCB ne ulazi u donju listu, a ima id = 0
 public:
  // Ova lista mora biti konstruisana pre ma kog Threada
  List<PCB> *PCBsById;

 private:
  IdleThread *idleThread;  // Ima id = 1
  Thread *userMainThread;  // Ima id = 2
  pInterrupt oldISR;

 public:
  inline static Kernel &getInstance(int argc = 0, char *argv[] = 0);
  ~Kernel();
  int getReturnValue() const;
  int isLocked() const;
  void lock();
  void unlock();
  void updateRunning();
  PCB *volatile running;  // naveden posle mainPCB
  int volatile context_switch_on_demand;
  int volatile dispatched;
  List<KernelSem> *semaphores;
};

#endif /* _kernel_h_ */
