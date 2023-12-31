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

#include "filesystem.h"

#ifdef _SDX_WINDOWS
#include <io.h>
#define F_OK 0
#define access _access
#endif

char *fs_mode_map[] = {
	[FS_APPEND] = "a",
	[FS_WRITE] = "w",
	[FS_READ_BINARY] = "rb",
	[FS_READ_TEXT] = "r",
};

struct fs_file fs_file_read(char *path, enum fs_mode mode)
{
	struct fs_file result = {0};
	size_t alloc_size = 0;

	FILE *file = fopen(path, fs_mode_map[mode]);
	if(!file) {
		result.data = 0;
		result.size = 0;
		return result;
	}

	fseek(file, 0, SEEK_END);
	result.size = ftell(file);
	fseek(file, 0, SEEK_SET);

	if (mode == FS_READ_TEXT)
		alloc_size = result.size + 1;
	else
		alloc_size = result.size;

	result.data = malloc(result.size + 1);
	if(result.data && result.size) {
		fread(result.data, result.size, 1, file);
		if (mode == FS_READ_TEXT)
			((char*)result.data)[result.size] = '\0';
	}
	else {
		result.data = 0;
		result.size = 0;
	}

	fclose(file);

	return result;
}

/**
 * This function writes the data to the file at path
 *
 * @param enum fs_mode {mode}: Mode can either be FS_APPEND
 *	or FS_WRITE, if FS_APPEND then data is appended to the end
 *	if FS_WRITE then the data inside the file is erased, then
 *	specified data by the user is written
 *
 * @return {int}: This function either returns
 *	FS_UNABLE_TO_OPEN_FILE
 *	FS_LEFT_UNWRITTEN_DATA
 *	FS_SUCCESS.
 */

// @Todo: Make another function to write to a spesific location
int fs_file_write(char *path, void *data, size_t size, enum fs_mode mode)
{
	FILE *file = fopen(path, fs_mode_map[mode]);
	if(!file)
		return FS_UNABLE_TO_OPEN_FILE;

	if (fwrite(data, 1, size, file) != size) {
		fclose(file);
		return FS_LEFT_UNWRITTEN_DATA;
	}

	fclose(file);
	return FS_SUCCESS;
}

int fs_space(char *path, struct fs_space_info *space)
{
#ifdef _SDX_UNIX
	struct statvfs stat;
	statvfs(path, &stat);

	space->capacity = stat.f_frsize * stat.f_blocks;
	space->free = stat.f_frsize * stat.f_bfree;
	space->available = stat.f_frsize * stat.f_bavail;

	return 1;
#elif defined _SDX_WINDOWS
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

int fs_directory_create(char *path)
{
#ifdef _SDX_UNIX
	struct stat st = {0};

	if (stat(path, &st) == -1) {
		mkdir(path, 0755);
		return 1;
	}
	else {
		return errno;
	}
#elif defined _SDX_WINDOWS
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
int fs_file_create(char *path)
{
#ifdef _SDX_UNIX
	int fd = open(path, O_CREAT | S_IRUSR | S_IWUSR);
	if (fd == -1)
		return errno;

	int err = close(fd);
	if (err == -1)
		return errno;

	return 1;
#elif defined _SDX_WINDOWS
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
int fs_directory_delete(char *path)
{
#if defined _SDX_UNIX
	if (rmdir(path) == -1)
		return errno;
	return 1;
#elif defined _SDX_WINDOWS
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
int fs_file_delete(char *path)
{
#if defined _SDX_UNIX
	if (remove(path) == -1)
		return errno;
	return 1;
#elif defined _SDX_WINDOWS
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
int fs_file_copy(char* source, char* dest)
{
#if defined _SDX_WINDOWS
	BOOL result = CopyFile(source, dest, FALSE);
	return !!result;
#elif defined(_SDX_LINUX) || defined(_SDX_FREEBSD)
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
#elif defined _SDX_UNIX
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
int fs_file_move(char* source, char* dest)
{
#if defined _SDX_WINDOWS
	BOOL result = MoveFile(source, dest);
	return !!result;
#elif defined _SDX_UNIX
	int result = rename(source, dest);
	return !result;
#endif
}

/**
 * Get file creation, modify access status
 *
 * @param {char*} path: Source path to file to get status
 * @param {fs_file_time*} time: Struct to receive file information
 * @return {int}: On success 1 is returned, on failure 0 is returned and
 * errorno is set.
 */
int fs_file_time(char *path, struct fs_ftime_info *time)
{
#ifdef _SDX_UNIX
	int err;
	struct stat attr;

	err = stat(path, &attr);
	if (err == -1)
		return 0;
	/*  */
	time->last_access = attr.st_atime;
	time->last_modify = attr.st_mtime;
	time->last_status_change = attr.st_ctime;

	return 1;
#elif defined (_SDX_WINDOWS)
#endif
}

int fs_cwd(char *dst, int size)
{
#ifdef _SDX_UNIX
	if (getcwd(dst, size) != NULL)
		return 1;
	return 0;
#elif defined (_SDX_WINDOWS)
	return !!GetCurrentDirectory(size, dst)
#endif
}

/*
 * Return 1 if file exists, 0 if not
 */
int fs_file_exists(char *path)
{
	return !access(path, F_OK);
}
