#ifndef EXTRA_H
#define EXTRA_H

/* OS Detection */
#if defined(__WINDOWS__) || defined(__TOS_WIN__) || defined(__WIN32__) || defined(_WIN64) || defined(_WIN32) || defined(_WIN16)
#define _STD_WINDOWS
#elif defined(__gnu_linux__) || defined(__unix__) || defined(__unix) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__bsdi__) || defined(__DragonFly__)
#define _STD_UNIX
#elif defined(macintosh) || defined(Macintosh) || defined(__APPLE__) || defined(__MACH__)
#define _STD_MACOS
#endif

#if defined(__gnu_linux__)
#define _STD_LINUX
#endif

#if defined (__FreeBSD__)
#define _STD_FREEBSD
#endif

#if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__bsdi__) || defined(__DragonFly__)
#define _STD_BSD
#endif

/* Compiler Detection */
#if defined(__GNUC__) || defined(__GNUC_MINOR__) || defined(__GNUC_PATCHLEVEL__)
#define _STD_GCC

#elif defined(__clang__) || defined(__clang_major__) || defined(__clang_minor__) || defined(__clang_patchlevel__) || defined(__clang_version__)
#define _STD_CLANG

#elif defined(_MSC_VER) || defined(_MSC_FULL_VER) || defined(_MSC_FULL_VER) || defined(_MSC_BUILD)
#define _STD_MSVC
#endif

/* Likely Unlikely */
#if defined(_STD_GCC) || defined(_STD_CLANG)
#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)
#endif

/* ALIGN */
#if defined(_STD_GCC) || defined(_STD_CLANG)
#define ALIGN(x) __attribute__((aligned(x)))
#elif defined(_STD_MSVC)
#define ALIGN(x) __declspec(align(x))
#else
#define ALIGN(x)
#endif

#if defined(_STD_GCC) || defined(_STD_CLANG)
#define PREFETCH(x, ...) __builtin_prefetch(x, __VA_ARGS__);
#else
#define PREFETCH(x, ...)
#endif

#if defined(_STD_GCC) || defined(_STD_CLANG)
#define UNREACHABLE __builtin_unreachable()
#else
#defined UNREACHABLE
#endif

/* Array Size */
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]) + __is_array(arr))
#define __is_array(a) BUILD_BUG_ON_ZERO(__same_type((a), &(a)[0]))
#define __same_type(a, b) __builtin_types_compatible_p(typeof(a), typeof(b))

#define KILOBYTE(x) ((x) / 1024)
#define MEGABYTE(x) ((x) / (1024 * 1024))
#define GIGABYTE(x) ((x) / (1024 * 1024 * 1024))
#define TERABYTE(x) ((x) / (1024 * 1024 * 1024 * 1024))

#endif
