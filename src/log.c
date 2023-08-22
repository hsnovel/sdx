// This file is a part of sdx libraries
// https://github.com/xcatalyst/sdx
//
// Licensed under the MIT License <http://opensource.org/licenses/MIT>.
//
// MIT License
// Copyright (c) Çağan Korkmaz <cagankorkmaz35@gmail.com>
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "log.h"

#include <stdarg.h>
#include <time.h>
#ifdef _MSC_VER
#include <windows.h>
#else
#include <errno.h>
#include <pthread.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

static struct {
	int level;
	unsigned int quiet;
	FILE* terminal_descriptor;
	FILE* file_descriptor;
#ifdef _MSC_VER
	CRITICAL_SECTION mutex;
#else
	pthread_mutex_t mutex;
#endif
	unsigned int thread_safe_initialized;
} _log_global_settings = {
	.level = LOG_NOTAG,
	.quiet = 0,
	.terminal_descriptor = 0,
	.file_descriptor = 0,
	.thread_safe_initialized = 0,
};

static const char* _log_level_strings[] = {
	"NOTAG ", "OK ", "TRACE ", "DEBUG ", "INFO ", "WARN ", "ERROR ", "FATAL "
};

#if defined(LOG_ENABLE_BOLD_COLORS) && !defined(LOG_DISABLE_COLORS)
static const char* _log_level_colors[] = {
	"\x1b[1;39m", "\x1b[1;32m", "\x1b[1;35m", "\x1b[1;34m", "\x1b[1;32m", "\x1b[1;33m", "\x1b[1;31m", "\x1b[1;31m"
};
#elif !defined(LOG_DISABLE_COLORS)
static const char* _log_level_colors[] = {
	"\x1b[39m", "\x1b[32m", "\x1b[35m", "\x1b[34m", "\x1b[32m", "\x1b[33m", "\x1b[31m", "\x1b[31m"
};
#endif

#if defined (LOG_DISABLE_COLORS)
static const char* _log_level_colors[] = {
	"\033[0m", "\033[0m", "\033[0m", "\033[0m", "\033[0m", "\033[0m", "\033[0m", "\033[0m"
};
#endif

void log_disable_thread_safe()
{
	_log_global_settings.thread_safe_initialized = 0;
}

void log_enable_thread_safe()
{
	if (_log_global_settings.thread_safe_initialized == 0) {
#ifdef _MSC_VER
		InitializeCriticalSection(&_log_global_settings.mutex);
#else
		pthread_mutex_init(&_log_global_settings.mutex, NULL);
#endif
	}
	_log_global_settings.thread_safe_initialized = 1;
}

void log_set_quiet(unsigned int quiet)
{
	_log_global_settings.quiet = quiet;
}

void log_set_level(int level)
{
	_log_global_settings.level = level;
}

int log_append_file(const char *path)
{
#ifndef _WIN32
	FILE *f = fopen(path, "a");
	if (f == NULL) {
		return errno;
	}
	_log_global_settings.file_descriptor = f;
	return 0;
#else
	FILE *f;
	errno_t err;
	if ((err = fopen_s(&f, path, "a")) != 0) {
		return (int)err;
	}
	_log_global_settings.file_descriptor = f;
	return 0;
#endif
}

void log_append_fp(FILE* file_descriptor)
{
	_log_global_settings.file_descriptor = file_descriptor;
}

void log_detach_fp()
{
	_log_global_settings.file_descriptor = NULL;
}

void _log_handle_config(int level, int *scoped_level)
{
	/* Mutex */
	if (_log_global_settings.thread_safe_initialized) {
#ifdef _MSC_VER
		EnterCriticalSection(&_log_global_settings.mutex);
#else
		pthread_mutex_lock(&_log_global_settings.mutex);
#endif
	}

	/* Quiet Mode */
	if (_log_global_settings.quiet == 0)
		_log_global_settings.terminal_descriptor = stderr;
	else
		_log_global_settings.terminal_descriptor = stdout;

	/* Min Level */
	if (level >= _log_global_settings.level)
		*scoped_level = level;
	else
		*scoped_level = _log_global_settings.level;
}

