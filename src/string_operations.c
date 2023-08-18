#include "string_operations.h"
#include <stdlib.h>

char *skip_whitespace(char *in)
{
	char *result = in;
	for (;;) {
		switch(*result) {
		case ' ':
		case '\r':
		case '\t':
		case '\n': {
			result++;
		} break;
		default:
			return result;
		}
	}
	return result;
}

char *skip_untill_whitespace(char *in)
{
	char *result = in;
	for (;;) {
		switch(*result) {
		case ' ':
		case '\r':
		case '\t':
		case '\n': {
			return result;
		} break;
		default:
			result++;
		}
	}
	return result;

}

char *skip_untill_next_word(char *in)
{
	char *result = in;
	for (;;) {
		switch(*result) {
		case ' ':
		case '\r':
		case '\t':
		case '\n': {
			goto cleanup;
		} break;
		default:
			result++;
		}
	}

 cleanup:
	result = skip_whitespace(result);
	return result;

}

char *skip_untill_x_char(char *in, char x)
{
	char *result = in;
	while (*result != x)
		result++;

	return result;
}

// Return a null terminated string from a part of
// a string
char *get_substring(char *ptr, size_t size)
{
	char *result = malloc(size);
	memcpy(result, ptr, size);
	result[size] = '\0';
	return result;
}

// This function does not check if dest has enough size
// expects the caller to check for it, if they are NEVER
// going to copy a big portion of the string such a check
// might be unncesesary.
void get_substring_dest(char *ptr, size_t size, char *dest)
{
	memcpy(dest, ptr, size);
	dest[size] = '\0';
}

char *skip_to_nextline(char *in)
{
	char *result = in;
	while (*result != '\n')
		result++;
	return ++result;
}
