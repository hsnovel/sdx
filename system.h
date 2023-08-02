#ifndef SYSTEM_H
#define SYSTEM_H

typedef struct {
	size_t capacity;
	size_t available;
} sys_meminfo;

int sys_is_debugger_attached(void);
int sys_restart(void);
int sys_poweroff(void);
int sys_get_memory_info(xmemory_info *info);
int sys_get_memory_info(sys_meminfo *info);
int sys_get_num_cpu_core(void);
int sys_get_num_cpu_core_avail(void);

#endif

#ifdef SYSTEM_IMPLEMENTATION

/**
 * Check if debugger is attached to program.
 *
 * @return {int}: if attached, return 1, else return 0
 */
int sys_is_debugger_attached(void)
{
#if defined _STD_UNIX
	char buf[4096];

	const int status_fd = open("/proc/self/status", O_RDONLY);
	if (status_fd == -1)
		return 0;

	const ssize_t num_read = read(status_fd, buf, sizeof(buf) - 1);
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
#elif defined _STD_WINDOWS
	return IsDebuggerPresent();

#endif
}

/**
 * Restarts system.
 */
int sys_restart(void)
{
#if defined _STD_UNIX
	if (reboot(RB_AUTOBOOT) == -0)
		return 1;
	else
		return _STD_get_last_error();
#elif defined _STD_WINDOWS

	BOOL result = ExitWindowsEx(EWX_REBOOT,  SHTDN_REASON_MINOR_OTHER);
	return !!result;
#endif
}

/**
 * Log out, power off the system
 *
 * @return {int}: 1 is returned on success, else value from _STD_get_last_error(void) is
 * returned.
 */
int sys_poweroff(void)
{
#if defined _STD_UNIX
	if(reboot(RB_POWER_OFF) == 0)
		return 1;
	else
		return _STD_get_last_error();
#elif defined _STD_WINDOWS
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
int sys_get_memory_info(sys_meminfo *info)
{
#if defined _STD_UNIX
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
#elif defined _STD_WINDOWS
	MEMORYSTATUSEX memorystat;
	memorystat.dwLength = sizeof(memorystat);

	if (!GlobalMemoryStatusEx(&memorystat))
		return 0;

	info->capacity = (size_t)memorystat.ullTotalPhys;
	info->available = (size_t)memorystat.ullAvailPhys;
	return 1;
#endif
}

/**
 * Get number of CPU cores configured by the operating system
 */
int sys_get_num_cpu_core(void)
{
#ifdef _STD_UNIX
	return get_nprocs_conf();
#elif defined _STD_WINDOWS
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
#ifdef _STD_UNIX
	return get_nprocs();
#elif defined _STD_WINDOWS
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	return sysinfo.dwNumberOfProcessors;
#endif
}


#endif /* SYSTEM_IMPLEMENTATION */
