## std

std is a improved standart library for C, it aims to be minimal and easy to use.
Current c standart does not have any data structures and does a poor job on handling
os dependent tasks. This library tries to cover parts C library doesn't. All files
are single header. They do not depend on each other, you can safely use them as a
single file.

ds stands for data structure.
library    | lastest version | category | LoC | description
--------------------- | ---- | -------- | --- | --------------------------------
**[filesystem.h](filesystem.h)** | 0.01 | sys | 186 | provides file/directory manipulation, creation deletion etc..
**[system.h](system.h)** | 0.01 | sys | 134 | provide os spesific functionalities, like reboot, poweroff, get number of cpu cores...
**[vector.h](vector.h)** | 0.01 | ds | 84 | vector library that accepts any type.
**[strvec.h](strvec.h)** | 0.01 | ds | 122 | vector library for strings
**[log.h](log.h)** | 0.01 | utility | 304 | thread safe logging library
**[string_view.h](string_view.h)** | 0.01 | utility | 243 | string view implementation for c
**[extra.h](extra.h)** | 0.01 | utility | 69 | extra utility library, includes compiler spesific macros.
**[file_format.h](file_format.h)** | 0.01 | utility | 722 | library for parsing file types, pe32, elf etc..
