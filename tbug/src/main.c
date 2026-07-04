#include <locale.h>

#define CLAY_IMPLEMENTATION
#include "clay.h"
#include "clay_colors.h"
#include "clay_renderer_tigr.h"

#include "gui/code_window.h"
#include "gui/header_window.h"
#include "gui/memory_window.h"
#include "gui/register_window.h"

#include "teenyat.h"

#define KEY_BACKSPACE 0x08

void handleKeyBoard(Tigr* ctx);

void HandleClayErrors(Clay_ErrorData errorData) {
    printf("%s\n", errorData.errorText.chars);
    switch(errorData.errorType) {
    }
}

Clay_ElementDeclaration outerContainerConfig = (Clay_ElementDeclaration) {
    .layout = { .layoutDirection = CLAY_TOP_TO_BOTTOM,
                            .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_GROW(0) },
                            .padding = {16,16,16,16},
                            .childGap = 16 },
    .backgroundColor = OUTER_COLOR
};

Clay_ElementDeclaration asmContainerConfig = (Clay_ElementDeclaration) {
    .layout = { .layoutDirection = CLAY_LEFT_TO_RIGHT,
                            .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_GROW(0) },
                            .padding = {16,16,16,16},
                            .childGap = 32 },
    .backgroundColor = HEADER_COLOR
};

Clay_ElementDeclaration memoryContainerConfig = (Clay_ElementDeclaration) {
    .layout = { .layoutDirection = CLAY_TOP_TO_BOTTOM,
                            .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_GROW(0) },
                            .padding = {16,16,16,16},
                            .childGap = 32 },
    .backgroundColor = HEADER_COLOR
};

int main(int argc, char* argv[]) {
    /* Require user to provide binary file */
    if(argc != 2) {
        printf("usage: tbug <bin_file>\n");
        return 1;
    }

    const char* file_name = argv[1];

    FILE* bin_file = fopen(file_name, "rb");
    if(!bin_file) {
        printf("Failed to open file: %s\n", file_name);
        return 1;
    }

    teenyat t;
    if(!tny_init_from_file(&t, bin_file, NULL, NULL)) {
        printf("Failed to initialize teenyAT!\n");
        return 1;
    }

    /* Try en_US.UTF-8 locale (my wsl defaults to C.UTF-8 :( */
    char *current_locale = setlocale(LC_NUMERIC, "en_US.UTF-8");
    if(!current_locale) {
        current_locale = setlocale(LC_NUMERIC, "");
    }

    uint64_t totalMemorySize = Clay_MinMemorySize();
    Clay_Arena arena = Clay_CreateArenaWithCapacityAndMemory(totalMemorySize, malloc(totalMemorySize));

    int screenWidth  = 1000;
    int screenHeight = 600;

    int mousePositionX, mousePositionY, isMouseDown;
    float mouseWheelX, mouseWheelY;
    Clay_Initialize(arena, (Clay_Dimensions) { screenWidth, screenHeight }, (Clay_ErrorHandler) { HandleClayErrors });
    Clay_SetMeasureTextFunction(Clay_Tigr_MeasureText, NULL);

    Tigr* win = tigrWindow(screenWidth, screenHeight, "Clay & Tigr", TIGR_AUTO);

    /* Create input box that can hold 6 characters
     *
     * This is because the longest string I want this
     * box to be able to hold is 0xFFFF & 65,535 though
     * you will only be able to inspect memory addresses
     *
     * 0x0000 -> 0x7FFF
     * */
    CREATE_INPUT_BOX(6);

    char* string_arena = calloc(5124, sizeof(char));

    uint64_t frame_cnt = 0;
    while(!tigrClosed(win) && !tigrKeyDown(win, TK_ESCAPE)) {

        handleKeyBoard(win);   // process keyboard input for input boxes

        int deltaTime = tigrTime(); // tigrTime return the time since it was last called
        tigrMouse(win, &mousePositionX, &mousePositionY, &isMouseDown);
        tigrScrollWheel(win, &mouseWheelX, &mouseWheelY);

        screenWidth = win->w;
        screenHeight = win->h;
        Clay_SetLayoutDimensions((Clay_Dimensions) { screenWidth, screenHeight });
        Clay_SetPointerState((Clay_Vector2) { mousePositionX, mousePositionY }, isMouseDown);
        Clay_UpdateScrollContainers(true, (Clay_Vector2) { mouseWheelX, mouseWheelY }, deltaTime);

        Clay_BeginLayout();

        CLAY(CLAY_ID("OuterContainer"), outerContainerConfig) {

            HeaderWindow();

            CLAY(CLAY_ID("AsmContainer"), asmContainerConfig) {

                CodeWindow();

                CLAY(CLAY_ID("MemoryContainer"), memoryContainerConfig) {

                    int offset = RegisterWindow(string_arena, &t);

                    MemoryWindow(string_arena + offset);

                }

            }

        }

        Clay_RenderCommandArray renderCommands = Clay_EndLayout(deltaTime);
        Clay_Tigr_Render(renderCommands, win);

        tigrUpdate(win);

    }

    FREE_INPUTS();
    free(string_arena);
    tigrFree(win);
}

void handleKeyBoard(Tigr* ctx) {
    for (;;) {
        int c = tigrReadChar(ctx);
        if (c == 0) {
           break;
        }

        if(c == ' ') {
            int32_t val = GET_INPUT_VALUE(0);
            printf("Value: %'d\n", val);
        }

        /* Handle text input */
        if(current_input != -1) {
            uint32_t cursor = input_boxes[current_input]->cursor;
            uint32_t max_size = input_boxes[current_input]->size;

            /* Backspace */
            if(c == KEY_BACKSPACE && cursor > 0) {
                cursor--;
                input_boxes[current_input]->value[cursor] = '\0';
            }else if(cursor < max_size && c != KEY_BACKSPACE) {
                input_boxes[current_input]->value[cursor] = c;
                cursor++;
            }

            input_boxes[current_input]->cursor = cursor;
        }

    }
    return;
}
