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

#ifndef MEM_DEBUG_H
#define MEM_DEBUG_H

#include <stdlib.h>
#include <stddef.h>

void mem_debug_init();
void mem_debug_print();
void *mem_debug_malloc(size_t num, size_t line, char *file);
void *mem_debug_realloc(void *ptr, size_t newsize, size_t line, char *file);
void mem_debug_free(void *ptr, size_t line, char *file);

#if defined(MEM_DEBUG_ENABLE) && !defined(MEM_DEBUG_DISABLE)
#define malloc(x) mem_debug_malloc(x, __LINE__, __FILE__)
/*
 * #define realloc(x,y ) mem_debug_realloc(x, y, __LINE__, __FILE__)
 */
#define free(x) mem_debug_free(x, __LINE__, __FILE__)
#endif

#ifndef MEM_DEBUG_OUT_OF_BOUNDS_NUM
#define MEM_DEBUG_OUT_OF_BOUNDS_NUM 256
#endif

#endif
