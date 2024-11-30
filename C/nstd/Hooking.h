#ifndef __HOOKING_H__

#define __HOOKING_H__

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

typedef void (* hook_func_t) ();

#define __INLINE static inline
#define __NORETURN __declspec(noreturn)
#define HOOKAPI __cdecl

#ifndef HOOK_SIZE

#define HOOK_SIZE 16 // Size of a hook in bytes

#endif // !defined(HOOK_SIZE)

/**
 * @brief Structure representing a Hook.
 */
typedef struct Hook
{
    hook_func_t   original;       // Pointer to the original function.
    hook_func_t   hook;           // Pointer to the hook function.
    BYTE *        original_bytes; // Bytes of the original function prologue.
    int           enabled;        // Flag indicating if the hook is enabled (0 or 1).
    struct Hook * next;           // Pointer to the next Hook in the list.
}
Hook;

/**
 * @brief Structure representing a HookManager.
 */
typedef struct HookManager
{
    Hook * head; // Pointer to the head of the Hook list.
}
HookManager;

/**
 * @brief Creates a new HookManager.
 * 
 * @return A pointer to the newly created HookManager, or NULL if allocation fails.
 */
__INLINE HookManager * HOOKAPI     create_hook_manager ()
{
    HookManager * manager = (HookManager *) malloc(sizeof(HookManager));
    if ( manager )
    {
        manager->head = NULL;
    }
    return manager;
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
__INLINE Hook * HOOKAPI     create_hook (HookManager *     manager, 
                                         hook_func_t       original, 
                                         hook_func_t       hook)
{
    if ( !original || !hook || !manager ) return NULL;

    Hook * new_hook = (Hook *) malloc(sizeof(Hook));
    if ( !new_hook ) return NULL;

    new_hook->original = original;
    new_hook->hook = hook;
    new_hook->enabled = 0;
    new_hook->original_bytes = (BYTE *) malloc(HOOK_SIZE);
    if ( !new_hook->original_bytes )
    {
        free(new_hook);
        return NULL;
    }

    // store the original function's prologue
    memcpy(new_hook->original_bytes, original, HOOK_SIZE);

    DWORD dwOldProtect;
    VirtualProtect(original, HOOK_SIZE, PAGE_EXECUTE_READWRITE, &dwOldProtect);

    // W trampoline sigma moment
    uintptr_t relative_address = (uintptr_t) hook - (uintptr_t) original - 5;
    * (BYTE *) original = 0xE9; // JMP opcode
    * (DWORD *) ( (BYTE *) original + 1 ) = (DWORD) relative_address;

    VirtualProtect(original, HOOK_SIZE, dwOldProtect, &dwOldProtect);

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
__INLINE INT HOOKAPI     apply_hook (Hook *     hook)
{
    if ( !hook || !hook->original || !hook->hook ) return -1;
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
__INLINE INT HOOKAPI     restore_hook (Hook *     hook)
{
    if ( !hook || !hook->original || !hook->original_bytes ) return -1;

    DWORD dwOldProtect;
    VirtualProtect(hook->original, HOOK_SIZE, PAGE_EXECUTE_READWRITE, &dwOldProtect);
    memcpy(hook->original, hook->original_bytes, HOOK_SIZE);
    VirtualProtect(hook->original, HOOK_SIZE, dwOldProtect, &dwOldProtect);

    hook->enabled = 0;
    return 0;
}

/**
 * @brief Frees the memory allocated for a Hook.
 * 
 * @param hook Pointer to the Hook to be freed.
 */
__INLINE VOID HOOKAPI     free_hook (Hook *     hook)
{
    if ( hook )
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
__INLINE VOID HOOKAPI     free_hook_manager (HookManager *     manager)
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
__INLINE VOID HOOKAPI     list_hooks (HookManager *     manager)
{
    if ( !manager ) return;
    Hook * current = manager->head;
    while ( current )
    {
        printf("Hook: original=%p, hook=%p, enabled=%d\n", 
               (LPVOID)current->original, (LPVOID) current->hook, current->enabled);
        current = current->next;
    }
}

/**
 * @brief Executes the hook function if the hook is enabled.
 * 
 * @param hook Pointer to the Hook to be executed.
 */
__INLINE __NORETURN VOID HOOKAPI     execute_hook (Hook *     hook)
{
    if (hook && hook->enabled && hook->hook)
    {
        hook->hook();
    }
}

#endif // __HOOKING_H__
