// Kernel.h implementation is outside of the repository.

#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>

typedef void *       HANDLE;
typedef uint32_t     DWORD;
typedef long         LONG;
typedef long long    LONGLONG;
typedef int          BOOL;
typedef size_t       SIZE_T;
typedef uint64_t     ULONG_PTR;
typedef uint64_t     UINT_PTR;
typedef uint8_t      BYTE;
typedef uint16_t     WORD;
typedef void *       PVOID;
typedef void *       LPVOID;
typedef const void * LPCVOID;
typedef DWORD *      LPDWORD;
typedef DWORD        (*LPTHREAD_START_ROUTINE)(LPVOID lpParameter);
typedef unsigned int UINT;

#define KERNEL_SUCCESS                                           0
#define KERNEL_ERROR                                            -1
#define KERNEL_TRUE                                              1
#define KERNEL_FALSE                                             0

#define KERNEL_NULL                                    ((void *)0)

#define STD_INPUT_HANDLE                              ((DWORD)-10)
#define STD_OUTPUT_HANDLE                             ((DWORD)-11)
#define STD_ERROR_HANDLE                              ((DWORD)-12)

#define ERROR_SUCCESS                                            0
#define ERROR_IO_PENDING                                       997
#define ERROR_ACCESS_DENIED                                      5
#define ERROR_FILE_NOT_FOUND                                     2
#define ERROR_PATH_NOT_FOUND                                     3
#define ERROR_NO_MORE_FILES                                     18
#define ERROR_ALREADY_EXISTS                                   183
#define ERROR_INVALID_HANDLE                                     6
#define ERROR_OPERATION_ABORTED                                995
#define ERROR_NOT_ENOUGH_MEMORY                                  8
#define ERROR_NOT_SUPPORTED                                     50
#define ERROR_NOT_FOUND                                       1168
#define ERROR_INVALID_FUNCTION                                   1
#define ERROR_INVALID_PARAMETER                                 87
#define ERROR_OUTOFMEMORY                                       14
#define ERROR_HANDLE_EOF                                        38
#define ERROR_HANDLE_DISK_FULL                                  39
#define ERROR_DISK_CORRUPT                                    1392
#define ERROR_GEN_FAILURE                                       31
#define ERROR_INVALID_BLOCK                                   1001
#define ERROR_INSUFFICIENT_BUFFER                              122
#define ERROR_WAIT_TIMEOUT                                     258
#define ERROR_INVALID_OPERATION                                  1

#define CREATE_ALWAYS                                            2
#define OPEN_EXISTING                                            3

#define FILE_READ_DATA                                      0x0001
#define FILE_WRITE_DATA                                     0x0002
#define FILE_APPEND_DATA                                    0x0004
#define FILE_READ_ATTRIBUTES                                0x0080
#define FILE_WRITE_ATTRIBUTES                               0x0100
#define FILE_EXECUTE                                        0x0020
#define FILE_DELETE_CHILD                                   0x0040
#define FILE_SHARE_READ_WRITE (FILE_SHARE_READ | FILE_SHARE_WRITE)

#define FILE_SHARE_READ                                     0x0001
#define FILE_SHARE_WRITE                                    0x0002
#define FILE_SHARE_DELETE                                   0x0004

#define FILE_FLAG_OVERLAPPED                            0x40000000
#define FILE_FLAG_RANDOM_ACCESS                         0x10000000
#define FILE_FLAG_SEQUENTIAL_SCAN                       0x08000000
#define FILE_FLAG_NO_BUFFERING                          0x20000000
#define FILE_FLAG_WRITE_THROUGH                         0x80000000
#define FILE_FLAG_DELETE_ON_CLOSE                       0x04000000
#define FILE_FLAG_BACKUP_SEMANTICS                      0x02000000
#define FILE_FLAG_POSIX_SEMANTICS                       0x01000000

#define FILE_ATTRIBUTE_DIRECTORY                              0x10
#define FILE_ATTRIBUTE_ARCHIVE                                0x20
#define FILE_ATTRIBUTE_HIDDEN                                 0x02
#define FILE_ATTRIBUTE_SYSTEM                                 0x04
#define FILE_ATTRIBUTE_READONLY                               0x01

