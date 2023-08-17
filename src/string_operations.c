#include "string_operations.h"

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
