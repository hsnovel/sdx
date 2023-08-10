#!/bin/sh
gcc -g3 -std=c2x -Iinclude/ tests.c src/arena.c src/array.c src/log.c src/strvec.c src/string_view.c src/filesystem.c src/system.c -o a && ./a
# g++ -O3 main.cpp -o b
