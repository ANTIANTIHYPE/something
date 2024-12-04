#ifndef __HOOKING_H__

#define __HOOKING_H__

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32

# include <windows.h>

#else // ^^^ defined(_WIN32) ^^^ / VVV !defined(_WIN32) VVV

# include <sys/mman.h>
# include <unistd.h>
# include <stdint.h>

#endif // ^^^ !defined(_WIN32) ^^^

typedef void ( * hook_func_t ) ( );
typedef const char * CSTR;

#ifdef _WIN32

# define HOOKAPI __cdecl

#else // ^^^ defined(_WIN32) ^^^ / VVV !defined(_WIN32) VVV

# define HOOKAPI

#endif // ^^^ !defined(_WIN32) ^^^

#ifndef HOOK_SIZE

# define HOOK_SIZE 16 // Size of a hook in bytes

#endif // !defined(HOOK_SIZE)

/**
 * @brief Structure representing a Hook.
 */
typedef struct _Hook
{
    hook_func_t    original;       // Pointer to the original function.
    hook_func_t    hook;           // Pointer to the hook function.
    BYTE *         original_bytes; // Bytes of the original function prologue.
    int            enabled;        // Flag indicating if the hook is enabled.
    int            ref_count;      // Reference count.
    struct _Hook * next;           // Pointer to the next Hook in the list.
}
Hook;

/**
 * @brief Structure representing a HookManager.
 */
typedef struct _HookManager
{
    Hook * head; // Pointer to the head of the Hook list.
}
HookManager;

/**
 * @brief Creates a new HookManager.
 * 
 * @return A pointer to the newly created HookManager, or NULL if allocation fails.
 */
HookManager * HOOKAPI     create_hook_manager ( VOID )
{
    HookManager * manager = (HookManager *) malloc(sizeof(HookManager));
    if ( manager )
    {
        manager->head = NULL;
    }
    else
    {
        fprintf(stderr, "Failed to allocate memory for HookManager.\n");
    }
    return manager;
}

/**
 * @brief Increments the reference count of a hook.
 * 
 * @param hook Pointer to the Hook.
 */
VOID HOOKAPI     retain_hook ( Hook *     hook )
{
    if ( hook ) hook->ref_count++;
}

/**
 * @brief Creates a new Hook and adds it to the HookManager.
 * 
 * @param manager Pointer to the HookManager where the hook will be added.
 * @param original Pointer to the original function to be hooked.
 * @param hook Pointer to the hook function that will replace the original.
 * 
 * @return A pointer to the created Hook, or NULL if any parameter is NULL or allocation fails.
 */
Hook * HOOKAPI     create_hook ( HookManager *     manager, 
                                 hook_func_t       original, 
                                 hook_func_t       hook )
{
    if ( !original || !hook || !manager )
    {
        fprintf(stderr, "Invalid parameters provided to create_hook.\n");
        return NULL;
    }

    Hook * new_hook = (Hook *) malloc(sizeof(Hook));
    if ( !new_hook ) return NULL;

    new_hook->original = original;
    new_hook->hook = hook;
    new_hook->enabled = 0;
    new_hook->ref_count = 1;
    new_hook->original_bytes = (BYTE *) malloc(HOOK_SIZE);
    if ( !new_hook->original_bytes )
    {
        fprintf(stderr, "Failed to allocate memory for original bytes.\n");
        free(new_hook);
        return NULL;
    }

    // store the original function's prologue
    memcpy(new_hook->original_bytes, original, HOOK_SIZE);

#ifdef _WIN32

    DWORD dwOldProtect;
    VirtualProtect(original, HOOK_SIZE, PAGE_EXECUTE_READWRITE, &dwOldProtect);

#else // ^^^ defined(_WIN32) ^^^ / VVV !defined(_WIN32) VVV

    size_t page_size = sysconf(_SC_PAGESIZE);
    void * page_start = (void *) ( (uintptr_t) original & ~( page_size - 1 ) );
    mprotect(page_start, page_size, PROT_READ | PROT_WRITE | PROT_EXEC);

#endif // ^^^ !defined(_WIN32) ^^^

    // W trampoline sigma moment
    uintptr_t relative_address = (uintptr_t) hook - (uintptr_t) original - 5;
    * (BYTE *) original = 0xE9; // JMP opcode
    * (DWORD *) ( (BYTE *) original + 1 ) = (DWORD) relative_address;

#ifdef _WIN32

    VirtualProtect(original, HOOK_SIZE, dwOldProtect, &dwOldProtect);

#else // ^^^ defined(_WIN32) ^^^ / VVV !defined(_WIN32) VVV

    mprotect(page_start, page_size, PROT_READ | PROT_EXEC);

#endif // ^^^ !defined(_WIN32) ^^^

    new_hook->next = manager->head;
    manager->head = new_hook;
    return new_hook;
}

