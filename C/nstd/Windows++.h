#ifndef __WIN_PLUS_PLUS__

#define __WIN_PLUS_PLUS__

#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <comutil.h>
#include <wbemidl.h>

#ifdef __cplusplus

extern "C"
{

#endif // C++

#ifdef _MSC_VER

#pragma comment(lib, "wbemuuid.lib")

#endif // MSVC

#define __INLINE inline

#define PROT_READ       0x1
#define PROT_WRITE      0x2
#define PROT_EXEC       0x4
#define PROT_NONE       0x0

#define IS_FEATURE_SUPPORTED (version)     ( GetVersion() >= version )

typedef const char * CSTR;
typedef void (* WMIObjectCallback) (IWbemClassObject * pclsObj, 
                                    LPVOID             userData);

__INLINE VOID WINAPI_INLINE     sendWQLQuery (CSTR                  query, 
                                              WMIObjectCallback     callback, 
                                              LPVOID                userData)
{
    HRESULT hres;

    hres = CoInitializeEx(0, COINIT_MULTITHREADED);
    if ( FAILED(hres) )
    {
        printf("Failed to initialize COM library. Error code 0x%X\n", hres);
        return;
    }

    hres = CoInitializeSecurity(
        NULL,
        -1,                          // COM negotiates service
        NULL,                        // Authentication services
        NULL,                        // Reserved
        RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication
        RPC_C_IMP_LEVEL_IMPERSONATE, // Impersonation
        NULL,                        // Reserved
        EOAC_NONE,                   // Additional capabilities
        NULL                         // Reserved
    );

    if ( FAILED(hres) )
    {
        printf("Failed to initialize security. Error code 0x%X\n", hres);
        CoUninitialize();
        return;
    }

    IWbemLocator * pLoc = NULL;
    hres = CoCreateInstance(
        CLSID_WbemLocator,
        0,
        CLSCTX_INPROC_SERVER,
        IID_IWbemLocator, (LPVOID *)&pLoc);

    if ( FAILED(hres) )
    {
        printf("Failed to create IWbemLocator object. Error code 0x%X\n", hres);
        CoUninitialize();
        return;
    }

    IWbemServices * pSvc = NULL;
    hres = pLoc->ConnectServer(
        _bstr_t(L"ROOT\\CIMV2"), // WMI namespace
        NULL,                    // User name
        NULL,                    // User password
        0,                       // Locale
        NULL,                    // Security flags
        0,                       // Authority
        0,                       // Context object
        &pSvc                    // IWbemServices proxy
    );

    if ( FAILED(hres) )
    {
        printf("Could not connect. Error code 0x%X\n", hres);
        pLoc->Release();
        CoUninitialize();
        return;
    }

    hres = CoSetProxyBlanket(
        pSvc,                        // Indicates the proxy to set
        RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
        RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
        NULL,                        // Server principal name 
        RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx
        RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
        NULL,                        // Client identity
        EOAC_NONE                    // Additional capabilities
    );

    if ( FAILED(hres) )
    {
        printf("Could not set proxy blanket. Error code 0x%X\n", hres);
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return;
    }

    IEnumWbemClassObject * pEnumerator = NULL;
    hres = pSvc->ExecQuery(
        bstr_t("WQL"),
        bstr_t(query),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL,
        &pEnumerator);

    if ( FAILED(hres) )
    {
        printf("Query failed. Error code 0x%X\n", hres);
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return;
    }

    IWbemClassObject * pclsObj = NULL;
    ULONG uReturn = 0;

    while ( pEnumerator )
    {
        HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
        if ( 0 == uReturn )
        {
            break;
        }

        if ( callback )
        {
            callback(pclsObj, userData);
        }

        pclsObj->Release();
    }

    pEnumerator->Release();
    pSvc->Release();
    pLoc->Release();
    CoUninitialize();
}

__INLINE WINBOOL WINAPI_INLINE     mprotect (LPVOID     addr, 
                                             SIZE_T     len, 
                                             int        prot)
{
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    DWORD pageSize = si.dwPageSize;

    LPVOID alignedAddr = (LPVOID) ( (uintptr_t) addr & ~( pageSize - 1 ) );
    size_t alignedLen = ( (uintptr_t) addr + len + pageSize - 1 ) & ~( pageSize - 1 ) - (uintptr_t) alignedAddr;

    DWORD oldProtect;
    DWORD newProtect = 0;

    if ( prot & PROT_READ )
    {
        newProtect |= PAGE_READONLY;
    }
    if ( prot & PROT_WRITE )
    {
        newProtect |= PAGE_READWRITE;
    }
    if ( prot & PROT_EXEC )
    {
        newProtect |= PAGE_EXECUTE_READWRITE;
    }
    if ( newProtect == 0 )
    {
        newProtect = PAGE_NOACCESS;
    }

    if ( !VirtualProtect(alignedAddr, alignedLen, newProtect, &oldProtect) )
    {
        PrintLastError("mprotect");
        return FALSE;
    }

    return TRUE;
}

__INLINE INT WINAPI_INLINE     CheckProtection (LPVOID     addr)
{
    MEMORY_BASIC_INFORMATION mbi;
    if (VirtualQuery(addr, &mbi, sizeof(mbi)))
    {
        switch (mbi.Protect)
        {
            case PAGE_READONLY:          return PROT_READ;
            case PAGE_READWRITE:         return PROT_READ | PROT_WRITE;
            case PAGE_EXECUTE_READ:      return PROT_EXEC | PROT_READ;
            case PAGE_EXECUTE_READWRITE: return PROT_EXEC | PROT_READ | PROT_WRITE;
            case PAGE_NOACCESS:          return PROT_NONE;
            default:                     return -1; // Unknown protection
        }
    }
    return -1;
}

__INLINE VOID WINAPI_INLINE     PrintLastError (CSTR     funcName)
{
    DWORD errcode = GetLastError();
    CSTR err = GetLastErrorStr();

    fprintf(stderr, "Error in %s: %lu - %S\n", funcName, errcode, (LPWSTR) err);
}

__INLINE CSTR WINAPI_INLINE     GetLastErrorStr ()
{
    static char errmsg[512];
    DWORD errcode = GetLastError();
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 
        NULL, 
        errcode,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
        (LPWSTR) errmsg, 
        sizeof(errmsg), 
        NULL);
    return errmsg;
}

