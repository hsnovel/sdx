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

#endif // STRING_VIEW_H
