#include "gui/memory_window.h"

/* The reason all of the rendering isint handled in
 * the input.c file is because I just want that header
 * to be responsible for storing the data of our input
 * boxes
 */

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

void MemoryWindow() {
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