__INLINE HANDLE WINAPI_INLINE     CreateNewThread (LPTHREAD_START_ROUTINE     threadFunc, 
                                                   LPVOID                     param)
{
    HANDLE hThread = CreateThread(NULL, 0, threadFunc, param, 0, NULL);
    if ( hThread == NULL )
    {
        PrintLastError("CreateNewThread");
    }

    return hThread;
}

__INLINE VOID WINAPI_INLINE     WaitForThread (HANDLE     hThread)
{
    if ( WaitForSingleObject(hThread, INFINITE) == WAIT_FAILED )
    {
        PrintLastError("WaitForThread");
    }

    CloseHandle(hThread);
}

__INLINE VOID WINAPI_INLINE     LockMutex (HANDLE     hMutex)
{
    DWORD waitResult = WaitForSingleObject(hMutex, INFINITE);
    if ( waitResult != WAIT_OBJECT_0 )
    {
        PrintLastError("LockMutex");
    }
}

__INLINE VOID WINAPI_INLINE     UnlockMutex (HANDLE     hMutex)
{
    if ( !ReleaseMutex(hMutex) )
    {
        PrintLastError("UnlockMutex");
    }
}

__INLINE LPVOID WINAPI_INLINE     smalloc (size_t     size)
{
    LPVOID ptr = malloc(size);
    if ( ptr == NULL )
    {
        printf("Memory allocation failed for size: %zu", size);
        exit(EXIT_FAILURE);
    }

    return ptr;
}

