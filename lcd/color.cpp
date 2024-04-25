#include "color.h"

#include <iostream>
#include <cmath>

void HSVtoRGB(int H, unsigned char *red, unsigned char *green, unsigned char *blue) {
    // static const int n_for_gray4_code[] =          {0,  1,  3,  2,   7,   6,  4,  5,  15,  14,  12,  13,   8,   9,  11,  10};
    // multply the above by 17 to step through 0...255 intensity
    static const unsigned char gray4_to_intensity[] = {0, 17, 51, 34, 119, 102, 68, 85, 255, 238, 204, 221, 136, 153, 187, 170}; 
    int gray_code = H ^ (H >> 1);
    unsigned char r_gray = ((unsigned)gray_code >> 8) & ~(~0U << 4);
    unsigned char g_gray = ((unsigned)gray_code >> 4) & ~(~0U << 4);
    unsigned char b_gray = ((unsigned)gray_code >> 0) & ~(~0U << 4);
    *red = gray4_to_intensity[r_gray];
    *green = gray4_to_intensity[g_gray];
    *blue = gray4_to_intensity[b_gray];

    return;
}
#if 0
void HSVtoRGB(int H, unsigned char *red, unsigned char *green, unsigned char *blue)
{

   // Prevent bad values
   H = H % COLORS_IN_PALLETE;

   int section = (int)std::floor(H / COLORS_PER_SECTION);

   // init to all zero which should show black if something is going very wrong
   *red = 0;
   *green = 0;
   *blue = 0;

   int remainder = (int)std::floor(H) % COLORS_PER_SECTION;

   switch(section) {
   case 0:
      // just increasing blue
      *blue = (int)std::round((remainder * RGB_RESOLUTION) / COLORS_PER_SECTION);
      break;
   case 1:
      // max blue increasing green
      *blue = RGB_MAX_VALUE;
      *green = (int)std::round((remainder * RGB_RESOLUTION) / COLORS_PER_SECTION);
      break;
   case 2:
      // max green decreasing blue
      *green = RGB_MAX_VALUE;
      *blue = RGB_MAX_VALUE - (int)std::round((remainder * RGB_RESOLUTION) / COLORS_PER_SECTION);
      break;
   case 3:
      // max green increasing red
      *green = RGB_MAX_VALUE;
      *red = (int)std::round((remainder * RGB_RESOLUTION) / COLORS_PER_SECTION);
      break;
   case 4:
      // max red and green increasing blue
      *red = RGB_MAX_VALUE;
      *green = RGB_MAX_VALUE;
      *blue = (int)std::round((remainder * RGB_RESOLUTION) / COLORS_PER_SECTION);
      break;
   case 5:
      // max red and blue decreasing green
      *red = RGB_MAX_VALUE;
      *blue = RGB_MAX_VALUE;
      *green = RGB_MAX_VALUE - (int)std::round((remainder * RGB_RESOLUTION) / COLORS_PER_SECTION);
      break;
   case 6:
      // max red decreasing blue
      *red = RGB_MAX_VALUE;
      *blue = RGB_MAX_VALUE - (int)std::round((remainder * RGB_RESOLUTION) / COLORS_PER_SECTION);
      break;
   case 7:
      // decreasing red
      //  max red decreasing blue
      *red = RGB_MAX_VALUE - (int)std::round((remainder * RGB_RESOLUTION) / COLORS_PER_SECTION);
      break;
   case 8: // shouldn't really end up here either
   default:
      // Something went wrong ...
      std::cout << "error with color section: " << section << std::endl;
      std::cout << "remainder ended up: " << remainder << std::endl;
      break;
   }
}
#endif