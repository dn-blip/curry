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

enum cr_attribute {
	CURRY_TERM_ECHO_ON = 0, // Echo input
	CURRY_TERM_ECHO_OFF, // No echo input
	CURRY_TERM_CURSOR_VISIBLE, // Cursor visibility
	CURRY_TERM_MOUSE_ENABLED, // Mouse support enabled
	CURRY_TERM_MOUSE_DISABLED, // Mouse support disabled
	CURRY_TERM_COLOR_DEPTH, // Color depth of the terminal
};

typedef enum cr_attribute cr_attribute;	

// This following struct also holds platform-specific handles. There's an int here, for generallity.
struct cr_state {
	int platform_handle; // Platform-specific handle (e.g., file descriptor, window handle)
	bool initialized; // Is the terminal initialized?
	bool mouse_enabled; // Is mouse support enabled?
	bool mouse_pressed; // Is the mouse pressed?
	cr_attribute attributes; // Terminal attributes
	unsigned int size; // Size of the terminal in characters
	unsigned int color_depth; // Color depth of the terminal
	unsigned int width, height; // Width and height of the terminal in characters
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

CURRY_API cr_error cr_get_attribute(cr_context ctx, cr_attribute attribute, void* value);

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

	#ifdef _WIN32
	#include <windows.h>

	ctx->platform = CURRY_TERM_PLATFORM_WINDOWS;
	ctx->state.platform_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (ctx->state.platform_handle == INVALID_HANDLE_VALUE) {
		return (cr_error){CURRY_TERM_ERROR_INIT_FAILED, "Failed to get standard output handle"};
	}
	ctx->state.initialized = true;
	ctx->state.mouse_enabled = false;
	ctx->state.mouse_pressed = false;
	ctx->state.attributes = CURRY_TERM_ECHO_ON; // Default attribute

	#elif defined(__linux__)
	#include <unistd.h>
	#include <termios.h>

	ctx->state.platform_handle = STDOUT_FILENO; // Standard output file descriptor
	if (ctx->state.platform_handle < 0) {
		return (cr_error){CURRY_TERM_ERROR_INIT_FAILED, "Failed to get standard output file descriptor"};
	}
	ctx->state.initialized = true;
	ctx->state.mouse_enabled = false;
	ctx->state.mouse_pressed = false;
	ctx->state.attributes = CURRY_TERM_ECHO_ON; // Default attribute

	ctx->platform = CURRY_TERM_PLATFORM_LINUX;


	#elif defined(__APPLE__)
	ctx->platform = CURRY_TERM_PLATFORM_MACOS;
	#elif defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)
	ctx->platform = CURRY_TERM_PLATFORM_BSD;
	#else
	ctx->platform = CURRY_TERM_PLATFORM_UNKNOWN;
		return (cr_error){CURRY_TERM_ERROR_UNKNOWN_PLATFORM, "Unknown platform"};
	#endif

	return (cr_error){CURRY_TERM_SUCCESS, "Initialization successful"};

}
#endif // __cplusplus
#endif // CURRY_IMPLEMENTATION