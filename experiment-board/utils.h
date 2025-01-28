#ifndef __UTILS_H__
#define __UTILS_H__

#include <cstdint>
#include "tigr.h"

Tigr* img_copy(Tigr*);
Tigr* img_flip_horizontal(Tigr*);
Tigr* img_flip_vertical(Tigr*);
Tigr* img_rotate_180(Tigr*);
int abs_mod(int,int);
bool point_rect(float,float,float,float,float,float);
uint64_t calibrate_teeny_clock(uint64_t target_us);

#endif  /* __UTILS_H__ */ 