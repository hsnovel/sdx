#ifndef ARENA_H
#define ARENA_H

//----------------------------------------------------------------------
//    PLEASE DO NOT USE THIS LIBRARY, IT IS CURRENTLY EXPERIMENTAL
//----------------------------------------------------------------------

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#define VECTOR_IMPLEMENTATION
#include "vector.h"

typedef struct {
	unsigned char *data;
	unsigned char *buf;
	vector backtrack;
	size_t cap;
	size_t used;
	size_t buf_cap;
	size_t buf_used;
	int flags;
} arena;

typedef struct {
	void *start;
	size_t size;
} backtrack_info;

enum {
	ARENA_NOBUFFERING = 1,
};

int arena_init(arena *ar);
void * _arena_push_size(arena *ar, size_t size);
void _arena_free(arena *ar, void *ptr, size_t size);
void _arena_get();

#define arena_free(arena, ptr) _arena_free(arena, ptr, sizeof(*ptr))
#define arena_push_size(arena, size) _arena_push_size(arena, size);
#define arena_push_type(arena, type) (type*)_arena_push_size(arena, sizeof(type))

#endif

#ifdef ARENA_IMPLEMENTATION

#define ARENA_DEFAULT_DATA_CAP 4096
#define ARENA_DEFAULT_BUFFER_CAP 1048

int arena_init(arena *ar)
{
	ar->used = 0;
	ar->buf_used = 0;
	ar->flags = 0;
	ar->cap = ARENA_DEFAULT_BUFFER_CAP;
	ar->buf_cap = ARENA_DEFAULT_BUFFER_CAP;

	if ((ar->data = malloc(ar->cap)) == NULL) return 0;
	if ((ar->buf = malloc(ar->buf_cap)) == NULL) {
		free(ar->data);
		return 0;
	}
	if (!vector_init(&ar->backtrack, sizeof(backtrack_info))) {
		free(ar->data);
		free(ar->buf);
		return 0;
	}

	return 1;
}

void arena_flush_buffer()
{

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
			memcpy(tmp, ar->data, ar->used);
			free(ar->data);
		}
		ar->cap = newcap;
		ar->data = tmp;
	}

	// No previously freed memory is present
	if (ar->backtrack.index == 0)
		return ar->data + ar->used;

	// New item is too big to buffer, we might
	// increase the buffer size but it seems
	// unnecesarry for now, maybe will do it later
	// @Todo: We still have to check for availabie
	// space on backtrace before doing this
	if ((ar->flags == 1) && (size >= ar->buf_cap))
		return ar->data + ar->used;

	// Freed memory is present, loop through backtrace

	// Make sure that initial best_fit is always replaced with
	// the first pick, set the size to highest possible value
	backtrack_info best_fit = { .start = NULL, .size = ULONG_MAX};

	// No buffering
	if (ar->flags == 1) {
		size_t i = 0;
		size_t best_fit_index = 0;
		do {
			backtrack_info *current = vector_get(&ar->backtrack, i);
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
		if (best_fit.start == NULL)
			return ar->data + ar->used;

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

	return NULL;
}

void _arena_free(arena *ar, void *ptr, size_t size)
{
	backtrack_info info = { .start = ptr,
				.size = size };
	vector_push(&ar->backtrack, &info);
	memset(ptr, 0, size);
}

void _arena_get()
{
	arena_flush_buffer();
}

#endif
