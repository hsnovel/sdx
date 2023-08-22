// This file is a part of std libraries
// https://github.com/xcatalyst/std
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

#ifndef SYSTEM_H
#define SYSTEM_H

#include "extra.h"
#include <string.h>
#include <ctype.h>
#ifdef _STD_WINDOWS
#include <windows.h>
#elif defined(_STD_UNIX)
#include <stddef.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include <sys/reboot.h>
#endif

struct sys_meminfo {
	size_t capacity;
	size_t available;
};

int sys_is_debugger_attached(void);
int sys_restart(void);
int sys_poweroff(void);
int sys_get_memory_info(struct sys_meminfo *info);
int sys_get_num_cpu_core(void);
int sys_get_num_cpu_core_avail(void);

#endif
