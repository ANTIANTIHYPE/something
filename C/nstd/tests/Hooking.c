#include <Hooking.h>

VOID     original_function ( VOID )
{
    printf("Original function called.\n");
}

VOID     hook_function ( VOID )
{
    printf("Hook function called instead of original function.\n");
}

VOID     error ( CSTR              MSG, 
                 BOOL              FREE, 
                 HookManager *     manager )
{
    if ( !manager ) goto hello;
    if ( FREE ) free_hook_manager(manager);

hello:
    fprintf(stderr, MSG);
    exit(EXIT_FAILURE);
}

int     main ( VOID )
{
    HookManager * manager = create_hook_manager();
    if ( !manager ) error("Failed to create HookManager.\n", FALSE, NULL);

    Hook * hook = create_hook(manager, original_function, hook_function);
    if ( !hook ) error("Failed to create hook.\n", TRUE, manager);

    if ( apply_hook(hook) != 0 ) error("Failed to apply hook.\n", TRUE, manager);

    list_hooks(manager);

    original_function();

    if ( restore_hook(hook) != 0 ) error("Failed to restore original function.\n", TRUE, manager);

    list_hooks(manager);

    original_function();

    free_hook_manager(manager);

    return EXIT_SUCCESS;
}
