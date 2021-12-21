#ifndef _pcb_h_
#define _pcb_h_

#include "list.h"
#include "safemem.h"
#include "thread.h"

class PCB : public SafeMemory {
 public:
  void print();
  PCB();
  ~PCB();
  ID getLocalId() const;
  static void wrapper();
  enum State { initial, ready, blocked, finished, sleeping };
  State state;
  unsigned sp, ss, bp;
  Time timeSlice;
  int unlimited;
  List<PCB> waitingToComplete;
  Time timeToWait;

 private:
  PCB(PCB const &);
  PCB &operator=(PCB const &);
  // Navedeni privatni konstruktor bice dostupan samo klasi Thread
  friend class Thread;
  PCB(Thread *myThread, StackSize stackSize, Time timeSlice);
  unsigned *stack;
  Thread *myThread;
  static ID globalId;
  ID localId;
};

#endif /* _pcb_h_ */
