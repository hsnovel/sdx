// This file is a part of sdx libraries
// https://github.com/xcatalyst/sdx
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

#include "string_operations.h"
#include <stdlib.h>

char *so_skip_whitespace(char *in)
{
	char *result = in;
	for (;;) {
		switch(*result) {
		case ' ':
		case '\r':
		case '\t':
		case '\n': {
			result++;
		} break;
		default:
			return result;
		}
	}
	return result;
}

char *so_skip_untill_whitespace(char *in)
{
	char *result = in;
	for (;;) {
		switch(*result) {
		case ' ':
		case '\r':
		case '\t':
		case '\n': {
			return result;
		} break;
		default:
			result++;
		}
	}
	return result;

}

char *so_skip_untill_next_word(char *in)
{
	char *result = in;
	for (;;) {
		switch(*result) {
		case ' ':
		case '\r':
		case '\t':
		case '\n': {
			goto cleanup;
		} break;
		default:
			result++;
		}
	}

 cleanup:
	result = so_skip_whitespace(result);
	return result;

}

char *so_skip_untill_x_char(char *in, char x)
{
	char *result = in;
	while (*result != x)
		result++;

	return result;
}

// Return a null terminated string from a part of
// a string
char *so_get_substring(char *ptr, size_t size)
{
	char *result = malloc(size);
	memcpy(result, ptr, size);
	result[size] = '\0';
	return result;
}

// This function does not check if dest has enough size
// expects the caller to check for it, if they are NEVER
// going to copy a big portion of the string such a check
// might be unncesesary.
void so_get_substring_dest(char *ptr, size_t size, char *dest)
{
	memcpy(dest, ptr, size);
	dest[size] = '\0';
}

char *so_skip_to_nextline(char *in)
{
	char *result = in;
	while (*result != '\n')
		result++;
	return ++result;
}
