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

typedef struct {
	size_t capacity;
	size_t free;
	size_t available;
} fs_space_info;

int fs_space(char *path, fs_space_info *space);
int fs_create_directory(char *path);
int fs_create_file(char *path);
int fs_delete_directory(char *path);
int fs_delete_file(char *path);
int fs_copy_file(char* source, char* dest);
int fs_move_file(char* source, char* dest);

#endif

#ifdef FILESYSTEM_IMPLEMENTATION

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
#elif defined _STD_LINUX
	int srcf = open(source, S_IRUSR, S_IWUSR);
	int destf = open(dest, S_IRUSR, S_IWUSR);
	struct stat st;
	if (fstat(srcf, &st))
		goto error;
	if (splice(srcf, NULL, destf, NULL, st.st_size, 0) == -1)
		goto error;
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
