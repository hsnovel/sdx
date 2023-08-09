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

#ifndef ARRAY_H
#define ARRAY_H

//----------------------------------------------------------------------
//    PLEASE DO NOT USE THIS LIBRARY, IT IS CURRENTLY EXPERIMENTAL
//----------------------------------------------------------------------
//   HEAVILY RECCOMENDED TO NOT USE IT, IT IS CURRENTLY FULL OF BUGS
//----------------------------------------------------------------------

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
#define ARRAY_INITIAL_BACKTRACK 32
#define ARRAY_INITIAL_DELETED_ITEM_CAP 16

typedef struct {
	size_t cap;		// in bytes
	size_t index;		// counter in numbers
	size_t itemsize;	// in bytes
	unsigned char *data;	// actualy data
} array;

int array_init(array *array, int size);
int array_push(array *array, void *data);
void *array_get(array *array, size_t index);
void array_pop(array *array);

#endif // ARRAY_H

#ifdef ARRAY_IMPLEMENTATION

int array_init(array *array, int size)
{
	array->cap = ARRAY_INITIAL_CAP;
	array->index = 0;
	array->itemsize = size;
	if ((array->data = malloc(ARRAY_INITIAL_CAP)) == NULL)
		return 0;

	return 1;
}

// Unfortunately we cannot replace deleted items with
// the data* as it will screw the indexing. For that
// Whenever array_free_item() is used it will
// leave a fragmentation behind. For a non
// fragmented version please check arena.h
int array_push(array *array, void *data)
{
	if (array->cap <= (array->index * array->itemsize) + array->itemsize) {
		size_t newcap = array->cap * 2;
		unsigned char *tmp = realloc(array->data, newcap);
		if (tmp == NULL) {
			tmp = malloc(newcap);
			if (tmp == NULL)
				return 0;
			memcpy(tmp, array->data, array->index * array->itemsize);
			free(array->data);
		}
		array->cap = newcap;
		array->data = tmp;
	}
	memcpy(array->data + (array->index * array->itemsize), data, array->itemsize);
	return array->index++;
}

// When using this function DO NOT call
// array_push or array_alloc before writing
// into the memory that you want, as the pointer
// that you get back can be removed and reassigned
// to another address, so when you use this function
// assign the value right after and leave it as is
// before calling any other pushing or reserving function
// from this library, you might write into a invalid memory
// so be careful!
void *array_alloc(array *array)
{
	if (array->cap <= (array->index * array->itemsize) + array->itemsize) {
		size_t newcap = array->cap * 2;
		unsigned char *tmp = realloc(array->data, newcap);
		if (tmp == NULL) {
			tmp = malloc(newcap);
			if (tmp == NULL)
				return NULL;
			memcpy(tmp, array->data, array->index * array->itemsize);
			free(array->data);
		}
		array->cap = newcap;
		array->data = tmp;
	}
	return array->data + (array->index++ * array->itemsize);
}

void array_replace_item(array *array, size_t index, void *data)
{
	// Place the data to the specified index
	memcpy(array->data + (index * array->itemsize), data, array->itemsize);
}

void *array_get(array *array, size_t index)
{
	assert((size_t)array->index >= (size_t)index);
	return array->data + (index * array->itemsize);
}

// We maybe hash this values later.
int array_free_item(array *array, size_t index)
{
	// Clear deleted items
	memset(array->data + (index * array->itemsize), 0, array->itemsize);
	return 1;
}

void array_free(array *array)
{
	free(array->data);
	array->cap = 0;
	array->itemsize = 0;
	array->index = 0;
}

void array_pop(array *array)
{
	array_free_item(array, array->index - 1);
	array->index--;
}

#endif // ARRAY_IMPLEMENTATION