#ifndef CURRY_H
#define CURRY_H

#ifdef __cplusplus

extern "C" {
#include <stdbool.h>


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
	CURRY_TERM_ERROR_NONE = 0,
	CURRY_TERM_ERROR_INIT_FAILED,
	CURRY_TERM_ERROR_INVALID_STATE,
	CURRY_TERM_ERROR_INVALID_SIZE,
	CURRY_TERM_ERROR_INVALID_COLOR,
	CURRY_TERM_ERROR_UNKNOWN_PLATFORM
};

struct cr_error {
	cry_term_errors short_form;
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

cr_error cr_init(curry_term_context *ctx);

cr_state cr_state(cr_context* ctx);

cr_state cr_state(cr_context* ctx, curry_term_state_t state);

unsigned int cr_size(cr_context* ctx);

cr_error cr_set_size(cr_context* ctx, uint32_t width, uint32_t height);

cr_error cr_set_color(cr_context* ctx, const int rgb[3]);

// curry_term_state_t curry_term_reset(curry_term_context* ctx);


} // extern "C"
#endif // __cplusplus
#endif // curry.h

#ifndef CURRY_IMPLEMENTATION
#define CURRY_IMPLEMENTATION

#ifdef __cplusplus
extern "C" {
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>

}
#endif // __cplusplus
#endif // CURRY_IMPLEMENTATION