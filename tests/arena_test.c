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

int main()
{
	arena ar;
	if (arena_init(&ar) == 0) {
		printf("Unable to init arena\n");
	}

	int *data = arena_alloc(&ar, sizeof(int));
	*data = 10;
	printf("%d\n", *data);
	teststr *strc = arena_alloc(&ar, sizeof(teststr));
	if (strc == NULL) {
		printf("Exiting");
		exit(1);
	}
	strc->a = 10;
	strc->b = 20;
	strc->c = 30;

	printf("%d, %d, %d\n", strc->a, strc->b, strc->c);

	for (int i = 0; i < 100000; i++) {
		arena_alloc(&ar, sizeof(int));
	}

}
