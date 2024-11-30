#include <Hooking.h>

void original_function()
{
    printf("Original function called.\n");
}

void hook_function()
{
    printf("Hook function called instead of original function.\n");
}

int main()
{
    HookManager * manager = create_hook_manager();
    if ( !manager )
    {
        fprintf(stderr, "Failed to create HookManager.\n");
        return EXIT_FAILURE;
    }

    Hook * hook = create_hook(manager, original_function, hook_function);
    if ( !hook )
    {
        fprintf(stderr, "Failed to create hook.\n");
        free_hook_manager(manager);
        return EXIT_FAILURE;
    }

    if ( apply_hook(hook) != 0 )
    {
        fprintf(stderr, "Failed to apply hook.\n");
        free_hook(hook);
        free_hook_manager(manager);
        return EXIT_FAILURE;
    }

    original_function();

    if ( restore_hook(hook) != 0 )
    {
        fprintf(stderr, "Failed to restore original function.\n");
        free_hook(hook);
        free_hook_manager(manager);
        return EXIT_FAILURE;
    }

    original_function();

    free_hook(hook);
    free_hook_manager(manager);

    return EXIT_SUCCESS;
}
