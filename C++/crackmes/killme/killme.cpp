/*
       ██ ▄█▀ ██▓ ██▓     ██▓     ███▄ ▄███▓▓█████
       ██▄█▒ ▓██▒▓██▒    ▓██▒    ▓██▒▀█▀ ██▒▓█   ▀
      ▓███▄░ ▒██▒▒██░    ▒██░    ▓██    ▓██░▒███
      ▓██ █▄ ░██░▒██░    ▒██░    ▒██    ▒██ ▒▓█  ▄
      ▒██▒ █▄░██░░██████▒░██████▒▒██▒   ░██▒░▒████▒
      ▒ ▒▒ ▓▒░▓  ░ ▒░▓  ░░ ▒░▓  ░░ ▒░   ░  ░░░ ▒░ ░
      ░ ░▒ ▒░ ▒ ░░ ░ ▒  ░░ ░ ▒  ░░  ░      ░ ░ ░  ░
      ░ ░░ ░  ▒ ░  ░ ░     ░ ░   ░      ░      ░
      ░  ░    ░      ░  ░    ░  ░       ░      ░  ░

                      No idea why.

      /* +-------------------------------------+ */
      /* |           g++ Flags Used            | */
      /* +-------------------------------------+ */
      /* | -std=c++23                          | */
      /* | -static                             | */
      /* | -fno-tree-loop-distribute-patterns  | */
      /* | -fno-loop-block                     | */
      /* | -fno-cse-follow-jumps               | */
      /* | -fno-combine-stack-adjustments      | */
      /* | -fno-early-inlining                 | */
      /* | -fno-cse-skip-blocks                | */
      /* | -fexpensive-optimizations           | */
      /* | -fno-hoist-adjacent-loads           | */
      /* | -fno-if-conversion                  | */
      /* | -fno-if-conversion2                 | */
      /* | -fno-loop-interchange               | */
      /* | -fno-loop-unroll-and-jam            | */
      /* | -fno-move-loop-invariants           | */
      /* | -fno-tree-dse                       | */
      /* | -fno-tree-forwprop                  | */
      /* | -fno-tree-coalesce-vars             | */
      /* | -fno-tree-copy-prop                 | */
      /* | -fno-sched-stalled-insns-dep        | */
      /* | -fno-split-wide-types               | */
      /* | -fno-tree-dominator-opts            | */
      /* | -fno-tree-cselim                    | */
      /* | -fno-section-anchors                | */
      /* | -fno-tree-ch                        | */
      /* | -fno-tree-ccp                       | */
      /* | -fno-tree-bit-ccp                   | */
      /* | -finline-functions                  | */
      /* | -fno-merge-constants                | */
      /* | -fwrapv                             | */
      /* | -ftrapv                             | */
      /* | -g0                                 | */
      /* | -fno-builtin                        | */
      /* | -fomit-frame-pointer                | */
      /* | -fno-tree-pta                       | */
      /* | -fno-tree-pre                       | */
      /* | -fno-tree-switch-conversion         | */
      /* | -fno-tree-tail-merge                | */
      /* | -fno-tree-phiprop                   | */
      /* | -fno-tree-sink                      | */
      /* | -fno-tree-fre                       | */
      /* | -fno-tree-dce                       | */
      /* | -fno-dce                            | */
      /* | -fno-split-ivs-in-unroller          | */
      /* | -fno-thread-jumps                   | */
      /* | -fno-shrink-wrap                    | */
      /* | -fno-sched-spec                     | */
      /* | -fno-sched-stalled-insns            | */
      /* | -fno-reorder-functions              | */
      /* | -fno-rerun-cse-after-loop           | */
      /* | -fno-ipa-icf                        | */
      /* | -fno-reorder-blocks-and-partition   | */
      /* | -fno-ipa-ra                         | */
      /* | -fpeel-loops                        | */
      /* | -fno-regmove                        | */
      /* | -fno-peel-loops                     | */
      /* | -fno-gcse                           | */
      /* | -fno-gcse-lm                        | */
      /* | -fno-modulo-sched                   | */
      /* | -fno-math-errno                     | */
      /* | -fno-ipa-sra                        | */
      /* | -flto                               | */
      /* | -fno-lifetime-dse                   | */
      /* | -fno-ipa-cp                         | */
      /* | -fno-cprop-registers                | */
      /* | -fno-combine-stack-adjustments      | */
      /* | -falign-functions                   | */
      /* | -falign-jumps                       | */
      /* | -fno-peephole                       | */
      /* | -fno-peephole2                      | */
      /* | -fno-reorder-blocks                 | */
      /* | -fno-reorder-functions              | */
      /* | -fno-dwarf2-cfi-asm                 | */
      /* | -fno-schedule-insns                 | */
      /* | -fno-schedule-insns2                | */
      /* | -fno-data-sections                  | */
      /* | -fno-delete-null-pointer-checks     | */
      /* | -fno-strict-aliasing                | */
      /* | -fno-strict-overflow                | */
      /* | -fno-tree-vrp                       | */
      /* | -O1                                 | */
      /* | -Wall                               | */
      /* | -fno-ident                          | */
      /* | -fno-debug-types-section            | */
      /* | -fno-merge-all-constants            | */
      /* | -frtti                              | */
      /* | -D_FORTIFY_SOURCE=3                 | */
      /* | -fstack-protector                   | */
      /* | -feliminate-unused-debug-types      | */
      /* | -feliminate-unused-debug-symbols    | */
      /* | -finline                            |*/
      /* | -finline-functions-called-once      | */
      /* +-------------------------------------+ */

