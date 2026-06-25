#include "gui/register_window.h"

void RegisterWindow() {
    CLAY(CLAY_ID("RegisterWindow"), {
            .layout = { .layoutDirection = CLAY_TOP_TO_BOTTOM,
                        .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(150) },
                        .padding = CLAY_PADDING_ALL(0),
                        .childGap = 16 },
            .backgroundColor = BOX_COLOR }) {

    }
}
