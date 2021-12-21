#include "utility.h"
#include "kernel.h"
#include <DOS.H>
#include <STDARG.H>
#include <STDIO.H>

int syncPrintf(const char *format, ...) {
  int res;
  va_list args;
  Kernel::getInstance().lock();
  va_start(args, format);
  res = vprintf(format, args);
  va_end(args);
  Kernel::getInstance().unlock();
  return res;
}
