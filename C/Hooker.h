#ifndef HOOKER_H
#define HOOKER_H

#include <stdio.h>
#include <stdlib.h>

typedef void (* hook_func_t) ();

/**
 * @brief Structure representing a Hook.
 */
typedef struct Hook
{
    hook_func_t   original; // Pointer to the original function.
    hook_func_t   hook;     // Pointer to the hook function.
    int           enabled;  // Flag indicating if the hook is enabled (0 or 1).
    struct Hook * next;     // Pointer to the next Hook in the list.
}
Hook;

/**
 * @brief Structure representing a HookManager.
 */
typedef struct HookManager
{
    Hook * head;            // Pointer to the head of the Hook list.
}
HookManager;

/**
 * @brief Creates a new HookManager.
 * 
 * @return A pointer to the newly created HookManager, or NULL if allocation fails.
 */
HookManager *     create_hook_manager ()
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
Hook *     create_hook (HookManager *      manager, 
                        hook_func_t        original, 
                        hook_func_t        hook)
{
    if ( !original || !hook || !manager ) return NULL;

    Hook * new_hook = (Hook *) malloc(sizeof(Hook));
    if ( !new_hook ) return NULL;

    new_hook->original = original;
    new_hook->hook = hook;
    new_hook->enabled = 0;
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
int     apply_hook (Hook *     hook)
{
    if ( !hook || !hook->original || !hook->hook ) return -1;
    hook->enabled = 1;
    hook->original = hook->hook;
    return 0;
}

/**
 * @brief Restores the original function of a hook.
 * 
 * @param hook Pointer to the Hook to be restored.
 * 
 * @return 0 on success, -1 if the hook or its functions are NULL.
 */
int     restore_hook (Hook *     hook)
{
    if ( !hook || !hook->original || !hook->hook ) return -1;
    hook->enabled = 0;
    hook->hook = hook->original;
    return 0;
}

/**
 * @brief Enables a hook.
 * 
 * @param hook Pointer to the Hook to be enabled.
 */
void     enable_hook (Hook *     hook)
{
    if ( hook )
    {
        hook->enabled = 1;
    }
}

/**
 * @brief Disables a hook.
 * 
 * @param hook Pointer to the Hook to be disabled.
 */
void     disable_hook (Hook *     hook)
{
    if ( hook )
    {
        hook->enabled = 0;
    }
}

/**
 * @brief Frees the memory allocated for a Hook.
 * 
 * @param hook Pointer to the Hook to be freed.
 */
void     free_hook (Hook *     hook)
{
    if ( hook )
    {
        free(hook);
    }
}

/**
 * @brief Frees the memory allocated for a HookManager and all its hooks.
 * 
 * @param manager Pointer to the HookManager to be freed.
 */
void     free_hook_manager (HookManager *     manager)
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
void     list_hooks (HookManager *     manager)
{
    if ( !manager ) return;
    Hook * current = manager->head;
    while ( current )
    {
        printf("Hook: original=%p, hook=%p, enabled=%d\n", 
               (void *) current->original, (void *) current->hook, current->enabled);
        current = current->next;
    }
}

/**
 * @brief Executes all enabled hooks in the HookManager.
 * 
 * @param manager Pointer to the HookManager containing the hooks to be executed.
 */
void     execute_hooks (HookManager *     manager)
{
    if ( !manager ) return;
    Hook * current = manager->head;
    while ( current )
    {
        if ( current->enabled && current->hook )
        {
            current->hook();
        }
        current = current->next;
    }
}

#endif // HOOKER_H
