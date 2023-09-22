// This file is a part of sdx libraries
// https://github.com/hsnovel/sdx
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

#ifndef LOG_H
#define LOG_H

#include <stdio.h>

enum { LOG_NOTAG, LOG_OK, LOG_TRACE, LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR, LOG_FATAL };

void _log(int level, int line, const char* file_name, const char* args, ...);

void log_disable_thread_safe();
void log_enable_thread_safe();
void log_set_quiet(unsigned int quiet);
void log_set_level(int level);
void log_append_fp(FILE* file_descriptor);
void log_detach_fp();
int log_append_file(const char *path);

/*
 * Cannot get line numbers inside functions without specifiying them as
 * an argument in the function call. You have to #define them otherwise
 * you have to pass __LINE__ and __FILE__ to functions which is an
 * extra typing...
 */

#ifdef LOG_DISABLE_NOTAG
#define log_notag(...)
#else
#define log_notag(...) _log(LOG_NOTAG, __LINE__, __FILE__, __VA_ARGS__)
#endif

#ifdef LOG_DISABLE_OK
#define log_ok(...)
#else
#define log_ok(...) _log(LOG_OK, __LINE__, __FILE__, __VA_ARGS__)
#endif

#ifdef LOG_DISABLE_TRACE
#define log_trace(...)
#else
#define log_trace(...) _log(LOG_TRACE, __LINE__, __FILE__, __VA_ARGS__)
#endif

#ifdef LOG_DISABLE_DEBUG
#define log_debug(...)
#else
#define log_debug(...) _log(LOG_DEBUG, __LINE__, __FILE__, __VA_ARGS__)
#endif

#ifdef LOG_DISABLE_INFO
#define log_info(...)
#else
#define log_info(...)  _log(LOG_INFO, __LINE__, __FILE__, __VA_ARGS__)
#endif

#ifdef LOG_DISABLE_WARN
#define log_warn(...)
#else
#define log_warn(...)  _log(LOG_WARN, __LINE__, __FILE__, __VA_ARGS__)
#endif

#ifdef LOG_DISABLE_ERROR
#define log_error(...)
#else
#define log_error(...) _log(LOG_ERROR, __LINE__, __FILE__, __VA_ARGS__)
#endif

#ifdef LOG_DISABLE_FATAL
#define log_fatal(...)
#else
#define log_fatal(...) _log(LOG_FATAL, __LINE__, __FILE__, __VA_ARGS__)
#endif

#endif
