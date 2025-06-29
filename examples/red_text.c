/*
====== Demo Description ======
NAME: red_text.c

DESCRIPTION: This example demonstrates how to use curry's API to set the
terminal color to red, without using ANSI escape codes.

WRITTEN BY: Daniel Nzekwe "dn-blip"

========= Usage ===============


Compile this code with a C compiler, ensuring that the curry library is linked.
This code also depends on kernel32.lib on Windows, so makke sure that's linked,
or use the C++ Console Application template in Visual Studio.



====== Functions Used ========
''cr_init'' to initialize the terminal context,
''cr_set_color'' to set the terminal color,
and ''cr_printf'' to print a message in the specified color.


*/
#define CURRY_IMPLEMENTATION
#include "../curry.h"

#include <stdio.h>

int main(void) {
    cr_context ctx;
    cr_error err = cr_init(&ctx);

    // First, check if initialization was successful
    if (err.code != CURRY_TERM_SUCCESS) {
        fprintf(stderr, "Error initializing terminal: %s\n", err.message);
        return 1;
    }
    // Then, set some attributes
    int rgb[3] = {255, 0, 0}; // Red color
    err = cr_set_color(&ctx, rgb);
    if (err.code != CURRY_TERM_SUCCESS) {
        fprintf(stderr, "Error setting color: %s\n", err.message);
        return 1;
    }
    // Finally, print a message in red
    cr_printf("This should be red, and we didn't use ANSI codes!\n");
}
