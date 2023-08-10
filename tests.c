#include "filesystem.h"
#include "extra.h"
#include <stdio.h>
#include "array.h"
#include "arena.h"
#include "log.h"
#include "strvec.h"
#include "string_view.h"
#include "system.h"

typedef struct {
	int a;
	int b;
	int c;
	int d;
	int e;
	int f;
	int g;
} test_struct;

#include <stdint.h>

/*
 * #define STRING_VIEW_IMPLEMENTATION
 * #include "string_view.h"
 */

void test_arena()
{
	printf("======= ARENA TEST START\n");
	arena ar;
	arena_init(&ar);
	int *data = arena_alloc(&ar, sizeof(int));
	printf("offset: %d\n", (size_t)data % sizeof(void*));
	*data = 20;
	printf("%d\n", *data);

	char *data2 = arena_alloc(&ar, sizeof(char));
	*data2 = 12;
	printf("%c\n", *data2);

	arena_align_next_block(&ar);
	test_struct *test = arena_alloc(&ar, sizeof(test_struct));
	test->a = 52;
	test->b = 98;
	printf("%d, %d\n", test->a, test->b);
	printf("offset: %d\n", (size_t)test % sizeof(void*));


	float *fvalue = arena_alloc(&ar, sizeof(float));
	*fvalue = 0.125;
	printf("%f\n", *fvalue);

	arena_free(&ar);

	printf("======= ARENA TEST END\n\n\n");
}

void test_log()
{
	printf("======= LOG TEST START\n");
	log_notag("TEST INFO");
	log_ok("TEST OK");
	log_trace("TEST TRACE");
	log_debug("TEST DEBUG");
	log_info("TEST INFO");
	log_warn("TEST WARN");
	log_error("TEST ERRRO");
	log_fatal("TEST FATAL");

	printf("======= LOG TEST END\n\n\n");
}

void test_array()
{
	printf("======= ARRAY TEST START\n");
	array vec;
	array_init(&vec, sizeof(int));

	int data = 0;
	for(int i = 0; i < 100000; i++) {
		array_push(&vec, &data);
		data++;
	}
	assert(*((int*)array_get(&vec, 99999)) == 99999 && "Assertion failed checking index 99999");

	assert(*((int*)array_get(&vec, 20)) == 20 && "Assertion failed checkig index 20");

	array_free_item(&vec, 20);
	assert(*((int*)array_get(&vec, 20)) != 20 && "Assertion failed checkig index 20");

	int newdata = 12;
	array_replace_item(&vec, 25, &newdata);
	assert(*((int*)array_get(&vec, 25)) == 12 && "Assertion failed replaceing index 20");
	assert(*((int*)array_get(&vec, 26)) == 26 && "Assertion failed checking index 26");

	printf("No errors reported\n");

	printf("======= ARRAY TEST END\n\n\n");
}

void test_string_view()
{
	printf("======= STRING VIEW TEST START\n");
	fs_file file = fs_read_file("resources/string_view_file.txt", "r+");
	string_view view = sv_from_parts(file.contents, file.size);
	string_view next_line = sv_next_line(view);
	printf(SV_Fmt, SV_Arg(next_line));
	printf("======= STRING VIEW TEST END\n\n\n");

}

void test_fs()
{
	printf("======= FILESYSTEM TEST START\n");

	char cwd[500];
	/*
	 * getcwd(cwd, sizeof(cwd));
	 */
	if (fs_cwd(cwd, 500) == 0)
		fprintf(stderr, "Unable to get cwd\n");
	else
		printf("CWD: %s\n", cwd);

	char *test_file_path = xasprintf("%s/resources/access_time.txt", cwd);
	printf("test_file_path: %s\n", test_file_path);

	struct tm* current_time;
	fs_ftime_info time;
	if (fs_file_time(test_file_path, &time) == 0)
		printf("%s\n", strerror(errno));

	time_t tx =  time.last_modify;

	char date[16];
	current_time = localtime(&tx);
	date[strftime(date, sizeof(date), "%H:%M:%S", current_time)] = '\0';
	printf("Last modify date: %s\n", date);

	free(test_file_path);
	printf("======= FILESYSTEM TEST END\n\n\n");
}

void test_strvec()
{
	printf("======= STRVEC TEST START\n");
	strvec vec;
	if (!strvec_init(&vec)) {
		printf("Unable to initialize strvec\n");
		return;
	}
	int index1 = strvec_push(&vec, "test string1");
	int index2 = strvec_push(&vec, "test string2");
	int index3 = strvec_push(&vec, "test string3");

	for(int i = 0; i < 1000; i++) {
		strvec_push(&vec, "placeholder");
	}

	int index4 = strvec_push(&vec, "test string 4");

	for(int i = 0; i < 10000; i++) {
		strvec_push(&vec, "placeholder");
	}

	int index5 = strvec_push(&vec, "test string 5");

	char *str1 = strvec_get(&vec, index1);
	char *str2 = strvec_get(&vec, index2);
	char *str3 = strvec_get(&vec, index3);
	char *str4 = strvec_get(&vec, index4);
	char *str5 = strvec_get(&vec, index5);

	printf("str1: %s\n", str1);
	printf("str2: %s\n", str2);
	printf("str3: %s\n", str3);
	printf("str4: %s\n", str4);
	printf("str5: %s\n", str5);

	printf("======= STRVEC TEST END\n\n\n");
}

void test_system()
{
	printf("======= SYSTEM TEST START\n");
	if (sys_is_debugger_attached()) {
		printf("debugger is attached to current process\n");
	}
	printf("======= SYSTEM TEST END\n\n\n");
}

int main()
{
	test_log();
	test_arena();
	test_string_view();
	test_array();
	test_system();
	test_fs();
	/*
	 * test_strvec();
	 */

	return 0;
}
