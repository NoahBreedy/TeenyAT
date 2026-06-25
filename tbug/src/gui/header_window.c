#include "gui/header_window.h"

void HeaderWindow() {
    CLAY(CLAY_ID("HeaderWindow"), {
        .layout = { .layoutDirection = CLAY_LEFT_TO_RIGHT,
                    .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(80) },
                    .padding = CLAY_PADDING_ALL(0),
                    .childGap = 16,
                    .childAlignment = { .y = CLAY_ALIGN_Y_CENTER }
                },
        .backgroundColor = HEADER_COLOR }) {

            CLAY(CLAY_ID("ProfilePicture"), {
                    .layout = {
                        .sizing = {
                            .width = CLAY_SIZING_FIXED(70),
                            .height = CLAY_SIZING_FIXED(86) }
                        },
                    .image = { .imageData = "resources/leroy.png" } }) {

            }

            CLAY_TEXT(CLAY_STRING("TEENY BUG"), { .fontSize = 24, .textColor = {255, 255, 255, 255} });
    }
}
