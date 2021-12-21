#include "PCB.h"

#include <dos.h>

#include "SCHEDULE.H"
#include "debug.h"
#include "kernel.h"

// Inicijalizacija statickih promenljivih

ID PCB::globalId = 0;

// Definicije

PCB::PCB()
    : state(PCB::ready),
      sp(0),
      ss(0),
      bp(0),
      timeSlice(0),
      unlimited(1),
      waitingToComplete(),
      stack(0),
      myThread(0),
      localId(0) {}

PCB::PCB(Thread *myThread, StackSize stackSize, Time timeSlice)
    : state(PCB::initial),
      timeSlice(timeSlice),
      unlimited(timeSlice ? 0 : 1),
      waitingToComplete(),
      stack(new unsigned[stackSize]),
      myThread(myThread),
      localId(++globalId) {
  const StackSize indexOfPSW = stackSize - 1,  // dno steka
      indexOfCS = stackSize - 2, indexOfIP = stackSize - 3,
                  indexOfStackTop = stackSize - 12;  // vrh steka

  stack[indexOfPSW] = 0x200;  // I = 1
  stack[indexOfCS] = FP_SEG(PCB::wrapper);
  stack[indexOfIP] = FP_OFF(PCB::wrapper);

  sp = FP_OFF((stack + indexOfStackTop));
  ss = FP_SEG((stack + indexOfStackTop));
  bp = sp;

  Kernel::getInstance().PCBsById->add(this);
}

void PCB::wrapper() {
  Kernel::getInstance().running->myThread->run();
  TRACE(("\PCB::wrapper(): posle run() metode niti sa id = %d",
         Kernel::getInstance().running->getLocalId()));
  Kernel::getInstance().lock();
  List<PCB>::Iterator it =
      Kernel::getInstance().running->waitingToComplete.begin();
  List<PCB>::Iterator end =
      Kernel::getInstance().running->waitingToComplete.end();
  for (; it != end; ++it) {
    TRACE(
        ("\nPCB::wrapper(): ubacujemo u waitingToComplete listu nit sa id = %d",
         (*it)->getLocalId()));
    (*it)->state = PCB::ready;
    Scheduler::put(*it);
  }
  Kernel::getInstance().running->state = PCB::finished;
  Kernel::getInstance().unlock();
  dispatch();
}

ID PCB::getLocalId() const { return localId; }

PCB::~PCB() {
  if (myThread == 0) return;  // u pitanju je mainPCB
  List<PCB>::Iterator it = Kernel::getInstance().PCBsById->begin(),
                      end = Kernel::getInstance().PCBsById->end();
  for (; it != end && (*it)->localId != localId; ++it)
    ;
  Kernel::getInstance().PCBsById->remove(it);
  delete stack;
}

/* Izgled steka pre povratka iz prekidne rutine:
 * 	bp (SP) (stack[1012]) -- SP ukazuje na poslednju zauzetu i raste nanize
 * 	d2 (SP+1)
 * 	si (SP+2)
 * 	ds (SP+3)
 * 	es (SP+4)
 * 	dx (SP+5)
 * 	cx (SP+6)
 *	bx (SP+7)
 *	ax (SP+8)
 * 	IP (SP+9)
 * 	CS (SP+10)
 * 	PSW (SP+11) (stack[1023])
 */
