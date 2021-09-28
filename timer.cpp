#include "timer.h"
#include <iostream.h>
#include <dos.h>
#include "kernel.h"
#include "PCB.h"
#include "SCHEDULE.H"
#include "debug.h"

// Ove pomocne promenljive se koriste unutar asm instrukcija
// Moraju biti globalne jer se asm instrukcijama invalidira tekuci bp
// A kako ne bi bile vidljive van ovog fajla, oznacene su i kao static
static unsigned tss, tsp, tbp;
static unsigned volatile cnt = 0, unlimited = 1;

pInterrupt timerInit() {
	LOCK
	pInterrupt oldISR = getvect(0x08);
	setvect(0x60, oldISR);
	setvect(0x08, timer);
	UNLOCK
	return oldISR;
}

void timerRestore(pInterrupt oldISR) {
	LOCK
	setvect(0x08, oldISR);
	UNLOCK
}

void interrupt timer(...) {
	tick();
	if (!Kernel::getInstance().context_switch_on_demand && cnt != 0) cnt--;
	if ((cnt == 0 && !unlimited)|| Kernel::getInstance().context_switch_on_demand) {
		if (!Kernel::getInstance().isLocked()) {
			Kernel::getInstance().context_switch_on_demand = 0;
			// Sacuvaj kontekst trenutne niti u njen PCB
			asm {
				mov tss, ss
				mov tsp, sp
				mov tbp, bp
			}

			Kernel::getInstance().running->ss = tss;
			Kernel::getInstance().running->sp = tsp;
			Kernel::getInstance().running->bp = tbp;

			// Trenutnu nit stavi u Scheduler ako je i dalje spremna
			if (Kernel::getInstance().running->state == PCB::ready) {
				Scheduler::put(Kernel::getInstance().running);
			}
			// Izaberi novu nit
			Kernel::getInstance().running = Scheduler::get();

			/* DEBUG */
#if DEBUG
			Kernel::getInstance().lock();
			cout << "\ntimer(): postavljamo running na nit sa id="
					<< Kernel::getInstance().running->getLocalId() << endl;
			Kernel::getInstance().unlock();
			asm cli;
#endif /* end of DEBUG */

			// Azuriraj kontekst prema novoizabranoj niti
			tsp = Kernel::getInstance().running->sp;
			tss = Kernel::getInstance().running->ss;
			tbp = Kernel::getInstance().running->bp;

			cnt = Kernel::getInstance().running->timeSlice;
			unlimited = Kernel::getInstance().running->unlimited;

			asm {
				mov sp, tsp
				mov ss, tss
				mov bp, tbp
			}

		}
	}

	// Poziv izvorne prekidne rutine tajmera
	if(!Kernel::getInstance().dispatched) {
		asm int 60h;
	} else Kernel::getInstance().dispatched = 0;
}

