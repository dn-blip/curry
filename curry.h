#ifndef CURRY_H
#define CURRY_H

#ifdef __cplusplus

extern "C" {
#include <stdbool.h>

// - Macros - //
#ifndef CURRY_API
#define CURRY_API static
#endif

#ifndef curry_malloc(sz)
#define curry_malloc(sz) malloc(sz)
#endif

// - Types and Enums - //

enum cr_platform {
	CURRY_TERM_PLATFORM_UNKNOWN = 0,
	CURRY_TERM_PLATFORM_WINDOWS,
	CURRY_TERM_PLATFORM_LINUX,
	CURRY_TERM_PLATFORM_MACOS,
	CURRY_TERM_PLATFORM_BSD
};

typedef enum cr_platform cr_platform;

enum cr_term_errors {
	CURRY_TERM_SUCCESS = 0,
	CURRY_TERM_ERROR_INIT_FAILED,
	CURRY_TERM_ERROR_INVALID_STATE,
	CURRY_TERM_ERROR_INVALID_SIZE,
	CURRY_TERM_ERROR_INVALID_COLOR,
	CURRY_TERM_ERROR_UNKNOWN_PLATFORM
};

struct cr_error {
	cr_term_errors short_form;
	const char message[512];
};

typedef struct cr_error cr_error;

// This following struct also holds platform-specific handles. There's an int here, for generallity.
struct cr_state {
	// curry_term_color_t fg_color, bg_color, 
	int cursor_x, cursor_y;
	bool cursor_visible, cursor_blinking, cursor_block, mouse_enabled, mouse_pressed;
	bool vt_enabled, vt100_enabled, vt200_enabled, vt300_enabled, vt400_enabled;
	bool input_echo;
	int handle; // Platform-specific handle
};

typedef struct cr_state cr_state;

// The context hold properties of the terminal, which should be filled using curry_term_init().
// The context passes information about the terminal to library functions. 
// It also holds color information, which is used to determine how to render colors in the terminal.
struct cr_context {
	cr_platform platform;
	cr_state state;
	int width, height; 
};

typedef struct cr_context cr_context;

// - Functions - //

CURRY_API cr_error cr_init(cr_context *ctx);

CURRY_API cr_state cr_get_state(cr_context ctx);

CURRY_API cr_error cr_set_state(cr_context* ctx, cr_state state);

CURRY_API unsigned int cr_get_size(cr_context ctx);

CURRY_API cr_error cr_set_size(cr_context* ctx, uint32_t width, uint32_t height);

CURRY_API cr_error cr_set_color(cr_context* ctx, const int rgb[3]);

// curry_term_state_t curry_term_reset(curry_term_context* ctx);


} // extern "C"
#endif // __cplusplus
#endif // CURRY_H

#ifndef CURRY_IMPLEMENTATION
#define CURRY_IMPLEMENTATION

#ifdef __cplusplus
extern "C" {
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>

CURRY_API cr_error cr_init(cr_context *ctx) {
	// Initialize the context based on the platform
	if (!ctx) {
		return (cr_error){CURRY_TERM_ERROR_INIT_FAILED, "Context is NULL"};
	}

	ctx->platform = CURRY_TERM_PLATFORM_UNKNOWN; // Default to unknown
	ctx->state = (cr_state){0}; // Initialize state with default values
	ctx->width = 80; // Default terminal width
	ctx->height = 24; // Default terminal height

	// Platform-specific initialization can be added here

	return (cr_error){CURRY_TERM_SUCCESS, "Initialization successful"};

}
#endif // __cplusplus
#endif // CURRY_IMPLEMENTATION