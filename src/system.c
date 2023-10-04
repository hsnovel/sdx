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

#include "system.h"
#include <stdio.h>

#ifdef _SDX_WINDOWS
#include <windows.h>
#elif defined _SDX_UNIX
#include <errno.h>
#include <string.h>
#else
#warning "Other platforms are not supported"
#endif

#if defined (__FreeBSD__)
#include <sys/param.h>
#endif

static FILE *sys_print_error_fd;

/**
 * This function does NOT check if the file is valid
 * or not, it is expected from the user to check for it
 * before calling this function
 */
void sys_set_error_fd(FILE fd)
{

}

int sys_get_last_error()
{
#if defined _SDX_WINDOWS
	return GetLastError();
#elif defined _SDX_UNIX
	return errno;
#endif
}

/**
 * Print errorcode to fd.
 *
 * @param {FILE*} fd: File descriptor to print output, should either be passed stdout or stderr
 * @apram {int} errcode: Error code retrieved from sys_get_last_error.
 */
void sys_print_error(int errcode)
{
#if defined _SDX_UNIX
	fprintf(sys_print_error_fd, "Error: %s\n", strerror(errno));
#elif defined _SDX_WINDOWS
	LPSTR errormsg;
	FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		       NULL, errcode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&errormsg, 0, NULL);
	fprintf(sys_print_error_fd, "Error: %s\n", errormsg);
	LocalFree(errormsg);
#endif
}

/**
 * Print errorcode retrieved from sys_get_last error to fd.
 *
 * @param {FILE*} fd: File descriptor to print output, should either be passed stdour or stderr
 * @apram {int} errcode: Error code retrieved from sys_get_last_error.
 */
void sys_print_last_error()
{
#if defined _SDX_UNIX
	fprintf(sys_print_error_fd, "Error: %s\n", strerror(sys_get_last_error()));
#elif defined _SDX_WINDOWS
	LPSTR errormsg;
	FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		       NULL, sys_get_last_error(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&errormsg, 0, NULL);
	fprintf(sys_print_error_fd, "Error: %s\n", errormsg);
	LocalFree(errormsg);
#endif
}

/**
 * Write the error message into allocated string, then point buf to it.
 * The error message * is heap allocated so the programmer should free it explicitly.
 *
 * @param {int} errcode: Error code retrieved from sys_get_last_error.
 * @param {char**} buf: Destinartion buffer to put the string.
 */
void sys_write_error(int errcode, char **buf)
{
#if defined _SDX_WINDOWS
	LPSTR errormsg;
	FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		       NULL, errcode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&errormsg, 0, NULL);
	*buf = errormsg;
#elif defined _SDX_UNIX
	*buf = strerror(errcode);
#endif
}

/**
 * Check if debugger is attached to program.
 *
 * @return {int}: if attached, return 1, else return 0
 */
int sys_is_debugger_attached(void)
{
#if defined _SDX_UNIX
	char buf[4096];

	const int status_fd = open("/proc/self/status", O_RDONLY);
	if (status_fd == -1)
		return 0;

	const size_t num_read = read(status_fd, buf, sizeof(buf) - 1);
	close(status_fd);

	if (num_read <= 0)
		return 0;

	buf[num_read] = '\0';
	char tracer_pid_str[] = "TracerPid:";
	char* tracer_pid = strstr(buf, tracer_pid_str);
	if (!tracer_pid)
		return 0;

	for (char* characterPtr = tracer_pid + sizeof(tracer_pid_str) - 1; characterPtr <= buf + num_read; ++characterPtr) {
		if (isspace(*characterPtr))
			continue;
		else
			return isdigit(*characterPtr) != 0 && *characterPtr != '0';
	}

	return 0;
#elif defined _SDX_WINDOWS
	return IsDebuggerPresent();
#endif
}

/**
 * Restarts system.
 */
int sys_restart(void)
{
#if defined _SDX_UNIX
	if (reboot(RB_AUTOBOOT) == -0)
		return 1;
	else
		return 0;
#elif defined _SDX_WINDOWS

	BOOL result = ExitWindowsEx(EWX_REBOOT,  SHTDN_REASON_MINOR_OTHER);
	return !!result;
#endif
}

