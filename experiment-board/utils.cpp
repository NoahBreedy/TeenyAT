#include <iostream>
#include <string>
#include "utils.h"

#if defined(_WIN64) || defined(_WIN32) 
    #include <windows.h>
    // Query windows high resolution clock for time
    uint64_t us_clock() {
        LARGE_INTEGER frequency, counter;
        QueryPerformanceFrequency(&frequency);
        QueryPerformanceCounter(&counter);
        // Convert to micorseconds
        return (counter.QuadPart * 1000000LL) / frequency.QuadPart;
    }
#else
    #include <time.h>
    #include <unistd.h>
    uint64_t us_clock() {
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        // Convert to micorseconds
        return (uint64_t)(ts.tv_sec * 1000000LL + ts.tv_nsec / 1000LL);
    }
#endif

Tigr* img_flip_horizontal(Tigr* image) {
    Tigr* flipped = tigrBitmap(image->w, image->h);
    for (int y = 0; y < image->h; y++) {
        for (int x = 0; x < (image->w / 2) + 1; x++) {
            TPixel temp = tigrGet(image, x, y);
            tigrPlot(flipped, x, y, tigrGet(image, image->w - 1 - x, y));
            tigrPlot(flipped, image->w - 1 - x, y, temp);
        }
    }
    return flipped;
}

Tigr*  img_flip_vertical(Tigr* image) {
    Tigr* flipped = tigrBitmap(image->w, image->h);
    for (int y = 0; y < (image->h / 2) + 1; y++) {
        for (int x = 0; x < image->w; x++) {
            TPixel temp = tigrGet(image, x, y);
            tigrPlot(flipped, x, y, tigrGet(image, x, image->h - 1 - y));
            tigrPlot(flipped , x, image->h - 1 - y, temp);
        }
    }
    return flipped;
}

Tigr* img_rotate_180(Tigr* image) {
    
    Tigr* rotated = tigrBitmap(image->w, image->h);

    for (int y = 0; y < image->h / 2; y++) {
        for (int x = 0; x < image->w; x++) {
            /* Swap pixel (x, y) with (image->w - 1 - x, image->h - 1 - y) */
            TPixel temp = tigrGet(image, x, y);
            tigrPlot(rotated, x, y, tigrGet(image, image->w - 1 - x, image->h - 1 - y));
            tigrPlot(rotated, image->w - 1 - x, image->h - 1 - y, temp);
        }
    }
    
    /* If the height is odd, handle the middle row separately */
    if (image->h % 2 != 0) {
        int middleY = image->h / 2;
        for (int x = 0; x < image->w / 2; x++) {
            /* Swap the middle row pixels */
            TPixel temp = tigrGet(image, x, middleY);
            tigrPlot(rotated, x, middleY, tigrGet(image, image->w - 1 - x, middleY));
            tigrPlot(rotated, image->w - 1 - x, middleY, temp);
        }
    }

    return rotated;
}

int abs_mod(int a,int b){
    return (a+b) % b;
}

/* Taken from jefferythompson.org */
bool point_rect(float px, float py, float rx, float ry, float rw, float rh) {

  // is the point inside the rectangle's bounds?
  if (px >= rx &&        // right of the left edge AND
      px <= rx + rw &&   // left of the right edge AND
      py >= ry &&        // below the top AND
      py <= ry + rh) {   // above the bottom
        return true;
  }
  return false;
}

/**
 *  This function will calibrate per host machine a wait time variable
 *  to be used at the end of every tny_clock to acheive close to 1mhz
 *  clock rate (1 us per clock) 
 * 
 *  The final time thats returned is based on which ever wait value had the 
 *  longest chain of target_us times which I think should be fine.....
 *  
 */ 
uint64_t calibrate_teeny_clock(uint64_t target_us){

    uint64_t wait_time = 32000000;
    uint64_t highest_accuracy_wait = wait_time;
    int highest_accuracy_count = 0;
    int accuracy_count = 0;
    int flag = 0;

    /* Simulate 1 million clock cycles */
    for(int i = 0; i < 1000000; i++){
        
        uint64_t t1 = us_clock(); // time stamp 1

        /* tny_clock(t); */ 

        /* Busy wait to hopefully fix the cycle rate */
        for(volatile uint64_t i = 0; i < wait_time; i++){
        }

        uint64_t t2 = us_clock(); // time stamp 2

        uint64_t elapsed = t2 - t1; //  total duration 

        /* Update our wait time accordingly */
        if(elapsed > target_us){
            flag = (wait_time / 10) * -1; // Lower wait time my 1/10
        }else if(elapsed < target_us){
            flag = (wait_time / 10); // raise time my 1/10
        }else{
            accuracy_count++;
            flag = 0;  // no change were inbounds :)
        }

        /* Track our longest chain of target_us clock cycles */
        if(flag){
            if(accuracy_count > highest_accuracy_count){
                highest_accuracy_count = accuracy_count;
                highest_accuracy_wait = wait_time;
            }
            accuracy_count = 0;
            wait_time += flag;
        }

    }
 
    /* Fail safe for devices that cannot hit time rate */
    if(!highest_accuracy_count) highest_accuracy_wait = 0;

    return highest_accuracy_wait;
}