#define ARENA_IMPLEMENTATION
#include "../arena.h"

typedef struct {
	int a;
	int b;
	int c;
	int d;
	int e;
	int f;
	int g;
} test_struct;

int main()
{
	arena ar;
	arena_init(&ar);
	int *data = arena_alloc(&ar, sizeof(int));
	*data = 20;
	assert(*data == 20);

	char *data2 = arena_alloc(&ar, sizeof(char));
	*data2 = 12;
	assert(*data2 == 12);

	arena_align_next_block(&ar);
	test_struct *test = arena_alloc(&ar, sizeof(test_struct));
	test->a = 52;
	test->b = 98;
	assert(test->a == 52);
	assert(test->b == 98);
	assert((size_t)test % sizeof(void*) == 0);


	float *fvalue = arena_alloc(&ar, sizeof(float));
	*fvalue = 0.125;
	assert(*fvalue == 0.125);

	for (int i = 0; i < 100000; i++) {
		arena_alloc(&ar, sizeof(int));
	}

	arena_free(&ar);

	printf("No errors are reported\n");


}
