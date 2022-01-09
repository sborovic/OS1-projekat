#include "kernel.h"

int main(int argc, char* argv[])
{
    int ret = Kernel::getInstance(argc, argv).getReturnValue();
    return ret;
}

// Za testiranje: FOR /L %i IN (1,1,30) DO javni.exe
