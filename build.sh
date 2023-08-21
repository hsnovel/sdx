#!/bin/sh
gcc -g3 -std=c2x -DLOG_DISABLE_ERROR_STRING -DLOG_RELEASE -Iinclude/ tests.c src/arena.c src/array.c src/log.c src/strvec.c src/string_view.c src/filesystem.c src/system.c -o a && ./a
