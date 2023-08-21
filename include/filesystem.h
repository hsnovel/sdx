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

#include <time.h>
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

typedef struct
{
	char *data;
	size_t size;
} fs_file;

typedef struct {
	time_t last_access;
	time_t last_modify;
	time_t last_status_change; // This is creation date in win32
} fs_ftime_info;

enum fs_error {
	FS_UNABLE_TO_OPEN_FILE,
	FS_LEFT_UNWRITTEN_DATA,
	FS_SUCCESS = 1,
};

fs_file fs_file_read(char *path);
int fs_file_write(char *path, void *data, size_t size);
int fs_file_create(char *path);
int fs_file_copy(char* source, char* dest);
int fs_file_delete(char *path);
int fs_file_move(char* source, char* dest);
int fs_file_time(char *path, fs_ftime_info *time);

int fs_space(char *path, fs_space_info *space);
int fs_directory_create(char *path);
int fs_directory_delete(char *path);
int fs_cwd(char *dst, int size);

#endif