#define THREAD_PRIORITY_LOWEST                          0x00000000
#define THREAD_PRIORITY_NORMAL                          0x00000002
#define THREAD_PRIORITY_HIGHEST                         0x00000004

#define EVENT_MANUAL_RESET                                       1
#define EVENT_AUTO_RESET                                         0

#define SEMAPHORE_NO_WAIT                                        0
#define SEMAPHORE_WAIT_FOREVER                          0xFFFFFFFF

#define PROCESS_QUERY_INFORMATION                           0x0400
#define PROCESS_SET_INFORMATION                             0x0200
#define PROCESS_TERMINATE                                   0x0001
#define PROCESS_ALL_ACCESS                                0x1F0FFF

#define CREATE_NEW_FILE                                          1
#define CREATE_ALWAYS                                            2
#define OPEN_EXISTING                                            3
#define OPEN_ALWAYS                                              4
#define TRUNCATE_EXISTING                                        5

#define MEMORY_COMMIT                                       0x1000
#define MEMORY_RESERVE                                      0x2000
#define MEMORY_RELEASE                                      0x8000

#define HEAP_ZERO_MEMORY                                0x00000008
#define HEAP_GENERATE_EXCEPTIONS                        0x00000001

#define MAX_PATH_LENGTH                                        260

#define CREATE_SUSPENDED                                0x00000004
#define DETACHED_PROCESS                                0x00000008

#define WAIT_ABANDONED                                         183
#define WAIT_IO_COMPLETION                              0xFFFFFFFF
#define WAIT_OBJECT_0                                   0x00000000
#define WAIT_FAILED                                     0xFFFFFFFF
#define WAIT_TIMEOUT                                           258

#define MUTEX_INITIAL_OWNER                                      1
#define MUTEX_NO_INITIAL_OWNER                                   0

#define CREATE_NO_WINDOW                                0x08000000
#define CREATE_NEW_CONSOLE                              0x00000010

#define INFINITE                                        0xFFFFFFFF
#define INFINITE_TIMEOUT                                0xFFFFFFFF

#define DEFAULT_STACK_SIZE                                 0x10000

#define STATUS_SUCCESS                                  0x00000000
#define STATUS_PENDING                                  0x00000003
#define STATUS_ACCESS_DENIED                            0xC0000022
#define STATUS_INSUFFICIENT_RESOURCES                   0xC000009A
#define STATUS_NOT_IMPLEMENTED                          0xC0000002
#define STATUS_INVALID_PARAMETER                        0xC000000D

typedef struct _STARTUPINFO
{
    DWORD  cb;
    char * lpReserved;
    char * lpDesktop;
    char * lpTitle;
    DWORD  dwX;
    DWORD  dwY;
    DWORD  dwXSize;
    DWORD  dwYSize;
    DWORD  dwXCountChars;
    DWORD  dwYCountChars;
    DWORD  dwFillAttribute;
    DWORD  dwFlags;
    WORD   wShowWindow;
    WORD   cbReserved2;
    BYTE * lpReserved2;
    HANDLE hStdInput;
    HANDLE hStdOutput;
    HANDLE hStdError;
} STARTUPINFO, * PSTARTUPINFO;

typedef struct _FILETIME
{
    DWORD dwLowDateTime;
    DWORD dwHighDateTime;
} FILETIME, * PFILETIME;

typedef struct _PROCESS_INFORMATION
{
    HANDLE hProcess;
    HANDLE hThread;
    DWORD  dwProcessId;
    DWORD  dwThreadId;
} PROCESS_INFORMATION, * PPROCESS_INFORMATION;

typedef struct _THREAD_INFORMATION
{
    HANDLE hThread;
    DWORD  dwThreadId;
    DWORD  dwExitCode;
} THREAD_INFORMATION, * PTHREAD_INFORMATION;

typedef struct _FILE_INFORMATION
{
    DWORD    dwFileSizeHigh;
    DWORD    dwFileSizeLow;
    DWORD    dwFileAttributes;
    FILETIME ftCreationTime;
    FILETIME ftLastAccessTime;
    FILETIME ftLastWriteTime;
} FILE_INFORMATION, * PFILE_INFORMATION;

