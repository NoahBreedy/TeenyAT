#ifndef __MEMORY_WINDOW__
#define __MEMORY_WINDOW__

#include <stdio.h>
#include <string.h>

#include "clay.h"
#include "input.h"
#include "clay_colors.h"

#define MAX_BOXES 100

/* typesafe safe macro for min of two values */
#define min(a,b) \
       ({ typeof (a) _a = (a); \
           typeof (b) _b = (b); \
         _a > _b ? _b : _a; })

void MemoryWindow(char* string_arena);

#endif /* __MEMORY_WINDOW__ */

