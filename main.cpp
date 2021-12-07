#include "kernel.h"
#include "debug.h"

int main(int argc, char* argv[]) {
	TRACE(("\nFirst line of main(), argc = %d, argv[0] = %s\n", argc, argv[0]));
	// Kernel::getInstance(argc, argv);
	int ret = Kernel::getInstance(argc, argv).getReturnValue();
	TRACE(("\nLast line of main with ret = %d", ret));
	return ret;
}


// Za testiranje: FOR /L %i IN (1,1,30) DO javni.exe
