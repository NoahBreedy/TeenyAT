/* For memoery and most likely asm portion im going to need to keep track
 * of how many elements im going to display */

#define CLAY_IMPLEMENTATION
#include "clay.h"

#include <string.h>

#include "input.h"
#include "clay_renderer_tigr.h"

#define KEY_BACKSPACE 0x08

const Clay_Color OUTER_COLOR = (Clay_Color) {8, 12, 25, 255};
const Clay_Color HEADER_COLOR = (Clay_Color) {29, 35, 42, 255};
const Clay_Color BOX_COLOR = (Clay_Color) {67, 84, 105, 255};
const Clay_Color BOX_2_COLOR = (Clay_Color) {46, 63, 84, 255};
const Clay_Color COLOR_ORANGE = (Clay_Color) {225, 138, 50, 255};
const Clay_Color COLOR_BLUE = (Clay_Color) {0, 50, 215, 255};
const Clay_Color COLOR_RED = (Clay_Color) {255, 50, 10, 255};
const Clay_Color COLOR_GREEN = (Clay_Color) {50, 250, 60, 255};

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

Clay_ElementDeclaration inputBoxConfig = (Clay_ElementDeclaration) {
    .layout = {
        .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(50) }
    }
};

void HandleInputBoxInteraction(Clay_ElementId elementId, Clay_PointerData pointerInfo, void *userData) {
    InputBox *inputData = (InputBox *)userData;

    // Pointer state allows you to detect mouse down / hold / release
    if (pointerInfo.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        TURN_OFF_FOCUS();
        inputData->focused = true;
        SET_CURRENT_FOCUSED_ID(inputData->id);
    }
}

void InputBoxComponent(char* label, int id) {
    Clay_String box_txt = (Clay_String){.isStaticallyAllocated = false, .length = input_boxes[id]->size, .chars = input_boxes[id]->value};
    Clay_String label_txt = (Clay_String){.isStaticallyAllocated = true, .length = strlen(label), .chars = label};
    CLAY_AUTO_ID(inputBoxConfig) {
        Clay_OnHover(HandleInputBoxInteraction, input_boxes[id]);
        CLAY_TEXT(label_txt, { .fontSize = 16, .textColor = {255, 255, 255, 255} });
        CLAY_TEXT(box_txt, { .fontSize = 16, .textColor = {255, 255, 255, 255} });
    }
}

void handleKeyBoard(Tigr* ctx) {
    for (;;) {
        int c = tigrReadChar(ctx);
        if (c == 0) {
           break;
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

int main() {
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

            CLAY(CLAY_ID("Header"), {
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

            CLAY(CLAY_ID("AsmContainer"), asmContainerConfig) {

                CLAY(CLAY_ID("DisassemblyWindow"), {
                        .layout = { .layoutDirection = CLAY_LEFT_TO_RIGHT,
                                    .sizing = { .width = CLAY_SIZING_FIXED(500),
                                                .height = CLAY_SIZING_GROW(0) },
                                    .padding = CLAY_PADDING_ALL(16),
                                    .childGap = 16 },
                        .backgroundColor = BOX_COLOR }) {

                }

                CLAY(CLAY_ID("MemoryContainer"), memoryContainerConfig) {

                    CLAY(CLAY_ID("RegisterWindow"), {
                            .layout = { .layoutDirection = CLAY_TOP_TO_BOTTOM,
                                        .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(150) },
                                        .padding = CLAY_PADDING_ALL(0),
                                        .childGap = 16 },
                            .backgroundColor = BOX_COLOR }) {

                    }

                    CLAY(CLAY_ID("MemoryWindow"), {
                            .layout = { .layoutDirection = CLAY_TOP_TO_BOTTOM,
                                        .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_GROW(0) },
                                        .padding = CLAY_PADDING_ALL(16),
                                        .childGap = 16 },
                            .backgroundColor = BOX_COLOR }) {

                        CLAY(CLAY_ID("SearchBar"), {
                                .layout = { .layoutDirection = CLAY_LEFT_TO_RIGHT,
                                            .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(30) },
                                            .padding = CLAY_PADDING_ALL(8),
                                            .childGap = 8 },
                                .backgroundColor = BOX_2_COLOR }) {

                            /* Place input box in the searchbar */
                            InputBoxComponent("Mem Address: ", 0);

                        }

                    }

                }

            }

        }

        Clay_RenderCommandArray renderCommands = Clay_EndLayout(deltaTime);
        Clay_Tigr_Render(renderCommands, win);

        tigrUpdate(win);
    }

    FREE_INPUTS();
    tigrFree(win);
}
