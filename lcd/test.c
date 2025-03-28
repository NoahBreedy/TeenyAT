#include <stdio.h>
#include <stdlib.h>
#include "tigr.h"

int main() {
    // Create two windows with specified sizes
    Tigr *win1 = tigrWindow(400, 300, "Window 1", TIGR_FIXED);
    Tigr *win2 = tigrWindow(400, 300, "Window 2", TIGR_FIXED);

    if (!win1 || !win2) {
        printf("Error creating windows\n");
        return -1;
    }
    TPixel red = {255,0,0,255};
    TPixel blue = {0,0,255,255};
    // Main loop to keep windows open
    while (!tigrClosed(win1) && !tigrClosed(win2)) {
        tigrFillRect(win1,0,0,400,300,red);
        tigrFillRect(win2,0,0,400,300,blue);
        // Update the windows
        tigrUpdate(win1);
        tigrUpdate(win2);
    }

    // Cleanup and close windows
    tigrFree(win1);
    tigrFree(win2);

    return 0;
}
