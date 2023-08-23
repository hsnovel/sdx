#!/bin/sh
gcc -g3 -std=gnu11 -DLOG_DISABLE_ERROR_STRING -DLOG_RELEASE -Iinclude/ src/mem_debug.c src/arena.c src/array.c src/log.c src/strvec.c src/string_view.c src/filesystem.c src/system.c tests.c -o a
