#ifndef CURRY_H
#define CURRY_H

#include <stddef.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

// - Macros - //
#ifndef CURRY_API
#define CURRY_API static
#endif


#ifndef cr_free
#define cr_free(ptr) free(ptr)
#endif

// - Types and Enums - //

typedef enum {
	CURRY_TERM_PLATFORM_UNKNOWN = 0,
	CURRY_TERM_PLATFORM_WINDOWS,
	CURRY_TERM_PLATFORM_LINUX,
	CURRY_TERM_PLATFORM_MACOS,
	CURRY_TERM_PLATFORM_BSD,
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
	cr_term_errors code;
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

#ifdef _WIN32
#include <windows.h>
#endif

typedef struct {
	int platform_handle;
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

CR_API cr_error cr_init(cr_context *ctx);

CR_API cr_error cr_set_attribute(cr_context *ctx, cr_attribute attribute);

CURRY_API cr_error cr_get_attribute(cr_context *ctx, cr_attribute attribute, void* value);

CR_API unsigned int cr_get_size(cr_context *ctx);

CR_API cr_error cr_set_size(cr_context* ctx, uint32_t width, uint32_t height);

CURRY_API cr_error cr_set_color(cr_context* ctx, const int rgb[3]);

#ifdef __cplusplus
}
#endif

#endif // CURRY_H

// Implementation section
#ifdef CURRY_IMPLEMENTATION
#ifdef __cplusplus
extern "C" {
#endif

CURRY_API cr_error cr_init(cr_context *ctx) {
	if (!ctx) {
		cr_error err = { CURRY_TERM_ERROR_INIT_FAILED, "Context pointer is null." };
		return err

#ifdef _WIN32
	#include <windows.h>
		ctx->platform = CURRY_TERM_PLATFORM_WINDOWS;
		HANDLE hConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		if (hConsoleHandle == INVALID_HANDLE_VALUE) {
			cr_error err = { CURRY_TERM_ERROR_INIT_FAILED, "Failed to get console handle." };
			return err;
		}
		ctx->state.platform_handle = (int)hConsoleHandle;
		ctx->state.initialized = true;
		
		// TODO: Set attributes using GetConsoleMode or SetConsoleMode
	#endif
}

#ifdef __cplusplus
}
#endif

#endif // CURRY_IMPLEMENTATION
