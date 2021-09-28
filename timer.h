#ifndef _timer_h_
#define _timer_h_

typedef void interrupt (*pInterrupt)(...);

pInterrupt timerInit();
void timerRestore(pInterrupt);
void interrupt timer(...);

void tick();

#endif /* _timer_h_ */
