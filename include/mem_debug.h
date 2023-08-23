#ifndef MEM_DEBUG_H
#define MEM_DEBUG_H

#include <stdlib.h>
#include <stddef.h>

void mem_debug_init();
void mem_debug_print();
void *mem_debug_malloc(size_t num, size_t line, char *file);
void *mem_debug_realloc(void *ptr, size_t newsize, size_t line, char *file);
void mem_debug_free(void *ptr, size_t line, char *file);

#if defined(MEM_DEBUG_ENABLE) && !defined(MEM_DEBUG_DISABLE)
#define malloc(x) mem_debug_malloc(x, __LINE__, __FILE__)
/*
 * #define realloc(x,y ) mem_debug_realloc(x, y, __LINE__, __FILE__)
 */
#define free(x) mem_debug_free(x, __LINE__, __FILE__)
#endif

#ifndef MEM_DEBUG_OVERRITE_NUM
#define MEM_DEBUG_OVERRITE_NUM 256
#endif

#ifndef MEM_DEBUG_OVERRITE_MAGIC_NUM
#define MEM_DEBUG_OVERRITE_MAGIC_NUM 154
#endif

#endif