/**
 * @brief Applies a hook by replacing the original function with the hook function.
 * 
 * @param hook Pointer to the Hook to be applied.
 * 
 * @return 0 on success, -1 if the hook or its functions are NULL.
 */
INT HOOKAPI     apply_hook ( Hook *     hook )
{
    if ( !hook || !hook->original || !hook->hook )
    {
        fprintf(stderr, "Invalid hook or its functions are NULL.\n");
        return -1;
    }
    hook->enabled = 1;
    return 0;
}

/**
 * @brief Restores the original function of a hook.
 * 
 * @param hook Pointer to the Hook to be restored.
 * 
 * @return 0 on success, -1 if the hook or its functions are NULL.
 */
INT HOOKAPI     restore_hook ( Hook *     hook )
{
    if ( !hook || !hook->original || !hook->original_bytes )
    {
        fprintf(stderr, "Invalid hook or its original bytes are NULL.\n");
        return -1;
    }

#ifdef _WIN32 // VVV defined(_WIN32) VVV

    DWORD dwOldProtect;
    if ( !VirtualProtect(hook->original, HOOK_SIZE, PAGE_EXECUTE_READWRITE, &dwOldProtect) )

#else // ^^^ defined(_WIN32) ^^^ / VVV !defined(_WIN32) VVV

    if ( !mprotect(hook->original, HOOK_SIZE, PROT_READ | PROT_WRITE) )

#endif // ^^^ !defined(_WIN32) ^^^

    {
        fprintf(stderr, "Failed to change memory protection for restoring original function.\n");
        return -1;
    }

    memcpy(hook->original, hook->original_bytes, HOOK_SIZE);

#ifdef _WIN32 // VVV defined(_WIN32) VVV

    VirtualProtect(hook->original, HOOK_SIZE, dwOldProtect, &dwOldProtect);

#else // ^^^ defined(_WIN32) ^^^ / VVV !defined(_WIN32) VVV

    mprotect(page_start, page_size, PROT_READ | PROT_EXEC);

#endif // ^^^ !defined(_WIN32) ^^^

    hook->enabled = 0;
    return 0;
}

/**
 * @brief Frees the memory allocated for a Hook.
 * 
 * @param hook Pointer to the Hook to be freed.
 */
VOID HOOKAPI     free_hook ( Hook *     hook )
{
    if ( hook && --hook->ref_count == 0 )
    {
        restore_hook(hook);
        free(hook->original_bytes);
        free(hook);
    }
}

/**
 * @brief Frees the memory allocated for a HookManager and all its hooks.
 * 
 * @param manager Pointer to the HookManager to be freed.
 */
VOID HOOKAPI     free_hook_manager ( HookManager *     manager )
{
    if ( manager )
    {
        Hook * current = manager->head;
        while ( current )
        {
            Hook * next = current->next;
            free_hook(current);
            current = next;
        }
        free(manager);
    }
}

/**
 * @brief Lists all hooks in the HookManager.
 * 
 * @param manager Pointer to the HookManager whose hooks will be listed.
 */
VOID HOOKAPI     list_hooks ( HookManager *     manager )
{
    if ( !manager ) return;
    Hook * current = manager->head;
    while ( current )
    {
        printf("Hook: original=%p, hook=%p, enabled=%d, ref_count=%d\n",
               (LPVOID) current->original, (LPVOID) current->hook, current->enabled, current->ref_count);
        current = current->next;
    }
}

/**
 * @brief Executes the hook function if the hook is enabled.
 * 
 * @param hook Pointer to the Hook to be executed.
 */
VOID HOOKAPI     execute_hook ( Hook *     hook )
{
    if ( hook && hook->enabled && hook->hook ) hook->hook();
}

#endif // __HOOKING_H__
