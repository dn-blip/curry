#include "../curry.h"
#define CURRY_IMPLEMENTATION
#include <stdio.h>

// This demo shows a basic working example with curry's terminal API.
int main(void) {
    // Initialize a context, which holds some info about the terminal
    // and platform. it has to be initialized seperately, as most functions either return
    // cr_error, the thing that was to be returned, or void.
    // cr_init() also fills the context with useful info about OS specifics.
    cr_context *ctx;
    cr_init(ctx);

    cr_state initial_state = cr_get_state(*ctx);

    // To set colors, you kinda have to pass the memory for the color buffer.

    unsigned int rgb[3] = {255, 0, 0};
    cr_error err = cr_set_color(ctx, rgb);

    if (err != CR_TERM_SUCCESS) {
        fprintf(stderr, "Error setting color: %s\n", err.message);
        return 1;
    }

    printf("This text should be red!\n");
}