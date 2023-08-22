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

#include "arena.h"

// Unset other alignement options if present, then set the
// current alignement policy
void arena_align_next_block(struct arena *ar)
{
	if ((!(ar->flags & ALIGN_NEXT_BLOCK))) {
		if (ar->flags & ALIGN_UNTILL_DISABLED)
			ar->flags &= ~(ALIGN_UNTILL_DISABLED);
		ar->flags |= ALIGN_NEXT_BLOCK;
	}
}

void arena_align_untill_disabled(struct arena *ar)
{
	if ((!(ar->flags & ALIGN_UNTILL_DISABLED))) {
		if (ar->flags & ALIGN_NEXT_BLOCK)
			ar->flags &= ~(ALIGN_NEXT_BLOCK);
		ar->flags |= ALIGN_UNTILL_DISABLED;
	}
}

void arena_align_disable(struct arena *ar)
{
	ar->flags &= ~(ALIGN_UNTILL_DISABLED);
}

void arena_align_disable_full(struct arena *ar)
{
	ar->flags &= ~(ALIGN_NEXT_BLOCK);
	ar->flags &= ~(ALIGN_UNTILL_DISABLED);
}

int arena_entry_init(struct arena_entry *ar, size_t size)
{
	if (size != 0)
		ar->cap = size;
	else
		ar->cap = ARENA_DEFAULT_DATA_CAP;
	ar->used = 0;

	if ((ar->data = malloc(ar->cap)) == NULL) return 0;

	return 1;
}

int arena_init(struct arena *ar)
{
	array_init(&ar->arenas, sizeof(struct arena_entry));
	ar->current_arena = 0;
	ar->flags = 0;

	struct arena_entry initial_entry;
	arena_entry_init(&initial_entry, 0);
	array_push(&ar->arenas, &initial_entry);
	return 1;
}

// @Todo: Add alignement for the next pushed memory
void *arena_push_size(struct arena_entry *ar, size_t size, int should_align)
{
	int align_off = 0;
	if (should_align)
		align_off = (size_t)(ar->data + size) % sizeof(void*) - 1;

	if (ar->cap < size + ar->used + align_off)
		return NULL;

	void *current = ar->data + ar->used + align_off;
	ar->used += size + align_off;
	return current;
}

void *arena_alloc(struct arena *ar, size_t size)
{
	int should_align = 0;
	struct  arena_entry *current_entry = array_get(&ar->arenas, ar->current_arena);

	if (ar->flags & ALIGN_NEXT_BLOCK) {
		should_align = 1;
		ar->flags &= ~(ALIGN_NEXT_BLOCK);
	} else if(ar->flags & ALIGN_UNTILL_DISABLED) {
		should_align = 1;
	}

	// in either case we will need to allocate a new block of memory
	void *result = arena_push_size(current_entry, size, should_align);
	if ((size > ARENA_DEFAULT_DATA_CAP) || result == NULL) {
		struct arena_entry new_entry;

		if (size > ARENA_DEFAULT_DATA_CAP)
			arena_entry_init(&new_entry, size);
		else
			arena_entry_init(&new_entry, 0);

		array_push(&ar->arenas, &new_entry);
		ar->current_arena++;

		struct arena_entry *current_entry = array_get(&ar->arenas, ar->current_arena);
		return arena_push_size(current_entry, size, should_align);
	}
	return result;
}

void arena_free(struct arena *ar)
{
	for (size_t i = 0; i < ar->arenas.index; i++) {
		struct arena_entry *entry = array_get(&ar->arenas, i);
		free(entry->data);
	}

	array_free(&ar->arenas);
}
