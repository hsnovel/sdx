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

#ifndef EXTRA_H
#define EXTRA_H

/* OS Detection */
#if defined(__WINDOWS__) || defined(__TOS_WIN__) || defined(__WIN32__) || defined(_WIN64) || defined(_WIN32) || defined(_WIN16)
#define _SDX_WINDOWS
#elif defined(__gnu_linux__) || defined(__unix__) || defined(__unix) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__bsdi__) || defined(__DragonFly__)
#define _SDX_UNIX
#elif defined(macintosh) || defined(Macintosh) || defined(__APPLE__) || defined(__MACH__)
#define _SDX_MACOS
#endif

#if defined(__gnu_linux__)
#define _SDX_LINUX
#endif

#if defined (__FreeBSD__)
#define _SDX_FREEBSD
#endif

#if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__bsdi__) || defined(__DragonFly__)
#define _SDX_BSD
#endif

/* Compiler Detection */
#if defined(__GNUC__) || defined(__GNUC_MINOR__) || defined(__GNUC_PATCHLEVEL__)
#define _SDX_GCC

#elif defined(__clang__) || defined(__clang_major__) || defined(__clang_minor__) || defined(__clang_patchlevel__) || defined(__clang_version__)
#define _SDX_CLANG

#elif defined(_MSC_VER) || defined(_MSC_FULL_VER) || defined(_MSC_FULL_VER) || defined(_MSC_BUILD)
#define _SDX_MSVC
#endif

/* Likely Unlikely */
#if defined(_SDX_GCC) || defined(_SDX_CLANG)
#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)
#endif

/* ALIGN */
#if defined(_SDX_GCC) || defined(_SDX_CLANG)
#define ALIGN(x) __attribute__((aligned(x)))
#elif defined(_SDX_MSVC)
#define ALIGN(x) __declspec(align(x))
#else
#define ALIGN(x)
#endif

#if defined(_SDX_GCC) || defined(_SDX_CLANG)
#define PREFETCH(x, ...) __builtin_prefetch(x, __VA_ARGS__);
#else
#define PREFETCH(x, ...)
#endif

#if defined(_SDX_GCC) || defined(_SDX_CLANG)
#define UNREACHABLE __builtin_unreachable()
#else
#defined UNREACHABLE
#endif

#if defined(_SDX_GCC) || defined(_SDX_CLANG)
#define FORCE_INLINE __attribute__((always_inline))
#elif defined (_SDX_MSVC)
#defined FORCE_INLINE __forceinline
#endif

/* Array Size */
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

#define KILOBYTE(x) ((x) / 1024)
#define MEGABYTE(x) ((x) / (1024 * 1024))
#define GIGABYTE(x) ((x) / (1024 * 1024 * 1024))
#define TERABYTE(x) ((x) / (1024 * 1024 * 1024 * 1024))

#define MIN(x, y)           (((x) < (y)) ?  (x) : (y))
#define MAX(x, y)           (((x) > (y)) ?  (x) : (y))
#define ABS(x)              (((x) <  0) ? -(x) : (x))
#define SWAP(a, b)          do { a ^= b; b ^= a; a ^= b; } while ( 0 )

#define SETBIT(x,p)     ((x)|(1<<(p)))
#define CLEARBIT(x,p)   ((x)&(~(1<<(p))))
#define GETBIT(x,p)     (((x)>>(p))&1)
#define TOGGLEBIT(x,p)  ((x)^(1<<(p)))

#include <errno.h>

// Allocate and print a formatted string to returned buffer
#ifndef xasprintf
#define xasprintf(format, ...) ({					\
	size_t needed = snprintf(NULL, 0, format, ##__VA_ARGS__) + 1; \
	char *buffer = (char *)malloc(needed);		\
	if (buffer == NULL)\
		NULL; \
	sprintf(buffer, format, ##__VA_ARGS__);		\
	buffer;						\
})
#endif

#endif
