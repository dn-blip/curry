#ifndef CURRY_H
#define CURRY_H

#include <stddef.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

// - Macros - //
#ifndef CURRY_API
	#define CURRY_API extern
#elif defined(CURRY_STATIC)
	#define CURRY_API static
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
	union {
#ifdef _WIN32
		HANDLE win_handle;
#endif
		int fd_handle;
	} platform_handle;
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

CURRY_API cr_error cr_set_attribute(cr_context *ctx, cr_attribute attr, bool value);

CURRY_API cr_error cr_get_attribute(cr_context *ctx, cr_attribute attr, bool *value);

CURRY_API cr_error cr_set_size(cr_context* ctx, uint32_t width, uint32_t height);

CURRY_API unsigned int cr_get_size(cr_context *ctx);

CURRY_API cr_error cr_set_color(cr_context* ctx, const uint8_t rgb[3]);

CURRY_API cr_state* cr_get_state(cr_context *ctx);

CURRY_API cr_error cr_printf(const char *format, ...);

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
		cr_error err = { CURRY_TERM_ERROR_INIT_FAILED, "[curry] FATAL ERROR: Context pointer is NULL." };
		return err;
	}

#ifdef _WIN32
	#include <windows.h>
	ctx->platform = CURRY_TERM_PLATFORM_WINDOWS;
	HANDLE hConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hConsoleHandle == INVALID_HANDLE_VALUE) {
		cr_error err = { CURRY_TERM_ERROR_INIT_FAILED, "[curry] FATAL ERROR: Failed to get console handle." };
		return err;
	}
	ctx->state.platform_handle.win_handle = hConsoleHandle;
	ctx->state.initialized = true;
#endif

#ifdef __linux__
	#include <unistd.h>
	ctx->platform = CURRY_TERM_PLATFORM_LINUX;
	ctx->state.platform_handle.fd_handle = STDOUT_FILENO;
	ctx->state.initialized = true;
#endif

#ifdef __APPLE__
	#include <unistd.h>
	ctx->platform = CURRY_TERM_PLATFORM_MACOS;
	ctx->state.platform_handle.fd_handle = STDOUT_FILENO;
	ctx->state.initialized = true;
#endif
#ifdef __FreeBSD__
	// TODO: FreeBSD support
#endif

	cr_error err = { CURRY_TERM_SUCCESS, "[curry] Initialization successful." };
	return err;
}


CURRY_API cr_error cr_set_attribute(cr_context *ctx, cr_attribute attr, bool value) {
	if (!ctx) {
		cr_error err = { CURRY_TERM_ERROR_INVALID_STATE, "[curry] FATAL ERROR: Context pointer is NULL." };
		return err;
	}

	switch(attr) {
		// TODO: Implement actual attribute setting logic for each platform.
	}

	cr_error err = { CURRY_TERM_SUCCESS, "[curry] Attribute set successfully." };
	return err;
}

// Stub implementation for cr_get_attribute
CURRY_API cr_error cr_get_attribute(cr_context *ctx, cr_attribute attr, bool *value) {
	if (!ctx || !value) {
		cr_error err = { CURRY_TERM_ERROR_INVALID_STATE, "[curry] Context or value pointer is NULL." };
		return err;
	}

	switch(attr) {
		// Most of the time, these attributes are always on.
		case CURRY_TERM_ECHO_ON:
		case CURRY_TERM_MOUSE_DISABLED:
		case CURRY_TERM_CURSOR_VISIBLE:
			*value = true;
			break;

		// These attributes are typically off by default.
		case CURRY_TERM_ECHO_OFF:
		case CURRY_TERM_MOUSE_ENABLED:
			*value = false; 
			break;

		case CURRY_TERM_COLOR_DEPTH:
			*value = ctx->state.color_depth;
			break;
	}

	cr_error err = { CURRY_TERM_SUCCESS, "[curry] Attribute retrieved successfully." };
	return err;
}

// Stub implementation for cr_get_size
CURRY_API unsigned int cr_get_size(cr_context *ctx) {
	if (!ctx) return 0;
	return ctx->state.size;
}

// Stub implementation for cr_set_size
CURRY_API cr_error cr_set_size(cr_context* ctx, uint32_t width, uint32_t height) {
	if (!ctx) {
		cr_error err = { CURRY_TERM_ERROR_INVALID_STATE, "[curry] Context pointer is NULL." };
		return err;
	}
	ctx->state.width = width;
	ctx->state.height = height;
	ctx->state.size = width * height;
	cr_error err = { CURRY_TERM_SUCCESS, "[curry] Size set successfully." };
	return err;
}

// Stub implementation for cr_set_color
CURRY_API cr_error cr_set_color(cr_context* ctx, const uint8_t rgb[3]) {
	if (!ctx) {
		cr_error err = { CURRY_TERM_ERROR_INVALID_STATE, "[curry] Context pointer is null." };
		return err;
	}
	
	if (!rgb) {
		cr_error err = { CURRY_TERM_ERROR_INVALID_COLOR, "[curry] RGB color array is null." };
		return err;
	}

	if (ctx->platform == CURRY_TERM_PLATFORM_WINDOWS) {
		// Windows-specific color setting logic
	}
	if (ctx->platform == CURRY_TERM_PLATFORM_LINUX || ctx->platform == CURRY_TERM_PLATFORM_MACOS 
		|| ctx->platform == CURRY_TERM_PLATFORM_BSD) {
		// Convert RGB to ANSI escape code for Linux/MacOS/FreeBSD
		printf("\033[38;2;%d;%d;%dm", rgb[0], rgb[1], rgb[2]);
		fflush(stdout);
	}

	cr_error err = { CURRY_TERM_SUCCESS, "[curry] Color set successfully." };
	return err;
}

CURRY_API cr_state* cr_get_state(cr_context *ctx) {
	if (!ctx) return NULL;
	return &ctx->state;
}

CURRY_API cr_error cr_printf(const char *format, ...) {
	if (!format) {
		cr_error err = { CURRY_TERM_ERROR_INVALID_STATE, "[curry] Format string is null." };
		return err;
	}

	va_list args;
	va_start(args, format);
	
	// TODO: Implement platform-specific printing logic.
	vprintf(format, args);

	va_end(args);

	cr_error err = { CURRY_TERM_SUCCESS, "[curry] Print successful." };
	return err;
}

#ifdef __cplusplus
}
#endif

#endif // CURRY_IMPLEMENTATION
