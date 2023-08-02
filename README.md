## std

std is a improved standart library for C, it aims to be minimal and easy to use.
Current c standart does not have any data structures and does a poor job on handling
os dependent tasks. This library tries to cover parts C library doesn't. All files
are single header. Most of the files do not depend on each other, if they do it is
written in the table.

ds stands for data structure.
library    | lastest version | category | doc | description
--------------------- | ---- | -------- | --- | --------------------------------
**[filesystem.h](filesystem.h)** | 0.01 | sys | null | provides file/directory manipulation, creation deletion etc. Depends on extra.h
**[system.h](system.h)** | 0.01 | sys | null | provide os spesific functionalities, like reboot, poweroff, get number of cpu cores...
**[vector.h](vector.h)** | 0.01 | ds | null | vector library that accepts any type.
**[strvec.h](strvec.h)** | 0.01 | ds | [view](https://github.com/xcatalyst/std/blob/master/docs/strvec/) | vector library for strings
**[log.h](log.h)** | 0.01 | utility | [view](https://github.com/xcatalyst/std/blob/master/docs/log/) | thread safe logging library
**[string_view.h](string_view.h)** | 0.01 | utility | null | string view implementation for c
**[extra.h](extra.h)** | 0.01 | utility | null | extra utility library, includes compiler spesific macros.
**[file_format.h](file_format.h)** | 0.01 | utility | null | library for parsing file types, pe32, elf etc..
**[types.h](types.h)** | 0.01 | utility | null | better names for types than posix_t