#include <iostream>
#include <string>
#include <memory>
#include <fstream>
#include <limits>
#include <unordered_set>
#include <vector>
#include <chrono>
#include <cctype>
#include <source_location>
void s()
{
    std::source_location loc = std::source_location::current();
    std::cout << "Error at " << loc.file_name() << ":" << loc.line() << std::endl;
}
void cs()
{
    std::source_location loc = std::source_location::current();
    std::cout << "Error at " << loc.file_name() << ":" << loc.line() << std::endl;
}
void sfd()
{
    std::source_location loc = std::source_location::current();
    std::cout << "Error at " << loc.file_name() << ":" << loc.line() << std::endl;
}
void we()
{
    std::source_location loc = std::source_location::current();
    std::cout << "Error at " << loc.file_name() << ":" << loc.line() << std::endl;
}
void qwde()
{
    std::source_location loc = std::source_location::current();
    std::cout << "Error at " << loc.file_name() << ":" << loc.line() << std::endl;
}
void DCE()
{
    const std::string _ = "Hello";
    std::source_location loc = std::source_location::current();
    std::cout << "Error at " << loc.file_name() << ":" << loc.line() << std::endl;
}
/// bro what
#include <c.hpp>
#include <throw.h>
#ifdef _WIN32
#include <vm.hpp>
extern "C" {
    //    included in vm.hpp
    //    vvvvvvvvvvvvvvvvvvvv
    // 
    //    #include <windows.h>
    //    #include <iphlpapi.h>
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")
} //hardcode the link at this point
#include <tlhelp32.h>
#else
exit(-1);
#endif

///////////// <summary>
/// init vm check start
//////////// </summary>

bool VMD::checkHW()
{
    ULONG out = sizeof(IP_ADAPTER_INFO);
    PIP_ADAPTER_INFO pAI = (IP_ADAPTER_INFO*)malloc(out);
    GetAdaptersInfo(pAI, &out);

    if (strstr(pAI->AdapterName, "VMware") || strstr(pAI->AdapterName, "VirtualBox") /*|| strstr(pAI->AdapterName, "Xen")*/)
    {
        free(pAI);
        return true;
    }

    free(pAI);
    return false;
}

bool VMD::checkRegedit()
{
    HKEY hKey;
    LONG res = RegOpenKeyEx(HKEY_LOCAL_MACHINE, (LPCSTR)"SOFTWARE\\VMware, Inc.\\VMware Tools", 0, KEY_QUERY_VALUE, &hKey);
    if (res == ERROR_SUCCESS)
    {
        RegCloseKey(hKey);
        return true;
    }
    return false;
}

bool VMD::checkFiles()
{
    WIN32_FIND_DATA findData;
    HANDLE hFind = FindFirstFile((LPCSTR)"C:\\Windows\\System32\\vmwareuser.exe", &findData);
    if (hFind != INVALID_HANDLE_VALUE)
    {
        FindClose(hFind);
        return true;
    }
    return false;
}

bool VMD::checkCPUI()
{
    int cpuInfo[4];
    __cpuid(cpuInfo, 0);
    if ((cpuInfo[1] & 0x00000001) == 0)
    {
        return true;
    }
    return false;
}

/////////// <summary>
/// init vm check end
////////// </summary>

bool VMD::checkVM()
{
    return checkHW() || checkRegedit() || checkFiles() || checkCPUI();
}

#include <dbgeng.h>
///
using namespace std;
///
#include <regex>
#include <algorithm>
bool eval_cond(bool cond, bool exit)
{
    return (cond & ~exit) | (!cond & exit);
}
//#define _ISDBGPRESENTPTR ((BOOL (WINAPI *)(VOID))0x7C8094A6) // yeah we are not using that
void spec()
{
    asm volatile("int $0x80" : : "a" (0x01));
}
typedef int _INT;

/// 
/// ////////////////////// ////////////////////// ////////////////////// ////////////////////// ////////////////////// ////////////////////// ///////////////////
/// ////////////////////// ////////////////////// ////////////////////// ////////////////////// ////////////////////// ////////////////////// ///////////////////
/// ////////////////////// ////////////////////// ////////////////////// ////////////////////// ////////////////////// ////////////////////// ///////////////////
/// ////////////////////// ////////////////////// ////////////////////// ////////////////////// ////////////////////// ////////////////////// ///////////////////
/// 

typedef BOOL(WINAPI* LPFN_ispre)(VOID);
LPFN_ispre fn = IsDebuggerPresent;

struct cualloc
{
    using value_type = bool;
    using pointer = bool*;
    using const_pointer = const bool*;

    pointer alloc(size_t n)
    {
        uint32_t rnd = rand();
        return new bool(!!fn() ^ (rnd & 0x1));
    }

