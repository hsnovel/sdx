#!/bin/sh
mkdir -p tmp
gcc -std=c2x -Wall -Wextra -g3 vector_test.c -o tmp/vector_test
