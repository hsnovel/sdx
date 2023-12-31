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

#define MEM_DEBUG_DISABLE
#include "mem_debug.h"

#include <array.h>
#include <time.h>

struct memory_info {
	void *ptr;
	void *new_ptr;
	size_t line;
	size_t num;
	char *file;
};

static struct {
	struct array malloc_info;
	struct array free_info;
} mem_debug_info;

static char bulk_memory[MEM_DEBUG_OUT_OF_BOUNDS_NUM];

void randomize_buffer(char *buf, size_t size) {
	srand(time(NULL));
	for (size_t i = 0; i < size; i++)
		buf[i] = rand() % 256;
}


void *mem_debug_malloc(size_t num, size_t line, char *file)
{
	void *mem = malloc(num + MEM_DEBUG_OUT_OF_BOUNDS_NUM);
	void *tmp = mem + num;

	memcpy(tmp, bulk_memory, MEM_DEBUG_OUT_OF_BOUNDS_NUM);
	/*
	 * for (int i = 0; i < MEM_DEBUG_OUT_OF_BOUNDS_NUM; i++)
	 * 	((unsigned char*)tmp)[i] = MEM_DEBUG_OUT_OF_BOUNDS_MAGIC_NUM;
	 */

	struct memory_info info;
	{
		info.ptr = mem;
		info.new_ptr = NULL;
		info.line = line;
		info.num = num;
		info.file = file;
	}

	array_push(&mem_debug_info.malloc_info, &info);

	return mem;
}

// Not implemented properly!
void *mem_debug_realloc(void *ptr, size_t newsize, size_t line, char *file)
{
	struct memory_info *tmp;
	void *new_ptr;
	int previously_allocated = 0;

	// Skip the check for looking for previous mallocs
	// as this parameter will make realloc behave like
	// malloc
	if (ptr == NULL) {
		new_ptr = mem_debug_malloc(newsize, line, file);
		return new_ptr;
	}

	for (int i = 0; i < mem_debug_info.malloc_info.index; i++) {
		tmp = array_get(&mem_debug_info.malloc_info, i);

		if (tmp->ptr == ptr) {
			previously_allocated = 1;
			break;
		}
	}

	if (previously_allocated == 0) {
		printf("----------------------\n");
		printf("Attempting to realloc memory that has not been malloced before\n");
		printf("Ptr: %p\n", ptr);
		printf("Line: %zu\n", line);
		printf("File: %s\n", file);
		printf("----------------------\n");
	}

	new_ptr = realloc(ptr, newsize);

	if (new_ptr == ptr) {
		tmp->new_ptr = new_ptr;
	} else {

	}

	return new_ptr;
}

void mem_debug_free(void *ptr, size_t line, char *file)
{
	free(ptr);

	struct memory_info info;
	{
		info.ptr = ptr;
		info.line = line;
		info.num = 0;
		info.file = file;
	}

	array_push(&mem_debug_info.free_info, &info);
}

void mem_debug_print()
{
	struct memory_info *current;
	struct memory_info *tmp;
	void *tmp2;
	size_t freed_num;

	printf("----------------------\n");

	for (int i = 0; i < mem_debug_info.malloc_info.index; i++) {
		freed_num = 0;
		current = array_get(&mem_debug_info.malloc_info, i);

		if (mem_debug_info.free_info.index) {
			for (int j = 0; j < mem_debug_info.free_info.index; j++) {
				tmp = array_get(&mem_debug_info.free_info, j);

				if (tmp->ptr == current->ptr)
					freed_num++;
			}
		}

		printf("Allocated: %zu bytes\nLine: %zu\nFile: %s\nFreed: %zu time\nAddress: %p\n", current->num, current->line, current->file, freed_num, current->ptr);

		if (freed_num) {
			printf("Freed at file: %s, line: %zu\n", tmp->file, tmp->line);
		}

		tmp2 = current->ptr + current->num;

		int outof_bounds = memcmp(tmp2, bulk_memory, MEM_DEBUG_OUT_OF_BOUNDS_NUM);
		/*
		 * for (int i = 0; i < MEM_DEBUG_OUT_OF_BOUNDS_NUM; i++)
		 * 	if (((unsigned char*)tmp2)[i] != MEM_DEBUG_OUT_OF_BOUNDS_MAGIC_NUM)
		 * 		outof_bounds_count++;
		 */

		if (outof_bounds)
			printf("Memory at this location has been written out of bounds\n");
	}

	printf("----------------------\n");
}

void mem_debug_init()
{
	if (!array_init(&mem_debug_info.malloc_info, sizeof(struct memory_info), 0)) {
		fprintf(stderr, "Unable to initialize array for memory debugging\n");
		return;
	}
	if (!array_init(&mem_debug_info.free_info, sizeof(struct memory_info), 0)) {
		fprintf(stderr, "Unable to initialize array for memory debugging\n");
		return;
	}
	randomize_buffer(bulk_memory, MEM_DEBUG_OUT_OF_BOUNDS_NUM);
}