    void dealloc(pointer p, size_t n)
    {
        delete p;
    }
};
cualloc ok;
cualloc::pointer pp1 = ok.alloc(0x1);

auto trans = [](bool b)
{
    char c = 'o';
    uint32_t key = _mm_crc32_u32(0, static_cast<uint32_t>(std::tolower(c)));
    return (b ? 'T' : 'F') ^ (key & 0xFF);
};
bool** pp2 = &pp1;

typedef union
{
    int dyn;
} Uniond;

bool* ptar = *pp2;

//#define no ((trans(*(ptar))) ^ (_rotl(GetTickCount(), 3) & 0x1))
void cls()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = { 0, 0 }; // top-left corner
    DWORD written;
    FillConsoleOutputCharacter(hConsole, ' ', 80 * 25000, pos, &written); // 25 -> 25000 (looks strange, but no care)
    FillConsoleOutputAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE, 80 * 25, pos, &written);
    SetConsoleCursorPosition(hConsole, pos);
}
#include <format>
/*
    c.hpp guide:


    Color::Code cc = Color::COLOR;
    std::string colored = Color::colorize("text", cc);
    cout << colored << endl;
*/
#include <locale>
#include <codecvt>
bool isthisrealchat()
{
    HKEY hKey;
    LONG lResult;
    DWORD dwIndex = 0;
    DWORD dwVT;
    DWORD dwVS = sizeof(WCHAR) * 1024;
    WCHAR szVN[1024];
    WCHAR szVD[1024];

    lResult = RegOpenKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\Hex-Rays\\IDA\\History64", 0, KEY_READ, &hKey);
    if (lResult == ERROR_SUCCESS)
    {
        while (RegEnumValueW(hKey, dwIndex, szVN, &dwVS, NULL, &dwVT, NULL, NULL) == ERROR_SUCCESS)
        {
            ++dwIndex;
            dwVS = sizeof(szVD);
            lResult = RegQueryValueExW(hKey, szVN, NULL, &dwVT, (LPBYTE)szVD, &dwVS);
            if (lResult == ERROR_SUCCESS)
            {
                if (dwVT == REG_SZ)
                {
                    wstring filePath(szVD);
                    WCHAR szCurrentExePath[MAX_PATH];
                    GetModuleFileNameW(NULL, szCurrentExePath, MAX_PATH);
                    if (!(filePath != szCurrentExePath))
                    {
                        RegCloseKey(hKey);
                        //cout << "Please clear your IDA History" << endl;
                        MessageBox(NULL, (LPCSTR)"Please clear your IDA History", (LPCSTR)"Ahem...", MB_ICONWARNING);
                        exit(0x1);
                        //return true;
                    }
                }
            }
        }
        RegCloseKey(hKey);
    }

    HWND hwnd = FindWindow((LPCSTR)"IDAView", NULL); // | FindWindow("IDAViewA", NULL); //idk
    if (hwnd != NULL)
    {
        //printf("Chat is this real?\n");
        return true;
    }

    HANDLE hModuleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, 0);
    if (hModuleSnapshot != INVALID_HANDLE_VALUE)
    {
        MODULEENTRY32 me;
        me.dwSize = sizeof(MODULEENTRY32);
        if (Module32First(hModuleSnapshot, &me))
        {
            do
            {
                wstring_convert<codecvt_utf8<wchar_t>> converter;
                wstring m = converter.from_bytes((CHAR*)me.szModule);
                string mama = converter.to_bytes(m);

                Color::Code avs = Color::BLUE;
                string hold = Color::colorize(format("Checking module: {}", mama), avs);
                //Color::Code cc = Color::COLOR;
                //std::string colored = Color::colorize("text", cc);
                //cout << colored << endl;
                //printf(hold.c_str());
                cout << hold << endl;

                if (strcmp((CHAR*)me.szModule, "ida.wll") == 0 || 
                    strcmp((CHAR*)me.szModule, "ida.dll") == 0 || 
                    strcmp((CHAR*)me.szModule, "idalib64.dll") == 0 || 
                    strcmp((CHAR*)me.szModule, "ida64.dll") == 0 || 
                    strcmp((CHAR*)me.szModule, "idaw.exe") == 0 || 
                    strcmp((CHAR*)me.szModule, "idat.exe") == 0 || 
                    strcmp((CHAR*)me.szModule, "ida64.exe") == 0 || 
                    strcmp((CHAR*)me.szModule, "ida.exe") == 0 || 
                    strcmp((CHAR*)me.szModule, "SystemInformer.exe") == 0)
                    {
                    
                    Color::Code coc = Color::RED;
                    string t = "Nothing found!";
                    string tc = Color::colorize(t, coc, true);
                    //printf(tc.c_str());
                    cout << tc << endl;

                    /* screw this
                    *  vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
                    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                    COORD pos = { 0, 0 }; // top-left corner
                    DWORD written;
                    FillConsoleOutputCharacter(hConsole, ' ', 80 * 25, pos, &written);
                    FillConsoleOutputAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE, 80 * 25, pos, &written);
                    SetConsoleCursorPosition(hConsole, pos);*/

                    //system("cls"); // sowwy...
                    return true;
                }
            }
            while (Module32Next(hModuleSnapshot, &me));
            cls();
        }
        CloseHandle(hModuleSnapshot);
    }

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (hSnapshot != INVALID_HANDLE_VALUE)
        {
            PROCESSENTRY32 pe;
            pe.dwSize = sizeof(PROCESSENTRY32);

            if (Process32First(hSnapshot, &pe))
            {
                do
                {
                    if (strcmp((CHAR*)pe.szExeFile, "idaw.exe") == 0 || 
                        strcmp((CHAR*)pe.szExeFile, "Microsoft.PythonTools.AttacherX86.exe") == 0 || 
                        strcmp((CHAR*)pe.szExeFile, "hvui.exe") == 0 || 
                        strcmp((CHAR*)pe.szExeFile, "idat64.exe") == 0 || 
                        strcmp((CHAR*)pe.szExeFile, "ida64.exe") == 0 || 
                        strcmp((CHAR*)pe.szExeFile, "ida32.exe") == 0 || 
                        strcmp((CHAR*)pe.szExeFile, "SystemInformer.exe") == 0)
                        {

                        printf("Nothing found!\n");

                        return true;
                    }
                }
                while (Process32Next(hSnapshot, &pe));
            }
        CloseHandle(hSnapshot);
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// 
// 
// Additional
bool ispre()
{
    // cout << __LINE__ << endl;
    CONTEXT ctx;
    ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;
    if (GetThreadContext(GetCurrentThread(), &ctx))
    {
        return ctx.Dr0 != 0 || ctx.Dr1 != 0 || ctx.Dr2 != 0 || ctx.Dr3 != 0;
    }
    return false;
}

bool iddbgapic(DWORD eip)
{
    static const DWORD apics[] = {
        (DWORD)(uintptr_t)DebugBreak,
        (DWORD)(uintptr_t)OutputDebugStringA,
        (DWORD)(uintptr_t)WaitForDebugEvent,
        (DWORD)(uintptr_t)GetThreadContext,
        (DWORD)(uintptr_t)SetThreadContext,
        (DWORD)(uintptr_t)SuspendThread,
        (DWORD)(uintptr_t)ResumeThread
    };

    for (size_t i = 0; i < sizeof(apics) / sizeof(DWORD); ++i)
    {
        if (eip == apics[i])
        {
            return true;
        }
    }
    return false;
}

bool strangemem(DWORD eip, DWORD rbx, DWORD rcx, DWORD rdx)
{
    static const std::array<DWORD, 5> patterns = {
        0x00401000, // reading from a specific memory region
        0x00402000, // writing to a specific memory region
        0x00503000, // executing from a specific memory region
        0x00604000, // accessing a specific memory region
        0x00705000  // modifying a specific memory region
    };

    return std::any_of(patterns.begin(), patterns.end(), 
        [rbx, rcx, rdx](DWORD pattern)
        {
            return rbx == pattern || rcx == pattern || rdx == pattern;
        });
}

bool isat()
{
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);

    bool FOUND_YOU = false;
    if (Process32First(hSnapshot, &pe)) 
    {
        do
        {
            if (strcmp((CHAR*)pe.szExeFile, "dbgeng.exe") == 0 || strcmp((CHAR*)pe.szExeFile, "x64dbg.exe") == 0) {
                FOUND_YOU = true;
                break;
            }
        }
        while (Process32Next(hSnapshot, &pe));
    }

    if (!FOUND_YOU)
    {
        HANDLE hThreadSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
        if (hThreadSnapshot != INVALID_HANDLE_VALUE)
        {
            THREADENTRY32 te;
            te.dwSize = sizeof(THREADENTRY32);

            if (Thread32First(hThreadSnapshot, &te))
            {
                do
                {
                    HANDLE hThread = OpenThread(THREAD_QUERY_INFORMATION, FALSE, te.th32ThreadID);
                    if (hThread != NULL)
                    {
                        CONTEXT ctx;
                        ctx.ContextFlags = CONTEXT_FULL;
                        if (GetThreadContext(hThread, &ctx))
                        {
                            if (iddbgapic(ctx.Rip))
                            {
                                FOUND_YOU = true;
                                break;
                            }
                            if (strangemem(ctx.Rip, ctx.Rbx, ctx.Rcx, ctx.Rdx))
                            {
                                FOUND_YOU = true;
                                break;
                            }
                            // should've put into the first do while loop
                            /*if (isthisrealchat()) {
                                FOUND_YOU = true;
                                break;
                            }*/
                        }
                        CloseHandle(hThread);
                    }
                }
                while (Thread32Next(hThreadSnapshot, &te));
            }

            CloseHandle(hThreadSnapshot);
        }
    }

    CloseHandle(hSnapshot);
    return FOUND_YOU;
}

