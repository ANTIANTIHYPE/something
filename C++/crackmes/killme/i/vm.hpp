#ifndef VM_HPP
#define VM_HPP

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

#endif // VM_HPP