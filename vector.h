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

#ifndef VECTOR_H
#define VECTOR_H

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

#define VECTOR_INITIAL_CAP 256
#define VECTOR_INITIAL_BACKTRACK 32
#define VECTOR_INITIAL_DELETED_ITEM_CAP 16

typedef struct {
	size_t cap;		// in bytes
	size_t index;		// counter in numbers
	size_t itemsize;	// in bytes
	unsigned char *data;	// actualy data
	int *deleted_items;	// funny but we need a vector inside a vector
	int deleted_item_cap;	// deleted item vector's capacity as count
	int deleted_item_index; // deleted item vector's current index
} vector;

int vector_init(vector *vector, int size);
int vector_push(vector *vector, void *data);
void *vector_get(vector *vector, size_t index);
void vector_pop(vector *vector);

#endif // VECTOR_H

#ifdef VECTOR_IMPLEMENTATION

int vector_init(vector *vector, int size)
{
	vector->cap = VECTOR_INITIAL_CAP;
	vector->index = 0;
	vector->itemsize = size;
	vector->deleted_item_index = 0;
	vector->deleted_item_cap = VECTOR_INITIAL_DELETED_ITEM_CAP;
	if ((vector->data = malloc(VECTOR_INITIAL_CAP)) == NULL)
		return 0;
	/*
	 * if ((vector->deleted_items = malloc(VECTOR_INITIAL_DELETED_ITEM_CAP * sizeof(int))) == NULL) {
	 * 	free(vector->data);
	 * 	return 0;
	 * }
	 *
	 * for (int i = 0; i < vector->deleted_item_cap; i++)
	 * 	vector->deleted_items[i] = -1;
	 */

	return 1;
}

// Unfortunately we cannot replace deleted items with
// the data* as it will screw the indexing. For that
// Whenever vector_free_item() is used it will
// leave a fragmentation behind. For a non
// fragmented version please check arena.h
int vector_push(vector *vector, void *data)
{
	if (vector->cap <= (vector->index * vector->itemsize) + vector->itemsize) {
		size_t newcap = vector->cap * 2;
		unsigned char *tmp = realloc(vector->data, newcap);
		if (tmp == NULL) {
			tmp = malloc(newcap);
			if (tmp == NULL)
				return 0;
			memcpy(tmp, vector->data, vector->index * vector->itemsize);
			free(vector->data);
		}
		vector->cap = newcap;
		vector->data = tmp;
	}
	memcpy(vector->data + (vector->index * vector->itemsize), data, vector->itemsize);
	vector->index++;
	return 1;
}

void vector_replace_item(vector *vector, size_t index, void *data)
{
	// Place the data to the specified index
	memcpy(vector->data + (index * vector->itemsize), data, vector->itemsize);

	// Remove the item from deleted index
	vector->deleted_items[index] = 0;
}

void *vector_get(vector *vector, size_t index)
{
	assert((size_t)vector->index >= (size_t)index);
	/*
	 * for (int i = 0; i < vector->deleted_item_cap; i++) {
	 * 	if (index == (size_t)vector->deleted_items[i])
	 * 		return NULL;
	 * }
	 */
	return vector->data + (index * vector->itemsize);
}

// We maybe hash this values later.
int vector_free_item(vector *vector, size_t index)
{
	// Clear deleted items
	/*
	 * for (size_t i = 0; i < vector->itemsize / sizeof(int); i++)
	 * 	vector->deleted_items[i] = -1;
	 */

	// Check deleted item array to see if there is enough space
	/*
	 * if (vector->deleted_item_cap < vector->deleted_item_index) {
	 * 	size_t newcap = vector->deleted_item_cap * 2 * sizeof(int);
	 * 	unsigned char *tmp = realloc(vector->data, newcap);
	 * 	if (tmp == NULL) {
	 * 		tmp = malloc(newcap);
	 * 		if (tmp == NULL)
	 * 			return 0;
	 * 		memcpy(tmp, vector->deleted_items, vector->deleted_item_index * sizeof(int));
	 * 		free(vector->deleted_items);
	 * 	}
	 * 	vector->cap = newcap;
	 * 	vector->data = tmp;
	 * }
	 */

	// Add the index to the deleted items vector
	/*
	 * vector->deleted_items[vector->deleted_item_index] = index;
	 * vector->deleted_item_index++;
	 */
	return 1;
}

void vector_free(vector *vector)
{
	free(vector->data);
	vector->cap = 0;
	vector->itemsize = 0;
	vector->index = 0;
}

void vector_pop(vector *vector)
{
	vector_free_item(vector, vector->index - 1);
	vector->index--;
}

#endif // VECTOR_IMPLEMENTATION