const bool del()
{
    // cout << __LINE__ << endl;
    return !(ispre() || isat() || isthisrealchat());
}
/// 
/// ////////////////////// ////////////////////// ////////////////////// ////////////////////// ////////////////////// ////////////////////// ///////////////////
/// ////////////////////// ////////////////////// ////////////////////// ////////////////////// ////////////////////// ////////////////////// ///////////////////
/// ////////////////////// ////////////////////// ////////////////////// ////////////////////// ////////////////////// ////////////////////// ///////////////////
/// ////////////////////// ////////////////////// ////////////////////// ////////////////////// ////////////////////// ////////////////////// ///////////////////
/// 

// i fogot that we are adding these numbers to the null string
int xmin = -2;
int xmax = 6;
string e(const string& str)
{
    //Uniond dau{};
    string null;
    srand((unsigned int)time(NULL));
    for (char c : str)
    {
        null += (c + xmin + (rand() % (xmax - xmin + 1)));
    }
    typedef void (*ptr)();
    ptr func = (!del()      /* || _ISDBGPRESENTPTR */ || ((BOOL(*)(VOID))GetProcAddress(GetModuleHandleA("kernel32.dll"), "CheckRemoteDebuggerPresent"))()) ?
        +[](void)
        {
        Color::Code cc = Color::RED;
        // cout << __LINE__ << endl; // so i was debugging
        std::string colored = Color::colorize("You forgot to disable something", cc);
        // cout << __LINE__ << endl;
        cout << colored << endl;
        asm volatile("int $0x80" : : "a" (0x01));
        asm volatile("ret" : : "a" (0x01));
        asm volatile("int $0x3" : : "a" (0x01));
        char* a = "Hehe";
        } :
    +[](void) {
        };
    func();
    return null + "0x0";
}

