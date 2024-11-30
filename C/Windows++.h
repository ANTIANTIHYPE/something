#ifndef __WIN_PLUS_PLUS__

#define __WIN_PLUS_PLUS__

#include <windows.h>
#include <stdio.h>
#include <time.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define __INLINE inline

#define IS_FEATURE_SUPPORTED (version)     ( GetVersion() >= version )

typedef const char * CSTR;

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

__INLINE LPVOID WINAPI_INLINE     smalloc (SIZE_T     size)
{
    LPVOID ptr = malloc(size);
    if ( ptr == NULL )
    {
        printf("Memory allocation failed for size: %zu", size);
        exit(EXIT_FAILURE);
    }

    return ptr;
}

__INLINE VOID WINAPI_INLINE     sfree (void **     ptr)
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
                                                   INT      id, 
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
                                                     INT      id)
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
#endif

#endif // __WIN_PLUS_PLUS__