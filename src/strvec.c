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
// SOFTWARE.// This file is a part of std libraries
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

#include "strvec.h"

int strvec_init(struct strvec *arr)
{
	arr->data = malloc(STRVEC_INITIAL_DATA_CAP * sizeof(char));
	if (arr->data == NULL)
		return 0;

	if (!array_init(&arr->offsets, sizeof(size_t))) {
		free(arr->data);
		return 0;
	}

	arr->data_cap = STRVEC_INITIAL_DATA_CAP;
	arr->data_size = 0;

	return 1;
}

int strvec_push(struct strvec *arr, char* str)
{
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


	unsigned char *ptr = arr->data + arr->data_size;
	memcpy(ptr, str, len);

	int index = array_push(&arr->offsets, &arr->data_size);

	arr->data_size += len;
	return index;
}

/* It is not possible to free() a part of memory, so we are clearing it*/
/* @Refactor: I might add an another array to this to keep track of empty
* parts of the vecotor and loop though all of them when pushing a string to
* see if there is any ampty space in cleared parts */
int strvec_clear(struct strvec *arr, int index)
{
}

// Rewrite
void strvec_delete_struct(struct strvec* arr)
{
}

char *strvec_get(struct strvec *arr, int index)
{
	assert(index < arr->offsets.index); /*  */
	size_t offset = *((size_t*)array_get(&arr->offsets, index));
	return arr->data + offset;
}
