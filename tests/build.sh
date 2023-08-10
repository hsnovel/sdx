#!/bin/sh
mkdir -p tmp
gcc -std=c99 -Wall -Wextra -g3 array_test.c -o tmp/array_test
gcc -std=c99 -Wall -Wextra -g3 string_view_test.c -o tmp/string_view
gcc -std=c99 -Wall -Wextra -g3 arena_test.c -o tmp/arena
gcc -std=c99 -Wall -Wextra -g3 strvec_test.c -o tmp/strvec
