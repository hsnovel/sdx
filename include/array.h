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

#ifndef ARRAY_H
#define ARRAY_H

/*
 * on fail 0 is returned, otherwise
 * 1 is returned.
 */

#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define ARRAY_INITIAL_CAP 256

struct array {
	size_t cap;		// in bytes
	size_t index;		// counter in numbers
	size_t itemsize;	// in bytes
	unsigned char *data;	// actualy data
};

int array_init(struct array *array, int size);
int array_push(struct array *array, void *data);
void *array_alloc(struct array *array);
int array_free_item(struct array *array, size_t index);
void *array_get(struct array *array, size_t index);
void array_replace_item(struct array *array, size_t index, void *data);
void array_pop(struct array *array);
void array_free(struct array *array);

#endif // ARRAY_H
