#include "kernel.h"
#include "debug.h"

int main(int argc, char* argv[]) {
	TRACE(("\nfirst line of main, argc = %d, argv[0] = %s\n", argc, argv[0]));
	Kernel::getInstance(argc, argv);
	int ret = Kernel::getInstance().getReturnValue();
	TRACE(("\nlast line of main with ret = %d", ret));
	return ret;
}


// Za testiranje: FOR /L %i IN (1,1,30) DO javni.exe
