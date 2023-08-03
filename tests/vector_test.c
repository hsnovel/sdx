#include <stdio.h>
#include <assert.h>
#define VECTOR_IMPLEMENTATION
#include "../vector.h"

int main()
{
	vector vec;
	vector_init(&vec, sizeof(int));

	int data = 0;
	for(int i = 0; i < 100000; i++) {
		data++;
		vector_push(&vec, &data);
	}
	assert(*((int*)vector_get(&vec, 99999)) == 100000 && "Assertion failed checking index 99999");

	vector_free_item(&vec, 20);
	assert(*((int*)vector_get(&vec, 20)) == 0 && "Assertion failed checkig index 20");
}
