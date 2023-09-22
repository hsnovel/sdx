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

#ifndef ARENA_H
#define ARENA_H

// This library is not a main memory allocator but more of a
// temporary one, or if you are sure you are never going to
// free memory this should be a perfect fit as well, this
// arena implementation does depend on array.h, it does not
// handle fragmentations or anything like that. This is as
// lightweight as it can get. This library may leave a lot of
// empty space if you are allocating a single chunk with bigger
// size than ARENA_DEFAULT_DATA_CAP. This arena library will grow
// automatically, so you don't have to depend on a fixed size memory

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

#include "array.h"

struct arena_entry {
	unsigned char *data;	// actual arena memory
	size_t cap;		// capacity of data*
	size_t used;
};

struct arena {
	struct array arenas;
	int current_arena; // index
	int flags;
};

enum  {
	ALIGN_NEXT_BLOCK = 1,
	ALIGN_UNTILL_DISABLED = 2,
};

int arena_init(struct arena *ar);
void *arena_alloc(struct arena *ar, size_t size);
int arena_entry_init(struct arena_entry *ar, size_t size);
void arena_align_next_block(struct arena *ar);
void arena_align_untill_disabled(struct arena *ar);
void arena_align_disable(struct arena *ar);
void arena_align_disable_full(struct arena *ar);
void arena_free(struct arena *ar);

#ifndef ARENA_DEFAULT_DATA_CAP
#define ARENA_DEFAULT_DATA_CAP 4096 // All pages are set to 4k by default
#endif

#endif // ARENA_H
