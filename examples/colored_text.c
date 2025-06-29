/*
====== Demo Description ======================================================
NAME: colored_text.c

DESCRIPTION: This example demonstrates how to use curry's API to set the
terminal color to red and then green, without using ANSI escape codes directly.

WRITTEN BY: Daniel Nzekwe "dn-blip"

========= Usage ==============================================================


Compile this code with a C compiler, ensuring that the curry library is linked.
This code also depends on kernel32.lib on Windows, so makke sure that's linked,
or use the C++ Console Application template in Visual Studio.



====== Functions Used ========================================================
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
    // Then, set some attributes like color.
    // Here we set the color to red using an RGB array.
    // As a fun experiment, you can change the RGB values to see different colors!
    uint8_t rgb[3] = {255, 0, 0};
    err = cr_set_color(&ctx, rgb);
    if (err.code != CURRY_TERM_SUCCESS) {
        fprintf(stderr, "Error setting color: %s\n", err.message);
        return 1;
    }
    // Finally, print a message in red
    cr_printf("This should be red!\n");

    uint8_t new_color[3] = {0, 255, 0}; // Change to green
    err = cr_set_color(&ctx, new_color);
    if (err.code != CURRY_TERM_SUCCESS) {
        fprintf(stderr, "Error setting color: %s\n", err.message);
        return 1;
    }
    // Print another message in green
    cr_printf("This should be green!\n");
}