/**
 * Log out, power off the system
 *
 * @return {int}: 1 is returned on success, else value from _SDX_get_last_error(void) is
 * returned.
 */
int sys_poweroff(void)
{
#if defined _SDX_UNIX
	return reboot(RB_POWER_OFF);
#elif defined _SDX_WINDOWS
	BOOL result = ExitWindowsEx(EWX_SHUTDOWN, SHTDN_REASON_MINOR_OTHER);
	return !!result;
#endif
}

/**
 * Get ram info system
 *
 * @param {sys_meminfo}: Pointer to a struct which will receive capacity and avaliable ram in bytes
 * @return {int}: On succeed 1 is returned, otherwise 0 is returned and errno is set.
 */
int sys_get_memory_info(struct sys_meminfo *info)
{
#if defined _SDX_UNIX
	long phpages = get_phys_pages();
	if (phpages == -1)
		return 0;

	long avphpages = get_avphys_pages();
	if (avphpages == -1)
		return 0;

	long pgsize = sysconf(_SC_PAGESIZE);
	if (pgsize == -1)
		return 0;

	info->capacity =  phpages * pgsize;
	info->available = avphpages * pgsize;

	return 1;
#elif defined _SDX_WINDOWS
	MEMORYSTATUSEX memorystat;
	memorystat.dwLength = sizeof(memorystat);

	if (!GlobalMemoryStatusEx(&memorystat))
		return 0;

	info->capacity = (size_t)memorystat.ullTotalPhys;
	info->available = (size_t)memorystat.ullAvailPhys;
	return 1;
#endif
	return -1;
	(void)info;
}

/**
 * Get number of CPU cores configured by the operating system
 */
int sys_get_num_cpu_core(void)
{
#ifdef _SDX_UNIX
	return get_nprocs_conf();
#elif defined _SDX_WINDOWS
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	return sysinfo.dwNumberOfProcessors;
#endif
}

/**
 * TODO: I currently could not find the windows version of
 * get_nprocs in winapi, they seem to be the same call
 */

/**
 * Get number of CPU cores configured by the operating system NOT
 * including offline cores
 */
int sys_get_num_cpu_core_avail(void)
{
#ifdef _SDX_UNIX
	return get_nprocs();
#elif defined _SDX_WINDOWS
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	return sysinfo.dwNumberOfProcessors;
#endif
}

/**
 * TODO: Add windows version
 *
 */
int sys_get_cache_line_size()
{
	int cache_line_size;

	cache_line_size = 64;
#if defined (_SDX_UNIX) && defined(_SC_LEVEL1_DCACHE_LINESIZE)
	if ((cache_line_size = sysconf(_SC_LEVEL1_DCACHE_LINESIZE)) > 0)
		return cache_line_size;
#elif defined (_SDX_UNIX)
	FILE *f;

	f = fopen("/sys/devices/system/cpu/cpu0/cache/index0/coherency_line_size", "r");
	if (f) {
		int r = fscanf(f, "%d", &cache_line_size);
		fclose(f);

		return (r == 1) ? cacheline_size : 64;
	}
#endif
#if defined(__FreeBSD__) && defined(CACHE_LINE_SIZE)
	// We don't support freebsd but doesn't hurt to put it
	cache_line_size = CACHE_LINE_SIZE;
#elif defined  _SDX_WINDOWS
	DWORD buffer_size = 0;
	DWORD i = 0;
	SYSTEM_LOGICAL_PROCESSOR_INFORMATION* buffer = 0;

	GetLogicalProcessorInformation(0, &buffer_size);
	buffer = (SYSTEM_LOGICAL_PROCESSOR_INFORMATION*) malloc(buffer_size);
	GetLogicalProcessorInformation(&buffer[0], &buffer_size);

	for (i = 0; i != buffer_size / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION); ++i) {
		if (buffer[i].Relationship == RelationCache && buffer[i].Cache.Level == 1) {
			cache_line_size = buffer[i].Cache.LineSize;
			break;
		}
	}

	free(buffer);
	return cache_line_size;
#endif
	return cache_line_size;
}