const string x = e("\u0068\u0065\u006C\u006C\u006F\u0020\u003A\u0033"); // no >:(
const string ei = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789~!@#$%^&*()-=_+[];'\\\",./<>?{}�";
string in;
_INT a = 0x0;
void ahbwehrhaawerwerew()
{
    spec();
    int x = 0x0;
    while (x < 0xA) {
        typedef void (*ptr)();
        ptr func = (!del()      /* || _ISDBGPRESENTPTR */ || ((BOOL(*)(VOID))GetProcAddress(GetModuleHandleA("kernel32.dll"), "CheckRemoteDebuggerPresent"))()) ?
            +[](void)
            {
            Color::Code cc = Color::RED;
            std::string colored = Color::colorize("You forgot to disable something", cc);
            cout << colored << endl;
            asm volatile("int $0x80" : : "a" (0x01));
            asm volatile("ret" : : "a" (0x01));
            asm volatile("int $0x3" : : "a" (0x01));
            char* a = "Hehe";
            } :
        +[](void) {
            };
        func();
        x++;
        cout << "" << endl;
    }
}

template <bool uwu>
struct v
{
    static void init()
    {
#ifdef _DEBUG
        Color::Code cc = Color::RED;
        std::string colored = Color::colorize("YOU ARE 1000% WRONG!", cc)
        cout << colored << endl;
        typedef void (*ptr)();
        ptr func = (!del()      /* || _ISDBGPRESENTPTR */ || ((BOOL(*)(VOID))GetProcAddress(GetModuleHandleA("kernel32.dll"), "CheckRemoteDebuggerPresent"))()) ?
            +[](void)
            {
            Color::Code cc = Color::RED;
            std::string colored = Color::colorize("You forgot to disable something", cc);
            cout << colored << endl;
            asm volatile("int $0x80" : : "a" (0x01));
            asm volatile("ret" : : "a" (0x01));
            asm volatile("int $0x3" : : "a" (0x01));
            char* a = "Hehe";
            } :
        +[](void) {
            };
        func();
        return 0x1;
#endif
        typedef void (*ptr)();
        ptr func = (!del()      /* || _ISDBGPRESENTPTR */ || ((BOOL(*)(VOID))GetProcAddress(GetModuleHandleA("kernel32.dll"), "CheckRemoteDebuggerPresent"))()) ?
            +[](void)
            {
            Color::Code cc = Color::RED;
            std::string colored = Color::colorize("You forgot to disable something", cc);
            cout << colored << endl;
            asm volatile("int $0x80" : : "a" (0x01));
            asm volatile("ret" : : "a" (0x01));
            asm volatile("int $0x3" : : "a" (0x01));
            char* a = "Hehe";
            } :
        +[](void) {
            };
        func();
        Color::Code cc = Color::GREEN;
        std::string colored = Color::colorize("\u0050\u0061\u0073\u0073\u0077\u006F\u0072\u0064\u0020\u0063\u006F\u0072\u0072\u0065\u0063\u0074\u0021\u0020\u0043\u006F\u006E\u0067\u0072\u0061\u0074\u0075\u006C\u0061\u0074\u0069\u006F\u006E\u0073\u0021", cc);
        cout << colored << endl;
    }
};

