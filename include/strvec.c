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

#include "strvec.h"

int strvec_init(struct strvec *arr)
{
	arr->data = malloc(STRVEC_INITIAL_DATA_CAP * sizeof(unsigned char));
	if (arr->data == NULL)
		return 0;

	arr->offset = malloc(STRVEC_INITIAL_STR_COUNT * sizeof(size_t));
	if (arr->offset == NULL) {
		free(arr->data);
		return 0;
	}

	arr->data_cap = STRVEC_INITIAL_DATA_CAP;
	arr->offset_cap = STRVEC_INITIAL_STR_COUNT;
	arr->offset_size = 0;
	arr->data_size = 0;
	arr->index = 0;

	return 1;
}

int strvec_push(strruct strvec *arr, unsigned char* str)
{
	if (arr->offset_size >= arr->offset_cap) {
		size_t newcap = arr->offset_cap * 10;
		size_t *tmp = malloc(newcap);
		memcpy(tmp, arr->offset, arr->offset_size);
		arr->offset_cap = newcap;
		if (tmp == NULL)
			return 0;
		/*
		 * free(arr->offset);
		 */
		arr->offset = tmp;
	}

	int len = strlen(str);
	len++; /* \0 character */
	if (arr->data_size >= arr->data_cap) {
		size_t newcap = (arr->data_cap * 10) + len;
		unsigned char *tmp = malloc(newcap);
		memcpy(tmp, arr->data, arr->data_size);
		arr->data_cap = newcap;
		if (tmp == NULL)
			return 0;
		arr->data = tmp;
	}

	unsigned char *ptr = arr->data + arr->offset_size;
	memcpy(ptr, str, len);
	arr->offset[arr->index] = ptr - arr->data;
	arr->offset_size += len;

	return arr->index++;
}

/* It is not possible to free() a part of memory, so we are clearing it*/
/* @Refactor: I might add an another array to this to keep track of empty
* parts of the vecotor and loop though all of them when pushing a string to
* see if there is any ampty space in cleared parts */
int strvec_clear(struct strvec *arr, int index)
{
	int i = 0;
	unsigned char *start = arr->data + arr->offset[index];
	while (start[i] != '\0') {
		i++;
	}
	memset(start, 0, i);
}

void strvec_delete_struct(struct strvec* arr)
{
	free(arr->data);
	free(arr->offset);
	arr->offset_cap = 0;
	arr->offset_size = 0;
	arr->index = 0;
	arr->data_cap = 0;
	arr->data_size = 0;
}

unsigned char *strvec_get(strvec *arr, int index)
{
	assert(index < arr->index);
	return arr->data + arr->offset[index];
}
