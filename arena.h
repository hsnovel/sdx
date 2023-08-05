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

#ifndef ARENA_H
#define ARENA_H

//----------------------------------------------------------------------
//    PLEASE DO NOT USE THIS LIBRARY, IT IS CURRENTLY EXPERIMENTAL
//----------------------------------------------------------------------
// THIS LIBRARY'S DEVELOPMENT IS STOPPED UNTILL I MAKE A HASHMAP LIBRARY
//----------------------------------------------------------------------

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#define VECTOR_IMPLEMENTATION
#include "vector.h"

typedef struct {
	unsigned char *data;	// actual arena memory
	vector backtrack;	// vector of backtrack_info, stores fragmented parts
	size_t cap;		// capacity of data*
	size_t stack_pointer;	// stack_pointer part of data*
	size_t used;
	int flags;		// user defined flags
} arena;

typedef struct {
	void *start;		// starting address of the fragmented part
	size_t size;		// size of the fragmented part in bytes
} backtrack_info;

int arena_init(arena *ar);
void * _arena_push_size(arena *ar, size_t size);
void arena_free(arena *ar, void *ptr, size_t size);
void _arena_get();

#define arena_free_type(arena, ptr) arena_free(arena, ptr, sizeof(*ptr))
#define arena_alloc(arena, size) _arena_push_size(arena, size);

#endif

#ifdef ARENA_IMPLEMENTATION

#define ARENA_DEFAULT_DATA_CAP 256

int arena_init(arena *ar)
{
	ar->stack_pointer = 0;
	ar->flags = 0;
	ar->cap = ARENA_DEFAULT_DATA_CAP;
	ar->used = 0;

	if ((ar->data = malloc(ar->cap)) == NULL) return 0;
	if ((vector_init(&ar->backtrack, sizeof(backtrack_info))) == 0) {
		free(ar->data);
		return 0;
	}

	return 1;
}

// @Todo: Add alignement for the next pushed memory
void * _arena_push_size(arena *ar, size_t size)
{
	// Grow the arena memory if not enough space
	// is present for the pushed size
	if (ar->cap <= size + ar->used) {
		size_t newcap = ar->cap * 2 + size;
		unsigned char *tmp = realloc(ar->data, newcap);
		if (tmp == NULL) {
			tmp = malloc(newcap);
			if (tmp == NULL)
				return 0;
			memcpy(tmp, ar->data, ar->stack_pointer);
			free(ar->data);
		}
		ar->cap = newcap;
		ar->data = tmp;
		printf("newcap: %ld\n", newcap);
	}

	ar->used += size;

	// No previously freed memory is present
	if (ar->backtrack.index == 0) {
		size_t prev_stack_pointer = ar->stack_pointer;
		ar->stack_pointer += size;
		return ar->data + prev_stack_pointer;
	}

	// Freed memory is present, loop through backtrace

	// Make sure that initial best_fit is always replaced with
	// the first pick, set the size to highest possible value
	backtrack_info best_fit = { .start = NULL, .size = ULONG_MAX};

	size_t i = 0;
	size_t best_fit_index = 0;
	do {
		backtrack_info *current = vector_get(&ar->backtrack, i);
		assert(current != NULL && "vector_get returned NULL\n");
		// Check if size is sufficent enough to hold it or
		// vector we got is not deleted
		if ((current->size < size) || (current->size == 0)) {
			i++;
			continue;
		}

		if (current->size < best_fit.size) {
			best_fit = *current;
			best_fit_index = i;
		}
		i++;
	} while (i < ar->backtrack.index);

	// Nowhere to fit the current allocation
	// push it to the top
	if (best_fit.start == NULL) {
		size_t prev_stack_pointer = ar->stack_pointer;
		ar->stack_pointer += size;
		return ar->data + prev_stack_pointer;
	}
	ar->stack_pointer += size;

	// Good fit is found, check memory left over
	// and replace the current current best fit
	// with the space left over
	size_t space_leftover = best_fit.size - size;

	// If it fits perfectly then do not replace it
	// instead delete the entry and return the space
	if (space_leftover == 0) {
		vector_free_item(&ar->backtrack, best_fit_index);
		return best_fit.start;
	}

	// If there is a space left, replace the
	// current fragmentation with the new one
	// that points to the beggining of the new
	// fragmentation part
	backtrack_info fragmented_part = {
		.start = (unsigned char*)best_fit.start + size,
		.size = space_leftover,
	};

	vector_replace_item(&ar->backtrack, best_fit_index, &fragmented_part);
	return best_fit.start;
}

void arena_free(arena *ar, void *ptr, size_t size)
{
	// Add fragmentation entry to backtrack vector
	backtrack_info info = { .start = ptr, .size = size };
	vector_push(&ar->backtrack, &info);
	ar->used -= size;

	// if we are deleting the last element, then
	// update the stack pointer to point to that place
	if (ptr == ar->data + ar->stack_pointer)
		ar->stack_pointer -= size;

	// clear the memory
	memset(ptr, 0, size);
}

#endif
