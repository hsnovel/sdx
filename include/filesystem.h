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

#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "extra.h"

#ifdef _STD_LINUX
#define _GNU_SOURCE
#endif

#ifdef _STD_UNIX
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#elif defined _STD_WINDOWS
#include <windows.h>
#endif

#include <stdlib.h>

typedef struct {
	size_t capacity;
	size_t free;
	size_t available;
} fs_space_info;

typedef struct entire_file
{
	void *contents;
	size_t size;
} fs_file;

fs_file fs_read_file(char *path, char *mode);
int fs_space(char *path, fs_space_info *space);
int fs_create_directory(char *path);
int fs_create_file(char *path);
int fs_delete_directory(char *path);
int fs_delete_file(char *path);
int fs_copy_file(char* source, char* dest);
int fs_move_file(char* source, char* dest);

#endif