typedef struct _OVERLAPPED
{
    DWORD  Internal;
    DWORD  InternalHigh;
    DWORD  Offset;
    DWORD  OffsetHigh;
    HANDLE hEvent;
} OVERLAPPED, * POVERLAPPED;

typedef struct _SYSTEMTIME
{
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
} SYSTEMTIME, * PSYSTEMTIME;

typedef struct _POINT
{
    LONG x;
    LONG y;
} POINT, * PPOINT;

typedef struct _RECT
{
    LONG left;
    LONG top;
    LONG right;
    LONG bottom;
} RECT, * PRECT;

typedef struct _SIZE
{
    LONG cx;
    LONG cy;
} SIZE, * PSIZE;

typedef struct _COLORREF
{
    BYTE rgbBlue;
    BYTE rgbGreen;
    BYTE rgbRed;
} COLORREF, * PCOLORREF;

typedef struct _PROCESS_MEMORY_COUNTERS
{
    DWORD  cb;
    DWORD  PageFaultCount;
    SIZE_T PeakWorkingSetSize;
    SIZE_T WorkingSetSize;
    DWORD  Reserved[2];
    SIZE_T PagefileUsage;
    SIZE_T PeakPagefileUsage;
} PROCESS_MEMORY_COUNTERS, * PPROCESS_MEMORY_COUNTERS;

typedef struct _SECURITY_ATTRIBUTES
{
    DWORD  nLength;
    LPVOID lpSecurityDescriptor;
    BOOL   bInheritHandle;
} SECURITY_ATTRIBUTES, * PSECURITY_ATTRIBUTES;

typedef union _LARGE_INTEGER
{
    struct
    {
        DWORD LowPart;
        LONG  HighPart;
    };
    LONGLONG QuadPart;
} LARGE_INTEGER, * PLARGE_INTEGER;

typedef struct _THREAD_POOL
{
    HANDLE * threads;
    DWORD    threadCount;
    BOOL     isActive;
} THREAD_POOL, * PTHREAD_POOL;

typedef struct _MUTEX
{
    HANDLE hMutex;
    BOOL   isLocked;
} MUTEX, * PMUTEX;

typedef struct _SEMAPHORE
{
    HANDLE hSemaphore;
    LONG   count;
} SEMAPHORE, * PSEMAPHORE;

typedef struct _EVENT
{
    HANDLE hEvent;
    BOOL   isManualReset;
} EVENT, * PEVENT;

typedef struct _FILE_DESCRIPTOR
{
    HANDLE hFile;
    DWORD  accessMode;
} FILE_DESCRIPTOR, * PFILE_DESCRIPTOR;

typedef struct _NETWORK_ADDRESS
{
    BYTE address[4];
    WORD port;
} NETWORK_ADDRESS, * PNETWORK_ADDRESS;

typedef struct _IO_OPERATION
{
    HANDLE     hFile;
    OVERLAPPED overlapped;
} IO_OPERATION, * PIO_OPERATION;

typedef struct _SYSTEM_INFO
{
    DWORD dwProcessorArchitecture;
    DWORD dwPageSize;
    DWORD dwNumberOfProcessors;
} SYSTEM_INFO, * PSYSTEM_INFO;

typedef struct _MEMORY_STATUS
{
    DWORD  dwLength;
    DWORD  dwMemoryLoad;
    SIZE_T dwTotalPhys;
    SIZE_T dwAvailPhys;
} MEMORY_STATUS, * PMEMORY_STATUS;

typedef struct _MEMORY_BASIC_INFORMATION
{
    PVOID  BaseAddress;
    PVOID  AllocationBase;
    DWORD  AllocationProtect;
    SIZE_T RegionSize;
    DWORD  State;
    DWORD  Protect;
    DWORD  Type;
} MEMORY_BASIC_INFORMATION, *PMEMORY_BASIC_INFORMATION;

