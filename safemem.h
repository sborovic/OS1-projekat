#ifndef _safemem_h_
#define _safemem_h_

#include <stddef.h>

/* NAPOMENA: Sve klase ciji se objekti prave sa new i delete
 * od trenutka kada je moguca promena konteksta
 * treba da budu izvedene iz ove klase
 * To su klase: List<T>, Node<T>, PCB
 */

class SafeMemory {
public:
	static void* operator new(size_t size);
	static void operator delete(void* ptr);
};

#endif