/* Sadly it's impossible to get rid of preprocessor macros for this level of customization */
static inline void _log_print_file_info(FILE *fd, int scoped_level, int line, const char *file_name)
{
#if !defined (_MSC_VER) /************** UNIX **************/
	struct tm* current_time;
	time_t tx = time(NULL);
	char date[16];
	current_time = localtime(&tx);
	date[strftime(date, sizeof(date), "%H:%M:%S", current_time)] = '\0';
#if !defined(LOG_RELEASE)
	if (scoped_level == LOG_NOTAG) {
		fprintf(fd, "%s \x1b[90m%s:%d:\x1b[0m",
		date, file_name, line);

	} else {
		fprintf(fd, "%s %s%-5s\x1b[0m \x1b[90m%s:%d:\x1b[0m",
			date, _log_level_colors[scoped_level], _log_level_strings[scoped_level], file_name, line);
	}
#else
	if (scoped_level != LOG_NOTAG) {
		fprintf(fd, "%s%s: \x1b[0m", _log_level_colors[scoped_level], _log_level_strings[scoped_level]);
	}
#endif

#else /************** WINDOWS **************/
	SYSTEMTIME LocalTime;
	GetLocalTime(&LocalTime);


#if !defined(LOG_RELEASE)
	if (scoped_level == LOG_NOTAG) {
		fprintf(fd, "%d:%d:%d \x1b[90m%s:%d:\x1b[0m",
			LocalTime.wHour, LocalTime.wMinute, LocalTime.wSecond, file_name, line);
	} else {
		fprintf(fd, "%d:%d:%d %s%-5s\x1b[0m \x1b[90m%s:%d:\x1b[0m",
			LocalTime.wHour, LocalTime.wMinute, LocalTime.wSecond, _log_level_colors[scoped_level], _log_level_strings[scoped_level], file_name, line);

	}
#else
	if (scoped_level != LOG_NOTAG) {
		fprintf(fd, "%s%s \x1b[0m", _log_level_colors[scoped_level], _log_level_strings[scoped_level]);
	}
#endif

#endif /* _MSC_VER */
}

void _log(int level, int line, const char* file_name, const char* args, ...)
{
	int scoped_level;
	va_list variadic_list;

	_log_handle_config(level, &scoped_level);

	va_start(variadic_list, args);

#ifdef LOG_DISABLE_ERROR_STRING
	_log_print_file_info(_log_global_settings.terminal_descriptor, LOG_NOTAG, line, file_name);
#else
	/* Print the file information*/
	_log_print_file_info(_log_global_settings.terminal_descriptor, scoped_level, line, file_name);
#endif

	/* Print user string */
	vfprintf(_log_global_settings.terminal_descriptor, args, variadic_list);
	putc('\n', _log_global_settings.terminal_descriptor);

	/* Print to file */
#if !defined LOG_RELEASE
	if (_log_global_settings.file_descriptor) {
		va_list file_list;
		va_copy(file_list, variadic_list);
		_log_print_file_info(_log_global_settings.file_descriptor, scoped_level, line, file_name);

		vfprintf(_log_global_settings.file_descriptor, args, file_list);
		putc('\n', _log_global_settings.file_descriptor);
		fflush(_log_global_settings.file_descriptor);
	}
#endif

	/*
	 * Normaly terminals are line buffered but we obviously:
	 *
	 *   - cannot rely on that as stream buffering mode might be
	 *     set to something different by the programmer
	 *   - cannot guarantee that the terminal will flush on a new line
	 *
	 * So we flush the file stream to prevent loss of logs.
	 */

	fflush(_log_global_settings.terminal_descriptor);

	va_end(variadic_list);
	if (_log_global_settings.thread_safe_initialized) {
		pthread_mutex_unlock(&_log_global_settings.mutex);
	}
}
