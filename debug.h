#ifndef _debug_h_
#define _debug_h_

#include "utility.h"

#define DEBUG 0
#define TRACE(x) do { if (DEBUG) syncPrintf x; } while (0)

#endif /* _debug_h_ */
