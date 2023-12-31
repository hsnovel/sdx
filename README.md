## sdx

sdx (Standard Extension) is an extended standard library for C, it aims to be minimal and easy to use.
It does not replace the standard library, it adds new features that are missing.
The current C standard does not have any data structures and does a poor job on handling
os dependent tasks. This library tries to cover parts C library doesn't.

on the status column:
"wip" means that the library is not finished or not ready to use,
may contain bugs and is not recommended to currently use on big projects.

"good" means that the library is either finished or passes the tests,
It does not guarantee that there are no bugs but means that the library
is in a usable state.

library    | lastest version | status | doc | description
--------------------- | ---- | -------- | --- | --------------------------------
**[filesystem.h](include/filesystem.h)** | 0.01 | wip | null | provides file/directory manipulation, creation deletion etc. Depends on extra.h
**[system.h](include/system.h)** | 0.01 | wip | null | provide os specific functionalities, like reboot, power-off, get number of CPU cores...
**[array.h](include/array.h)** | 0.01 | wip | null | array library that accepts any type.
**[strvec.h](include/strvec.h)** | 0.01 | wip | [view](https://github.com/xcatalyst/sdx/blob/master/docs/strvec/) | array library for strings
**[log.h](include/log.h)** | 0.01 | good | [view](https://github.com/xcatalyst/sdx/blob/master/docs/log/) | thread-safe logging library
**[string_view.h](include/string_view.h)** | 0.01 | good | null | string view implementation for c
**[extra.h](include/extra.h)** | 0.01 | wip | null | extra utility library, includes compiler spesific macros.
**[file_format.h](include/file_format.h)** | 0.01 | wip | null | library for parsing file types, pe32, elf etc..
**[types.h](include/types.h)** | 0.01 | good | null | better names for types than posix_t
**[arena.h](include/arena.h)** | 0.01 | wip | null | memory arena for c, experimental
**[benchmark.h](include/benchmark.h)** | 0.01 | wip | null | benchmark library for c, experimental
**[string_operations.h](include/string_operations.h)** | 0.01 | wip | null | string operation library
**[mem_debug.h](include/mem_debug.h)** | 0.01 | wip | null | Memory debugging library, [idea from this video](https://youtu.be/443UNeGrFoM?t=2988)

## Coding style
sdx follows [linux kernel coding style](https://www.kernel.org/doc/html/v4.10/process/coding-style.html) except couple of things.
* Comments can also be //. This library is C99 compatible so there should be no problem with that comment style.
