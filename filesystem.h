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

fs_file fs_read_file(char *path);
int fs_space(char *path, fs_space_info *space);
int fs_create_directory(char *path);
int fs_create_file(char *path);
int fs_delete_directory(char *path);
int fs_delete_file(char *path);
int fs_copy_file(char* source, char* dest);
int fs_move_file(char* source, char* dest);

#endif

#ifdef FILESYSTEM_IMPLEMENTATION

fs_file fs_read_file(char *path)
{
	fs_file result = {0};

	FILE *file = fopen(path, "rb");
	if(!file) {
		result.contents = 0;
		result.size = 0;
		return result;
	}

	fseek(file, 0, SEEK_END);
	result.size = ftell(file);
	fseek(file, 0, SEEK_SET);

	result.contents = malloc(result.size);
	if(result.contents && result.size) {
		fread(result.contents, result.size, 1, file);
	}
	else {
		result.contents = 0;
		result.size = 0;
	}

	fclose(file);

	return result;
}

int fs_space(char *path, fs_space_info *space)
{
#ifdef _STD_UNIX
	struct statvfs stat;
	statvfs(path, &stat);

	space->capacity = stat.f_frsize * stat.f_blocks;
	space->free = stat.f_frsize * stat.f_bfree;
	space->available = stat.f_frsize * stat.f_bavail;

	return 1;
#elif defined _STD_WINDOWS
	ULARGE_INTEGER freeBytesAvailable, totalNumberOfBytes, totalNumberOfFreeBytes;

	if (GetDiskFreeSpaceEx(path, &freeBytesAvailable, &totalNumberOfBytes, &totalNumberOfFreeBytes))
	{
		space->available = (size_t)freeBytesAvailable.QuadPart;
		space->capacity = (size_t)totalNumberOfBytes.QuadPart;
		space->free = (size_t)totalNumberOfFreeBytes.QuadPart;
		return 1;
	}
	else {
		return 0;
	}
#else
	return 0;
#endif
}
/**
 * Create directory with given permissions
 *
 * @return {int}: Errno is returned
 */

int fs_create_directory(char *path)
{
#ifdef _STD_UNIX
	struct stat st = {0};

	if (stat(path, &st) == -1) {
		mkdir(path, 0755);
		return 1;
	}
	else {
		return errno;
	}
#elif defined _STD_WINDOWS
	BOOL result = CreateDirectoryA(path, NULL);
	return !!result;
#endif

}

/**
 * Create directory
 *
 * @param {char*} path: Path to create the directory, name of the directory is included to path
 * @return {int}: On success 1 is returned, otherwise errno is returned
 */
int fs_create_file(char *path)
{
#ifdef _STD_UNIX
	int fd = open(path, O_CREAT | S_IRUSR | S_IWUSR);
	if (fd == -1)
		return errno;

	int err = close(fd);
	if (err == -1)
		return errno;

	return 1;
#elif defined _STD_WINDOWS
	HANDLE fd = CreateFileA(path, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	if (fd != INVALID_HANDLE_VALUE) {
		CloseHandle(fd);
		return 1;
	}
	return 0;
#endif
}

/**
 * Delete directory
 *
 * @param {char*} path: Path to the directory to be deleted
 * @return {int}: 1 is returned on success, otherwise errno is returned
 */
int fs_delete_directory(char *path)
{
#if defined _STD_UNIX
	if (rmdir(path) == -1)
		return errno;
	return 1;
#elif defined _STD_WINDOWS
	BOOL result = RemoveDirectoryA(path);
	return !!result;
#endif
}

/**
 * Delete file
 *
 * @param {char*} path: Path to the file to be deleted
 * @return {int}: 1 is returned on success, otherwise errno is returned
 */
int fs_delete_file(char *path)
{
#if defined _STD_UNIX
	if (remove(path) == -1)
		return errno;
	return 1;
#elif defined _STD_WINDOWS
	BOOL result = DeleteFileA(path);
	return !!result;
#endif
}

/**
 * Copy file from source to destination
 *
 * @param {char*} source: Path to the source file to be copied
 * @param {char*} dest: Destination to copy to
 * @return {int}: On success 1 is returned, on failure 0 is returned and
 * errorno is set.
 */
int fs_copy_file(char* source, char* dest)
{
#if defined _STD_WINDOWS
	BOOL result = CopyFile(source, dest, FALSE);
	return !!result;
#elif defined(_STD_LINUX) || defined(_STD_FREEBSD)
	int srcf = open(source, S_IRUSR, S_IWUSR);
	int destf = open(dest, S_IRUSR, S_IWUSR);
	struct stat st;
	if (fstat(srcf, &st))
		goto error;
	if (splice(srcf, NULL, destf, NULL, st.st_size, 0) == -1)
		goto error;
	return 1;
 error:
	close(srcf);
	close(destf);
	return 0;
#elif defined _STD_UNIX
#endif
}

/**
 * Move file from source to desitnation
 *
 * @param {char*} source: Source path to file to be moved
 * @param {char*} dest: Destination path to move the file to
 * @return {int}: On success 1 is returned, on failure 0 is returned and
 * errorno is set.
 */
int fs_move_file(char* source, char* dest)
{
#if defined _STD_WINDOWS
	BOOL result = MoveFile(source, dest);
	return !!result;
#elif defined _STD_UNIX
	int result = rename(source, dest);
	return !result;
#endif
}

#endif /* FILESYSTEM_IMPLEMENTATION */
