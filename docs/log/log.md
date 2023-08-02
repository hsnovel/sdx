# log

log is a simple thread safe fast logging library for C.

![sreenshot](https://user-images.githubusercontent.com/53369750/215074252-5ad1e9aa-71ad-49e6-9809-bf18d4a7b6bf.png)

# Usage

Do not put "\n" at the end of the logging functions, they are added by default. \
If you want to disable logging define ```LOG_DISABLE``` before including the library.

Each level of logs can be disabled with ```LOG_DISABLE_<NAME_OF_LOG_LEVEL>``` macro, Example:
```#define LOG_DISABLE_TRACE```

You can use this preprocessor variables to control how library works. Do not
#define these, instead specify them to the compiler like `-DLOG_RELEASE`
otherwise these macros will probably not work.
```cpp
LOG_ENABLE_BOLD_COLORS /* Works only if LOG_DISABLE_COLOR is not set */
LOG_RELEASE /* Disables printing file properties and time */
LOG_DISABLE_COLORS /* Disable all colors for log information names */
```

Logging functions
```cpp
log_notag(...);
log_ok(...);
log_trace(...);
log_debug(...);
log_info(...);
log_warn(...);
log_error(...);
log_fatal(...);
```

Thread safety, which is not enabled by default.
```cpp
log_enable_thread_safe();   /* Enables thread safety globally  */
log_disable_thread_safe();  /* Disables thread safety globally */
```

Quiet mode, set to 0 by default, set quiet to 1 enable.
```cpp
log_set_quiet(unsigned int quiet); /* If enabled do not print to stderr. */
```

Set the minimum warning level.
```cpp
log_set_level(int level);
```

Output log to file. Note if you use ```log_append_file``` it will open the file in the append mode to prevent
loss of old data. If this is not what you want you can open the file in any mode you want and call
```log_append_fp```

```cpp
log_append_file(const char *path);
log_append_fp(FILE *file_descriptor);
log_detach_fp(); /* Remove file descriptor. Disable logging to file. */
```

## Note
Recently I changed a lot of code and didn't have any time to check if it works on Windows.
If the current version doesn't work on Windows please revert back to this [b56416ff26198a3d7d2fd803a7a6245aff6e5ded](https://github.com/xcatalyst/log/commit/b56416ff26198a3d7d2fd803a7a6245aff6e5ded) commit.

I also removed couple of features like being able to disable colors. I find it
to be really useless as almost all the terminals are able to print the colors
without a problem.
