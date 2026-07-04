#ifndef __HEADER_WINDOW__
#define __HEADER_WINDOW__

#include <string.h>
#include <stdio.h>

#include "clay.h"
#include "clay_colors.h"

#include "teenyat.h"

extern bool stop_mode;
extern bool step_mode;
extern bool reset_mode;

int HeaderWindow(char* string_arena, teenyat* t);

#endif /* __HEADER_WINDOW__ */