#ifdef __cplusplus
extern "C"
{
#endif

HANDLE GetStdHandle             (DWORD dwStdHandle);
BOOL   CloseHandle              (HANDLE hObject);

HANDLE CreateFile               (const char * lpFileName, 
                                DWORD dwDesiredAccess, 
                                DWORD dwShareMode, 
                                void * lpSecurityAttributes, 
                                DWORD dwCreationDisposition, 
                                DWORD dwFlagsAndAttributes, 
                                HANDLE hTemplateFile);
BOOL   DeleteFile               (const char * lpFileName);
BOOL   MoveFile                 (const char * lpExistingFileName, 
                                const char * lpNewFileName);
BOOL   ReadFile                 (HANDLE hFile, 
                                void * lpBuffer, 
                                DWORD nNumberOfBytesToRead, 
                                DWORD * lpNumberOfBytesRead, 
                                void * lpOverlapped);
BOOL   WriteFile                (HANDLE hFile, 
                                const void * lpBuffer, 
                                DWORD nNumberOfBytesToWrite, 
                                DWORD * lpNumberOfBytesWritten, 
                                void * lpOverlapped);
BOOL   CloseFile                (HANDLE hFile);

DWORD  GetFileSize              (HANDLE hFile);
BOOL   GetFileInformation       (HANDLE hFile, PFILE_INFORMATION pFileInfo);
BOOL   SetFileAttributes        (const char * lpFileName, DWORD dwFileAttributes);
BOOL   GetFileAttributes        (const char * lpFileName, DWORD * lpFileAttributes);
BOOL   GetFileAttributesEx      (const char * lpFileName, 
                                DWORD dwFileInfoLevel, 
                                PFILE_INFORMATION pFileInfo);
BOOL   SetFilePointer           (HANDLE hFile, 
                                LONG lDistanceToMove, 
                                DWORD dwMoveMethod);

void   ExitProcess              (UINT uExitCode);
BOOL   CreateProcess            (const char * lpApplicationName, 
                                char * lpCommandLine, 
                                void * lpProcessAttributes, 
                                void * lpThreadAttributes, 
                                BOOL bInheritHandles, 
                                DWORD dwCreationFlags, 
                                void * lpEnvironment, 
                                const char * lpCurrentDirectory, 
                                PSTARTUPINFO lpStartupInfo, 
                                PPROCESS_INFORMATION lpProcessInformation);
BOOL   TerminateProcess         (HANDLE hProcess, UINT uExitCode);
BOOL   GetExitCodeProcess       (HANDLE hProcess, LPDWORD lpExitCode);

HANDLE HeapCreate               (DWORD flOptions, 
                                SIZE_T dwInitialSize, 
                                SIZE_T dwMaximumSize);
void * HeapAlloc                (HANDLE hHeap, 
                                DWORD dwFlags, 
                                SIZE_T dwBytes);
BOOL   HeapFree                 (HANDLE hHeap, 
                                DWORD dwFlags, 
                                void * lpMem);
BOOL   HeapDestroy              (HANDLE hHeap);

BOOL   GetMemoryStatus          (PMEMORY_STATUS pMemoryStatus);

void * VirtualAlloc             (LPVOID lpAddress, 
                                SIZE_T dwSize, 
                                DWORD flAllocationType, 
                                DWORD flProtect);
BOOL   VirtualFree              (LPVOID lpAddress, 
                                SIZE_T dwSize, 
                                DWORD dwFreeType);
BOOL   VirtualQuery             (LPCVOID lpAddress, 
                                PMEMORY_BASIC_INFORMATION lpBuffer, 
                                SIZE_T dwLength);


BOOL   GetSystemInfoEx          (PSYSTEM_INFO pSystemInfo);

HANDLE CreateMutex              (void * lpMutexAttributes, 
                                BOOL bInitialOwner, 
                                const char * lpName);
BOOL   ReleaseMutex             (HANDLE hMutex);
DWORD  WaitForSingleObject      (HANDLE hHandle, DWORD dwMilliseconds);

DWORD  GetLastError             (void);

BOOL   CreateNewThread          (void * lpThreadAttributes, 
                                DWORD dwStackSize, 
                                LPTHREAD_START_ROUTINE lpStartAddress);
BOOL   GetThreadId              (HANDLE hThread, DWORD * lpThreadId);
BOOL   SuspendThread            (HANDLE hThread);
BOOL   ResumeThread             (HANDLE hThread);
BOOL   GetThreadPriority        (HANDLE hThread, LPDWORD lpPriority);
BOOL   SetThreadPriority        (HANDLE hThread, DWORD dwPriority);

BOOL   InitializeThreadPool     (PTHREAD_POOL pThreadPool, DWORD threadCount);
BOOL   AddThreadToPool          (PTHREAD_POOL pThreadPool, 
                                LPTHREAD_START_ROUTINE lpStartAddress, 
                                LPVOID lpParameter);
BOOL   RemoveThreadFromPool     (PTHREAD_POOL pThreadPool, HANDLE hThread);
BOOL   ShutdownThreadPool       (PTHREAD_POOL pThreadPool);

BOOL   GetProcessId             (HANDLE hProcess, DWORD * lpProcessId);
BOOL   GetProcessMemoryInfo     (HANDLE hProcess, 
                                PPROCESS_MEMORY_COUNTERS ppsmemCounters, 
                                DWORD cb);
BOOL   SetProcessWorkingSetSize (HANDLE hProcess, 
                                SIZE_T dwSizeMin, 
                                SIZE_T dwSizeMax);
BOOL   WaitForProcess           (HANDLE hProcess, DWORD dwMilliseconds);
BOOL   GetProcessPriority       (HANDLE hProcess, DWORD * lpPriority);
BOOL   SetProcessPriority       (HANDLE hProcess, DWORD dwPriority);
BOOL   GetProcessHandleCount    (HANDLE hProcess, LPDWORD lpdwHandleCount);
BOOL   EnumerateProcesses       (DWORD * processIds, DWORD * count);

BOOL   CreateEvent              (SECURITY_ATTRIBUTES * lpEventAttributes, 
                                BOOL bManualReset, 
                                BOOL bInitialState, 
                                const char * lpName);
BOOL   SetEvent                 (HANDLE hEvent);
BOOL   ResetEvent               (HANDLE hEvent);
BOOL   PulseEvent               (HANDLE hEvent);
BOOL   WaitForEvent             (HANDLE hEvent, DWORD dwMilliseconds);
BOOL   WaitForEvents            (DWORD nCount, 
                                const HANDLE * pHandles, 
                                BOOL bWaitAll, 
                                DWORD dwMilliseconds);

BOOL   CreateSemaphore          (SECURITY_ATTRIBUTES * lpSemaphoreAttributes, 
                                LONG lInitialCount, 
                                LONG lMaximumCount, 
                                const char * lpName);
BOOL   ReleaseSemaphore         (HANDLE hSemaphore, LONG lReleaseCount);
BOOL   GetSemaphoreInfo         (HANDLE hSemaphore, LONG * lpCount);
BOOL   SetSemaphoreCount        (HANDLE hSemaphore, LONG lNewCount);

BOOL   GetSystemTime            (PSYSTEMTIME lpSystemTime);
BOOL   SetSystemTime            (const SYSTEMTIME * lpSystemTime);

#define FREE                    (ptr)              do { if (ptr) { HeapFree(KERNEL_NULL, 0, ptr); ptr = KERNEL_NULL; } } while (0)

#define SWAP                    (a, b)             do { typeof(a) temp = a; a = b; b = temp; } while (0)

#define BIT_MASK                (bit)              (1UL << (bit))

#define MAKE_DWORD              (low, high)        (((DWORD)(low)) | ((DWORD)(high) << 32))

#define GET_WORD                (value)            ((DWORD)(value) & 0xFFFF)
#define GET_HIGH_WORD           (value)            ((DWORD)(((value) >> 32) & 0xFFFFFFFF))
#define GET_LOW_WORD            (value)            ((DWORD)((value) & 0xFFFFFFFF))

#define TIMEOUT                 (ms)               (WAIT_TIMEOUT + (ms))

#define IS_PROCESS_RUNNING      (pid)              (GetProcessId(pid) != NULL)

#define IS_MEMORY_ALIGNED       (ptr, alignment)   (((uintptr_t)(ptr) % (alignment)) == 0)

#ifdef __cplusplus
}
#endif

#endif // KERNEL_H