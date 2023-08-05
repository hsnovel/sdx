#define ARENA_IMPLEMENTATION
#include "../arena.h"

#include <stdio.h>

typedef struct {
	int a;
	int b;
	int c;
	int k;
	int n;
	int p;
	int r;
	int s;
	int t;
	int x;
	int y;
	int z;
	int o;
	int f;
	int g;
	int l;
} teststr;

typedef struct {
	teststr a;
	teststr b;
	teststr c;
	teststr d;
} bigstr;

int main()
{
	arena ar;
	if (arena_init(&ar) == 0) {
		printf("Unable to init arena\n");
	}

	bigstr *data = arena_alloc(&ar, sizeof(bigstr));
	bigstr *strc = arena_alloc(&ar, sizeof(bigstr));
	if (strc == NULL) {
		printf("Exiting");
		exit(1);
	}

	for (int i = 0; i < 10000; i++) {
		arena_free_type(&ar, data);
		arena_free_type(&ar, strc);
		data = arena_alloc(&ar, sizeof(bigstr));
		strc = arena_alloc(&ar, sizeof(bigstr));
	}

	/*
	 * for (int i = 0; i < 100000; i++) {
	 * 	arena_alloc(&ar, sizeof(int));
	 * }
	 */

}