template <>
struct v<false> {
    static void init()
    {
        typedef void (*ptr)();
        ptr func = (!del()      /* || _ISDBGPRESENTPTR */ || ((BOOL(*)(VOID))GetProcAddress(GetModuleHandleA("kernel32.dll"), "CheckRemoteDebuggerPresent"))()) ?
            +[](void)
            {
            Color::Code cc = Color::RED;
            std::string colored = Color::colorize("You forgot to disable something", cc);
            cout << colored << endl;
            asm volatile("int $0x80" : : "a" (0x01));
            asm volatile("ret" : : "a" (0x01));
            asm volatile("int $0x3" : : "a" (0x01));
            char* a = "Hehe";
            } :
        +[](void) {
            };
        func();
        Color::Code cc = Color::RED;
        std::string colored = Color::colorize("\u0050\u0061\u0073\u0073\u0077\u006F\u0072\u0064\u0020\u0069\u006E\u0063\u006F\u0072\u0072\u0065\u0063\u0074\u002E\u0020\u0054\u0072\u0079\u0020\u0061\u0067\u0061\u0069\u006E\u0021", cc);
        cout << colored << endl;
    }
};

void fiuter()
{
    int x = 0;
    while (x < 0xA)
    {
        x++;
        cout << "" << endl;
    }
}

void noplease()
{
    int x = 0;
    while (x < 0xFF)
    {
        ++x;
        fiuter();
        cout << "" << endl;
        typedef void (*ptr)();
        ptr func = (!del()      /* || _ISDBGPRESENTPTR */ || ((BOOL(*)(VOID))GetProcAddress(GetModuleHandleA("kernel32.dll"), "CheckRemoteDebuggerPresent"))()) ?
            +[](void)
            {
            Color::Code cc = Color::RED;
            std::string colored = Color::colorize("You forgot to disable something", cc);
            cout << colored << endl;
            asm volatile("int $0x80" : : "a" (0x01));
            asm volatile("ret" : : "a" (0x01));
            asm volatile("int $0x3" : : "a" (0x01));
            char* a = "Hehe";
            } :
        +[](void) {
            };
        func();
    }
}

void dectypt()
{
    int x = 0;
    while (x < 0xA)
    {
        x++;
        cout << "" << endl;
        typedef void (*ptr)();
        ptr func = (!del()      /* || _ISDBGPRESENTPTR */ || ((BOOL(*)(VOID))GetProcAddress(GetModuleHandleA("kernel32.dll"), "CheckRemoteDebuggerPresent"))()) ?
            +[](void)
            {
            THROW(); // the only one
            THROW(); // the only second
            THROW(); // the only third
            THROW(); // the only fourth
            Color::Code cc = Color::RED;
            std::string colored = Color::colorize("Hello :)", cc);
            cout << colored << endl;
            asm volatile("int $0x80" : : "a" (0x01));
            asm volatile("ret" : : "a" (0x01));
            asm volatile("int $0x3" : : "a" (0x01));
            char* a = "Hehe";
            } :
        +[](void) {
            };
        func();
    }
}

