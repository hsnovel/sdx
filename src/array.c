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

#include "array.h"

int array_init(struct array *array, size_t size, size_t num_alloc)
{
	num_alloc = (num_alloc == 0) ? ARRAY_INITIAL_CAP : num_alloc;
	array->index = 0;
	array->itemsize = size;
	if ((array->data = malloc(ARRAY_INITIAL_CAP)) == NULL)
		return 0;

	return 1;
}

// Unfortunately we cannot replace deleted items with
// the data* as it will screw the indexing. For that
// Whenever array_free_item() is used it will
// leave a fragmentation behind.
int array_push(struct array *array, void *data)
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
void *array_alloc(struct array *array)
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

void array_replace_item(struct array *array, size_t index, void *data)
{
	// Place the data to the specified index
	memcpy(array->data + (index * array->itemsize), data, array->itemsize);
}

void *array_get(struct array *array, size_t index)
{
	assert((size_t)array->index >= (size_t)index);
	return array->data + (index * array->itemsize);
}

// We maybe hash this values later.
int array_free_item(struct array *array, size_t index)
{
	// Clear deleted items
	memset(array->data + (index * array->itemsize), 0, array->itemsize);
	return 1;
}

void array_free(struct array *array)
{
	free(array->data);
	array->cap = 0;
	array->itemsize = 0;
	array->index = 0;
}

void array_clear(struct array *array)
{
	memset(array->data, 0, array->index * array->itemsize);
	array->index = 0;
}

void array_pop(struct array *array)
{
	array_free_item(array, array->index - 1);
	array->index--;
}

void array_overwrite(struct array *array)
{
	array->index = 0;
}

size_t array_size(struct array *array)
{
	return array->index;
}
