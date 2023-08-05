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

//----------------------------------------------------------------------
//    PLEASE DO NOT USE THIS LIBRARY, IT IS CURRENTLY EXPERIMENTAL
//----------------------------------------------------------------------
// THIS LIBRARY MIGHT BE REMOVED IN THE FUTURE IF I CANNOT DO A GOOD JOB AT IT
//----------------------------------------------------------------------

#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <stdlib.h>
#include <stdio.h>

static double seconds_spend_on_each_function = 0.5;
static double benchmark_iter_start_count = 100;
static double bench_hundered_million_latency;

void benchmark_init();

#define BENCHMARK(function, ...)					\
	do {								\
		hr_clock time;						\
		unsigned long int iter_count;				\
		if (sizeof( (char[]){#__VA_ARGS__} ) != 1){		\
			iter_count = __VA_ARGS__;			\
			goto skip_iter_count_calculation;		\
		}							\
		start_clock(&time);					\
		for (int i = 0; i < benchmark_iter_start_count; i++) {	\
			function;					\
			do_not_optimize_away(&i);			\
		}							\
		end_clock(&time);					\
		iter_count = ((benchmark_iter_start_count * seconds_spend_on_each_function) / time.wt); \
	skip_iter_count_calculation:					\
		hr_clock delay = benchmark_get_iter_count_delay(iter_count); \
		printf("-------------------------------------------\n"); \
		printf("BENCHMARK: %s\n", #function);			\
		start_clock(&time);					\
		for (unsigned long int i = 0; i < iter_count; i++) {	\
			function;					\
			do_not_optimize_away(&i);			\
		}							\
		end_clock(&time);					\
		printf("delay wt: %f\ndelay ct: %f\niteration: %lu\nwalltime: %f\ncputime: %f\n\n", delay.wt, delay.ct, iter_count, time.wt - delay.wt, time.ct - delay.ct); \
	} while(0);							\

#endif

#ifdef BENCHMARK_IMPLEMENTATION

#ifdef _WIN32
#include <windows.h>
double get_wall_time(){
	LARGE_INTEGER time,freq;
	if (!QueryPerformanceFrequency(&freq))
		return 0;
	if (!QueryPerformanceCounter(&time))
		return 0;

	return (double)time.QuadPart / freq.QuadPart;
}
double get_cpu_time(){
	FILETIME a,b,c,d;
	if (GetProcessTimes(GetCurrentProcess(),&a, &b, &c, &d) != 0)
		return (double)(d.dwLowDateTime | ((unsigned long long)d.dwHighDateTime << 32)) * 0.0000001;

	return 0;
}

#else

// @Refactor: There may be a higher resolution timer than these...
#include <time.h>
#include <sys/time.h>
double get_wall_time(){
	struct timeval time;
	if (gettimeofday(&time,NULL)) {
		return 0;
	}
	return (double)time.tv_sec + (double)time.tv_usec * .000001;
}
double get_cpu_time(){
	return (double)clock() / CLOCKS_PER_SEC;
}
#endif


static void do_not_optimize_away(volatile void *p) {
    __asm__ __volatile__("" : : "r" (p) : "memory");
}

double get_wall_time();
double get_cpu_time();

typedef struct {
	double wt0;
	double ct0;
	double wt1;
	double ct1;
	double wt;
	double ct;
} hr_clock;

void start_clock(hr_clock *clock)
{
	clock->wt0 = get_wall_time();
	clock->ct0 = get_cpu_time();
}

void end_clock(hr_clock *clock)
{
	clock->wt1 = get_wall_time();
	clock->ct1 = get_cpu_time();
	clock->wt = clock->wt1 - clock->wt0;
	clock->ct = clock->ct1 - clock->ct0;
}

void benchmark_init()
{
	hr_clock time;
	start_clock(&time);
	for (int i = 0; i < 100000000; i++) {
		do_not_optimize_away(&i);
	}
	end_clock(&time);
	bench_hundered_million_latency = time.ct;
	printf("latency: %f\n", bench_hundered_million_latency);
}

hr_clock benchmark_get_iter_count_delay(int iter_count)
{
	hr_clock time;
	start_clock(&time);
	for (int i = 0; i < iter_count; i++) {
		do_not_optimize_away(&i);
	}
	end_clock(&time);

	return time;
}

#endif // BENCHMARK_IMPLEMENTATION
