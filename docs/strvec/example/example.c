#define STRVEC_IMPLEMENTATION
#include "../../../strvec.h"
#include <stdio.h>

int main()
{
	strvec arr;
	strvec_init(&arr);
	strvec_push(&arr, "test1");
	strvec_push(&arr, "string1");
	strvec_push(&arr, "hellostring");
	strvec_push(&arr, "kappa");
	strvec_clear(&arr, 2);
	printf("%s\n", strvec_get(&arr, 0));
	printf("%s\n", strvec_get(&arr, 1));
	printf("%s\n", strvec_get(&arr, 2));
	printf("%s\n", strvec_get(&arr, 3));

	/* This is how you should NOT use the next function,
	*  the os will clear the memory after you exit the
	*  program so calling free() is unnecesarry unless
	*  the program is running for a long time and you
	*  generate a string in the middle of the program
	*  and need to delete, do not use this before exiting
	*  the program it is unnecesarry. */
	strvec_delete_struct(&arr);
	return 0;
}
