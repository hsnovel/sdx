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
		vector_push(&vec, &data);
		data++;
	}
	assert(*((int*)vector_get(&vec, 99999)) == 99999 && "Assertion failed checking index 99999");

	vector_free_item(&vec, 20);
	assert(((int*)vector_get(&vec, 20)) == NULL && "Assertion failed checkig index 20");

	int newdata = 12;
	vector_replace_item(&vec, 25, &newdata);
	assert(*((int*)vector_get(&vec, 25)) == 12 && "Assertion failed replaceing index 20");
	assert(*((int*)vector_get(&vec, 26)) == 26 && "Assertion failed checking index 26");
}
