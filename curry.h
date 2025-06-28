#ifndef CURRY_H
#define CURRY_H

#include <stdbool.h>
#include <stdint.h>

// - Macros - //
#ifndef CURRY_API
#define CURRY_API static
#endif

#ifndef curry_malloc
#define curry_malloc(sz) malloc(sz)
#endif

// - Types and Enums - //

typedef enum {
	CURRY_TERM_PLATFORM_UNKNOWN = 0,
	CURRY_TERM_PLATFORM_WINDOWS,
	CURRY_TERM_PLATFORM_LINUX,
	CURRY_TERM_PLATFORM_MACOS,
	CURRY_TERM_PLATFORM_BSD
} cr_platform;

typedef enum {
	CURRY_TERM_SUCCESS = 0,
	CURRY_TERM_ERROR_INIT_FAILED,
	CURRY_TERM_ERROR_INVALID_STATE,
	CURRY_TERM_ERROR_INVALID_SIZE,
	CURRY_TERM_ERROR_INVALID_COLOR,
	CURRY_TERM_ERROR_UNKNOWN_PLATFORM
} cr_term_errors;

typedef struct {
	cr_term_errors short_form;
	char message[512];
} cr_error;

typedef enum {
	CURRY_TERM_ECHO_ON = 0,
	CURRY_TERM_ECHO_OFF,
	CURRY_TERM_CURSOR_VISIBLE,
	CURRY_TERM_MOUSE_ENABLED,
	CURRY_TERM_MOUSE_DISABLED,
	CURRY_TERM_COLOR_DEPTH,
} cr_attribute;

typedef struct {
	void* platform_handle;
	bool initialized;
	bool mouse_enabled;
	bool mouse_pressed;
	cr_attribute attributes;
	unsigned int size;
	unsigned int color_depth;
	unsigned int width, height;
} cr_state;

typedef struct {
	cr_platform platform;
	cr_state state;
	int width, height;
} cr_context;

#ifdef __cplusplus
extern "C" {
#endif

// - Functions - //

CURRY_API cr_error cr_init(cr_context *ctx);

CURRY_API cr_error cr_set_attribute(cr_context *ctx, cr_attribute attribute);

CURRY_API cr_error cr_get_attribute(cr_context *ctx, cr_attribute attribute, void* value);

CURRY_API unsigned int cr_get_size(cr_context *ctx);

CURRY_API cr_error cr_set_size(cr_context* ctx, uint32_t width, uint32_t height);

CURRY_API cr_error cr_set_color(cr_context* ctx, const uint8_t rgb[3]);

#ifdef __cplusplus
}
#endif

#endif // CURRY_H

// Implementation section
#ifdef CURRY_IMPLEMENTATION

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

CURRY_API cr_error cr_init(cr_context *ctx) {
	if (!ctx) {
		cr_error err = { CURRY_TERM_ERROR_INIT_FAILED, "The pointer to cr_context is NULL." };
		return err;

	#ifdef _WIN32
	#include <windows.h>
		ctx->platform = CURRY_TERM_PLATFORM_WINDOWS;
		HANDLE hConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		if (hConsoleHandle == INVALID_HANDLE_VALUE) {
			cr_error err = { CURRY_TERM_ERROR_INIT_FAILED, "Failed to obtain a suitable HANDLE." };
			return err;
		}
		ctx->state.platform_handle = (void*)hConsoleHandle;
		ctx->state.initialized = true;
		cr_error success = {CURRY_TERM_SUCCESS, "No errors occured."}
	#endif
}

#ifdef __cplusplus
}
#endif

#endif // CURRY_IMPLEMENTATION
