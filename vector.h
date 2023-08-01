#ifndef VECTOR_H
#define VECTOR_H

/*
 * on fail 0 is returned, otherwise
 * 1 is returned.
 */

#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define VECTOR_INITIAL_CAP 256
#define VECTOR_INITIAL_BACKTRACK 32

typedef struct {
	size_t cap;   /* in bytes */
	size_t index; /* counter in numbers */
	size_t itemsize; /* in bytes */
	unsigned char *data;
} vector;

int vector_init(vector *vector, int size);
int vector_push(vector *vector, void *data);
void *vector_get(vector *vector, int index);
void vector_pop(vector *vector);

#endif /* VECTOR_H */

#ifdef VECTOR_IMPLEMENTATION

int vector_init(vector *vector, int size)
{
	vector->cap = VECTOR_INITIAL_CAP;
	vector->index = 0;
	vector->itemsize = size;
	vector->data = malloc(VECTOR_INITIAL_CAP);
	if (vector->data == NULL)
		return 0;
}

int vector_push(vector *vector, void *data)
{
	if (vector->cap <= (vector->index * vector->itemsize) + vector->itemsize) {
		unsigned char *tmp = malloc(vector->cap * 2);
		vector->cap = vector->cap * 2;
		if (tmp == NULL)
			return 0;
		memcpy(tmp, vector->data, vector->index * vector->itemsize);
		vector->data = tmp;
	}
	memcpy(vector->data + (vector->index * vector->itemsize), data, vector->itemsize);
	vector->index++;
	return 1;
}

void *vector_get(vector *vector, int index)
{
	assert(vector->index >= index);
	return vector->data + (index * vector->itemsize);
}

int vector_free_item(vector *vector, int index)
{
	memset(vector->data + (index * vector->itemsize), 0, vector->itemsize);
}

void vector_free(vector *vector)
{
	free(vector->data);
	vector->cap = 0;
	vector->itemsize = 0;
	vector->index = 0;
}

void vector_pop(vector *vector)
{
	vector_free_item(vector, vector->index - 1);
	vector->index--;
}

#endif /* VECTOR_IMPLEMENTATION */