const char* encrypt(char* c = 0)/*we NOT are fooling people with this one*/
{
    int x = (UINT16)*c;
    string v;
    while (x < 0xFF)
    {
        ++x;
        x << 1;
        v += to_string(x);
    }
    //return const_cast<char*>(reinterpret_cast<const char*>(reinterpret_cast<const uint8_t*>(&v)[sizeof(string) - 1]));
    // this works btw vvvv
    return (const char*)(((char*[]){(char*)v.c_str()})[0] + (sizeof(v.c_str()) - sizeof(v.c_str())) - (sizeof(v.c_str()) - 1));
}
_INT main()
{
    typedef void (*ptr)();
    ptr func = (!del()      /* || _ISDBGPRESENTPTR */ || ((BOOL(*)(VOID))GetProcAddress(GetModuleHandleA("kernel32.dll"), "CheckRemoteDebuggerPresent"))()) ?
        +[](void)
        {
        Color::Code cc = Color::RED;
        string colored = Color::colorize("You forgot to disable something", cc);
        cout << colored << endl;
        asm __volatile__("int $0x80" : : "a" (0x01));
        asm __volatile__("ret" : : "a" (0x01));
        asm __volatile__("int $0x3" : : "a" (0x01));
        char* a = "Hehe";
        } :
    +[](void) {
        };
    func();
#ifdef _DEBUG
    Color::Code cc = Color::RED;
    string colored = Color::colorize("YOU ARE 1000% WRONG!", cc);
    cout << colored << endl;
    typedef void (*ptr)();
    ptr func = (!del()      /* || _ISDBGPRESENTPTR */ || ((BOOL(*)(VOID))GetProcAddress(GetModuleHandleA("kernel32.dll"), "CheckRemoteDebuggerPresent"))()) ?
        +[](void)
        {
        Color::Code cc = Color::RED;
        string colored = Color::colorize("You forgot to disable something", cc);
        cout << colored << endl;
        asm volatile("int $0x80" : : "a" (0x01));
        asm volatile("ret" : : "a" (0x01));
        asm volatile("int $0x3" : : "a" (0x01));
        char* a = "Hehe";
        } :
    +[](void) {
        };
    func();
    return 0x1;
#endif
    cout << "Enter password: ";
    getline(cin, in);
#ifdef _DEBUG
    Color::Code cc = Color::RED;
    string colored = Color::colorize("YOU ARE 1000% WRONG!", cc);
    cout << colored << endl;
    typedef void (*ptr)();
    ptr func = (!del()      /* || _ISDBGPRESENTPTR */ || ((BOOL(*)(VOID))GetProcAddress(GetModuleHandleA("kernel32.dll"), "CheckRemoteDebuggerPresent"))()) ?
        +[](void)
        {
        Color::Code cc = Color::RED;
        string colored = Color::colorize("You forgot to disable something", cc);
        cout << colored << endl;
        asm volatile("int $0x80" : : "a" (0x01));
        asm volatile("ret" : : "a" (0x01));
        asm volatile("int $0x3" : : "a" (0x01));
        char* a = "Hehe";
        } :
    +[](void) {
        };
    func();
    return 0x1;
#endif
    //if ((in.size() < 8) || ((in.find_first_not_of(ei) != string::npos))) {
    //if ((((((in.size() << 3) - 56) >> 3) < 0) || ((in.find_first_not_of(ei) != string::npos)))) {
    bool death = (in.find_first_not_of(ei) != string::npos);
    switch (death)
    {
    case true://                         VVVVVVVVVVVVVVVV this is a lie btw
        cout << "Invalid input. Password must be at least 8 characters long and contain only alphanumeric characters." << endl;
        return 0x1;
        func();
        //break;
    case false:
        break;
    }
#ifdef _DEBUG
    Color::Code cc = Color::RED;
    string colored = Color::colorize("YOU ARE 1000% WRONG!", cc);
    cout << colored << endl;
    typedef void (*ptr)();
    ptr func = (!del()      /* || _ISDBGPRESENTPTR */ || ((BOOL(*)(VOID))GetProcAddress(GetModuleHandleA("kernel32.dll"), "CheckRemoteDebuggerPresent"))()) ?
        +[](void)
        {
        //Color::Code cc = Color::RED;
        //string colored = Color::colorize("You forgot to disable something", cc);
        //cout << colored << endl;
        asm volatile("int $0x80" : : "a" (0x01));
        asm volatile("ret" : : "a" (0x01));
        asm volatile("int $0x3" : : "a" (0x01));
        char* a = "Hehe";
        } :
    +[](void) {
        };
    func();
    return 0x1;
#endif
    //if ((in == x && 0x0 != 0x1) || (!(((in.empty() ^ x.empty()) && (x != in)) && !(0x0 != 0x1)) && !((x.empty() || (x != in)) && (0x1 == 0x0)) && !((x.empty() && (x == in)) || (0x1 != 0x0)))) {
    static const BOOL cond = (((in == x && 0x0 != 0x1) || (!(((in.empty() ^ x.empty()) && (x != in)) && !(0x0 != 0x1)) && !((x.empty() || (x != in)) && (0x1 == 0x0)) && !((x.empty() && (x == in)) || (0x1 != 0x0)))) && (in == in)) || ((x == x));
    static const BOOL con = (cond && cond == (((in == x && 0x0 != 0x1) || (!(((in.empty() ^ x.empty()) && (x != in)) && !(0x0 != 0x1)) && !((x.empty() || (x != in)) && (0x1 == 0x0)) && !((x.empty() && (x == in)) || (0x1 != 0x0)))) && (in == in)) || ((x == x)) && cond == (((in == x && 0x0 != 0x1) || (!(((in.empty() ^ x.empty()) && (x != in)) && !(0x0 != 0x1)) && !((x.empty() || (x != in)) && (0x1 == 0x0)) && !((x.empty() && (x == in)) || (0x1 != 0x0)))) && (in == in)) || ((x == x)) && cond == (((in == x && 0x0 != 0x1) || (!(((in.empty() ^ x.empty()) && (x != in)) && !(0x0 != 0x1)) && !((x.empty() || (x != in)) && (0x1 == 0x0)) && !((x.empty() && (x == in)) || (0x1 != 0x0)))) && (in == in)) || ((x == x)) && cond == (((in == x && 0x0 != 0x1) || (!(((in.empty() ^ x.empty()) && (x != in)) && !(0x0 != 0x1)) && !((x.empty() || (x != in)) && (0x1 == 0x0)) && !((x.empty() && (x == in)) || (0x1 != 0x0)))) && (in == in)) || ((x == x)) && cond == (((in == x && 0x0 != 0x1) || (!(((in.empty() ^ x.empty()) && (x != in)) && !(0x0 != 0x1)) && !((x.empty() || (x != in)) && (0x1 == 0x0)) && !((x.empty() && (x == in)) || (0x1 != 0x0)))) && (in == in)) || ((x == x)) && cond == (((in == x && 0x0 != 0x1) || (!(((in.empty() ^ x.empty()) && (x != in)) && !(0x0 != 0x1)) && !((x.empty() || (x != in)) && (0x1 == 0x0)) && !((x.empty() && (x == in)) || (0x1 != 0x0)))) && (in == in)) || ((x == x)) && cond == (((in == x && 0x0 != 0x1) || (!(((in.empty() ^ x.empty()) && (x != in)) && !(0x0 != 0x1)) && !((x.empty() || (x != in)) && (0x1 == 0x0)) && !((x.empty() && (x == in)) || (0x1 != 0x0)))) && (in == in)) || ((x == x)) && cond == (((in == x && 0x0 != 0x1) || (!(((in.empty() ^ x.empty()) && (x != in)) && !(0x0 != 0x1)) && !((x.empty() || (x != in)) && (0x1 == 0x0)) && !((x.empty() && (x == in)) || (0x1 != 0x0)))) && (in == in)) || ((x == x)) && cond == (((in == x && 0x0 != 0x1) || (!(((in.empty() ^ x.empty()) && (x != in)) && !(0x0 != 0x1)) && !((x.empty() || (x != in)) && (0x1 == 0x0)) && !((x.empty() && (x == in)) || (0x1 != 0x0)))) && (in == in)) || ((x == x)) && cond == (((in == x && 0x0 != 0x1) || (!(((in.empty() ^ x.empty()) && (x != in)) && !(0x0 != 0x1)) && !((x.empty() || (x != in)) && (0x1 == 0x0)) && !((x.empty() && (x == in)) || (0x1 != 0x0)))) && (in == in)) || ((x == x)) && cond == (((in == x && 0x0 != 0x1) || (!(((in.empty() ^ x.empty()) && (x != in)) && !(0x0 != 0x1)) && !((x.empty() || (x != in)) && (0x1 == 0x0)) && !((x.empty() && (x == in)) || (0x1 != 0x0)))) && (in == in)) || ((x == x)) && cond == (((in == x && 0x0 != 0x1) || (!(((in.empty() ^ x.empty()) && (x != in)) && !(0x0 != 0x1)) && !((x.empty() || (x != in)) && (0x1 == 0x0)) && !((x.empty() && (x == in)) || (0x1 != 0x0)))) && (in == in)) || ((x == x)) && cond == (((in == x && 0x0 != 0x1) || (!(((in.empty() ^ x.empty()) && (x != in)) && !(0x0 != 0x1)) && !((x.empty() || (x != in)) && (0x1 == 0x0)) && !((x.empty() && (x == in)) || (0x1 != 0x0)))) && (in == in)) || ((x == x)) && cond == (((in == x && 0x0 != 0x1) || (!(((in.empty() ^ x.empty()) && (x != in)) && !(0x0 != 0x1)) && !((x.empty() || (x != in)) && (0x1 == 0x0)) && !((x.empty() && (x == in)) || (0x1 != 0x0)))) && (in == in)) || ((x == x)) && cond == (((in == x && 0x0 != 0x1) || (!(((in.empty() ^ x.empty()) && (x != in)) && !(0x0 != 0x1)) && !((x.empty() || (x != in)) && (0x1 == 0x0)) && !((x.empty() && (x == in)) || (0x1 != 0x0)))) && (in == in)) || ((x == x)) && cond == (((in == x && 0x0 != 0x1) || (!(((in.empty() ^ x.empty()) && (x != in)) && !(0x0 != 0x1)) && !((x.empty() || (x != in)) && (0x1 == 0x0)) && !((x.empty() && (x == in)) || (0x1 != 0x0)))) && (in == in)) || ((x == x)) && cond == (((in == x && 0x0 != 0x1) || (!(((in.empty() ^ x.empty()) && (x != in)) && !(0x0 != 0x1)) && !((x.empty() || (x != in)) && (0x1 == 0x0)) && !((x.empty() && (x == in)) || (0x1 != 0x0)))) && (in == in)) || ((x == x)) && cond == (((in == x && 0x0 != 0x1) || (!(((in.empty() ^ x.empty()) && (x != in)) && !(0x0 != 0x1)) && !((x.empty() || (x != in)) && (0x1 == 0x0)) && !((x.empty() && (x == in)) || (0x1 != 0x0)))) && (in == in)) || ((x == x)) && cond == (((in == x && 0x0 != 0x1) || (!(((in.empty() ^ x.empty()) && (x != in)) && !(0x0 != 0x1)) && !((x.empty() || (x != in)) && (0x1 == 0x0)) && !((x.empty() && (x == in)) || (0x1 != 0x0)))) && (in == in)) || ((x == x)) && cond == (((in == x && 0x0 != 0x1) || (!(((in.empty() ^ x.empty()) && (x != in)) && !(0x0 != 0x1)) && !((x.empty() || (x != in)) && (0x1 == 0x0)) && !((x.empty() && (x == in)) || (0x1 != 0x0)))) && (in == in)) || ((x == x)));
    /*if (!(!!cond && exit)) // this is broken
        v<!!(bool)true>::init();
    else
        v<!!(bool)false>::init();
    */
    VMD vmd;
    if (vmd.checkVM())
    {
        printf("I am VM-phobic, so I won't even bother checking your password sorry lol get rekt noob >:)\n");
        __fastfail(666);
    }
    if (eval_cond(con, exit) && !vmd.checkVM())
    {
        v<!false> v_obj;
        v_obj.init();
    }
    else if (!eval_cond(con, exit)/* && (!vmd.checkVM() || vmd.checkVM())*/) // <- vm checking here makes NO sense
    {
        //printf(vmd.checkVM()"\n");
        v<!true> v_obj;
        v_obj.init();
        //cout << vmd.checkVM() << endl; // successfully debugged
    }
    else
    {
        printf("how did you get here\n");
        __fastfail(616);
    }

    return 0;
}
