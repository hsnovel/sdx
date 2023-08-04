#!/bin/sh
mkdir -p tmp
# gcc -std=c99 -Wall -Wextra -g3 vector_test.c -o tmp/vector_test
gcc -std=c99 -Wall -Wextra -g3 string_view_test.c -o tmp/string_view
