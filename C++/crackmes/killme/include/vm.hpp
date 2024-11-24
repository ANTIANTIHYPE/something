#pragma once

#include <windows.h>
#include <iphlpapi.h>
#include <intrin.h>

class VMD {
public:
    bool checkHW();
    bool checkRegedit(); // reddit
    bool checkFiles();
    bool checkCPUI();
    //bool checkTiming();
    bool checkVM();
};