__INLINE VOID WINAPI_INLINE     sfree (LPVOID *     ptr)
{
    if ( ptr && * ptr )
    {
        free(* ptr);
        * ptr = NULL;
    }
}

__INLINE INT WINAPI_INLINE     FileExists (CSTR     filename)
{
    DWORD fileAttr = GetFileAttributesA(filename);
    return ( fileAttr != INVALID_FILE_ATTRIBUTES && !( fileAttr & FILE_ATTRIBUTE_DIRECTORY ) );
}

__INLINE WINBOOL WINAPI_INLINE     WriteToFile (CSTR     filename, 
                                                CSTR     message)
{
    HANDLE hFile = CreateFileA(filename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if ( hFile == INVALID_HANDLE_VALUE )
    {
        return FALSE;
    }

    DWORD bytesWritten;
    BOOL result = WriteFile(hFile, message, strlen(message), &bytesWritten, NULL);
    CloseHandle(hFile);
    return result;
}

__INLINE CHAR * WINAPI_INLINE     ReadFromFile (CSTR     filename)
{
    HANDLE hFile = CreateFileA(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if ( hFile == INVALID_HANDLE_VALUE )
    {
        return NULL;
    }

    DWORD fileSize = GetFileSize(hFile, NULL);
    CHAR * buffer = (CHAR *) smalloc(fileSize + 1);

    DWORD bytesRead;
    ReadFile(hFile, buffer, fileSize, &bytesRead, NULL);
    buffer[fileSize] = '\0';

    CloseHandle(hFile);
    return buffer;
}

__INLINE VOID WINAPI_INLINE     MessageLoop ()
{
    MSG msg;
    while ( GetMessage(&msg, NULL, 0, 0) )
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

__INLINE VOID WINAPI_INLINE     SetWindowTransparency (HWND     hWnd, 
                                                       BYTE     alpha)
{
    LONG_PTR style = GetWindowLongPtr(hWnd, GWL_EXSTYLE);
    if ( !( style & WS_EX_LAYERED ) )
    {
        SetWindowLongPtr(hWnd, GWL_EXSTYLE, style | WS_EX_LAYERED);
    }
    if ( !SetLayeredWindowAttributes(hWnd, 0, alpha, LWA_ALPHA) )
    {
        PrintLastError("SetWindowTransparency");
    }
}

__INLINE WINBOOL WINAPI_INLINE     RegisterHotkey (HWND     hWnd, 
                                                   int      id, 
                                                   UINT     fsModifiers, 
                                                   UINT     vk)
{
    if ( !RegisterHotKey(hWnd, id, fsModifiers, vk) )
    {
        PrintLastError("RegisterHotkey");
        return FALSE;
    }

    return TRUE;
}

__INLINE WINBOOL WINAPI_INLINE     UnregisterHotkey (HWND     hWnd, 
                                                     int      id)
{
    if ( !UnregisterHotKey(hWnd, id) )
    {
        PrintLastError("UnregisterHotkey");
        return FALSE;
    }

    return TRUE;
}

__INLINE INT WINAPI_INLINE      GetWindowPosition (HWND     hWnd, 
                                                   int *    x, 
                                                   int *    y)
{
    RECT rect;
    if ( GetWindowRect(hWnd, &rect) )
    {
        * x = rect.left;
        * y = rect.top;
        return * x, * y;
    }

    PrintLastError("GetWindowPosition");
}

__INLINE WINBOOL WINAPI_INLINE      SetWindowPosition (HWND     hWnd, 
                                                       int      x, 
                                                       int      y)
{
    if ( SetWindowPos(hWnd, NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE) )
    {
        return TRUE;
    }

    PrintLastError("SetWindowPosition");
}


#ifdef __cplusplus

}

#endif // C++

#endif // __WIN_PLUS_PLUS__
