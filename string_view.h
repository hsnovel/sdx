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

#ifndef STRING_VIEW_H
#define STRING_VIEW_H

#include <stddef.h>
#include <string.h>
#include <ctype.h>

#ifndef SV_DEF
#define SV_DEF
#endif

typedef struct {
	char *data;
	size_t len;
} string_view;

SV_DEF string_view sv_from_parts(char *data, size_t len);
SV_DEF string_view sv_from_cstr(char *data);
SV_DEF int sv_eq(string_view a, string_view b);
SV_DEF string_view sv_chop_by_delim(string_view *sv, char delim);
SV_DEF string_view sv_chop_by_delim_and_trim(string_view *sv, char delim);
SV_DEF string_view sv_chop_left(string_view *sv, size_t n);
SV_DEF string_view sv_chop_right(string_view *sv, size_t n);
SV_DEF string_view sv_chop_by_sv(string_view *sv, string_view delim); /* Return trimmed part, do not advance */
SV_DEF string_view sv_trim_left(string_view sv);
SV_DEF string_view sv_trim_right(string_view sv);
SV_DEF string_view sv_trim(string_view sv);
SV_DEF string_view sv_trim_by_delim(string_view *sv, char delim); /* Use delim as sepeartor, advance until delim is hit and return trimmed part */
SV_DEF string_view sv_trim_by_sv(string_view *sv, string_view delim); /* Return trimemd part, advance pointer to last index */
SV_DEF string_view sv_next_line(string_view sv);

#define SV_Fmt "%.*s"
#define SV_Arg(sv) (int) (sv).len, (sv).data

#define SV(cstr) sv_from_parts(cstr, sizeof(cstr) - 1)
#define SV_STATIC(cstr)			\
	{					\
		sizeof(cstr) - 1,		\
			(cstr)		\
			}

#endif /* STRING_VIEW_H */

#ifdef STRING_VIEW_IMPLEMENTATION

SV_DEF string_view sv_from_parts(char *data, size_t len)
{
	string_view sv;
	sv.data = data;
	sv.len = len;
	return sv;
}

SV_DEF string_view sv_from_cstr(char *data)
{
	string_view sv;
	sv.data = data;
	sv.len = strlen(data);
	return sv;
}

SV_DEF int sv_eq(string_view a, string_view b)
{
	if (a.len != b.len) {
		return 0;
	} else {
		return memcmp(a.data, b.data, a.len) == 0;
	}
}

SV_DEF string_view sv_chop_left(string_view *sv, size_t n)
{
	if (n > sv->len) {
		n = sv->len;
	}

	string_view tmp = sv_from_parts(sv->data, n);

	sv->data  += n;
	sv->len -= n;

	return tmp;
}

SV_DEF string_view sv_chop_right(string_view *sv, size_t n)
{
	if (n > sv->len) {
		n = sv->len;
	}

	string_view result = sv_from_parts(sv->data + sv->len - n, n);

	sv->len -= n;

	return result;
}

SV_DEF string_view sv_chop_by_delim(string_view *sv, char delim)
{
	size_t i = 0;
	while (i < sv->len && sv->data[i] != delim)
		i++;

	string_view tmp = sv_from_parts(sv->data, i);

	if (i < sv->len) {
		sv->len -= i + 1;
		sv->data  += i + 1;
	} else {
		sv->len -= i;
		sv->data  += i;
	}

	return tmp;
}

SV_DEF int sv_try_chop_by_delim(string_view *sv, char delim, string_view *dest)
{
	size_t i = 0;
	while (i < sv->len && sv->data[i] != delim)
		i++;

	string_view result = sv_from_parts(sv->data, i);

	if (i < sv->len) {
		sv->len -= i + 1;
		sv->data  += i + 1;
		if (dest) {
			*dest = result;
		}
		return 1;
	}

	return 0;
}

/**
 * Return trimmed part, do not advance
 *
 * @param {string_view*} sv: target sv object
 * @param {string_view} delim: delimeter string
 * @return {string_view} part that is trimmed
 */

SV_DEF string_view sv_chop_by_sv(string_view *sv, string_view delim)
{
	string_view window = sv_from_parts(sv->data, delim.len);
	size_t i = 0;
	while (i + delim.len < sv->len && !(sv_eq(window, delim))) {
		i++;
		window.data++;
	}

	string_view result = sv_from_parts(sv->data, i);

	if (i + delim.len == sv->len) {
		result.len += delim.len;
	}

	return result;
}

/**
 * Use delim as sepeartor, advance until delim is hit and return trimmed part
 *
 * @param {string_view*} sv: target sv object
 * @param {char} delim: delimeter character
 * @return {string_view} part that is trimmed
 */

SV_DEF string_view sv_trim_by_delim(string_view *sv, char delim)
{
	size_t i = 0;
	while (i < sv->len && sv->data[i] != delim)
		i++;

	string_view trimmed_part = sv_from_parts(sv->data, i);

	if (i < sv->len) {
		sv->len -= i + 1;
		sv->data  += i + 1;
	} else {
		sv->len -= i;
		sv->data  += i;
	}

	return trimmed_part;
}

/**
 * Return trimemd part, advance pointer to last index
 *
 * @param {string_view*} sv: target sv object
 * @param {string_view} delim: delimeter string
 * @return {string_view} part that is trimmed
 */

SV_DEF string_view sv_trim_by_sv(string_view *sv, string_view delim)
{
	string_view window = sv_from_parts(sv->data, delim.len);
	size_t i = 0;
	while (i + delim.len < sv->len && !(sv_eq(window, delim))) {
		i++;
		window.data++;
	}

	string_view result = sv_from_parts(sv->data, i);

	if (i + delim.len == sv->len) {
		result.len += delim.len;
	}

	sv->data += i + delim.len;
	sv->len -= i + delim.len;

	return result;
}

SV_DEF string_view sv_trim_left(string_view sv)
{
	size_t i = 0;
	while (i < sv.len && isspace(sv.data[i])) {
		i += 1;
	}

	return sv_from_parts(sv.data + i, sv.len - i);
}

SV_DEF string_view sv_trim_right(string_view sv)
{
	size_t i = 0;
	while (i < sv.len && isspace(sv.data[sv.len - 1 - i])) {
		i += 1;
	}

	return sv_from_parts(sv.data, sv.len - i);
}

SV_DEF string_view sv_trim(string_view sv)
{
	return sv_trim_right(sv_trim_left(sv));
}

SV_DEF string_view sv_next_line(string_view sv)
{
	string_view result;
	result.data = sv.data;
	result.len = sv.len;
	while (*result.data != '\n') {
		result.data++;
		result.len--;
	}
	result.data++;
	result.len--;

	if (result.len <= 0)
		return sv;

	return result;
}

#endif
