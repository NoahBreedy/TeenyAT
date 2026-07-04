#include "gui/memory_window.h"

Clay_ElementDeclaration memoryBoxConfig = (Clay_ElementDeclaration) {
    .layout = { .layoutDirection = CLAY_LEFT_TO_RIGHT,
                .sizing = { .width = CLAY_SIZING_GROW(0),
                            .height = CLAY_SIZING_FIXED(20) },
                .padding  = {8, 8, 8, 8},
                .childAlignment = { .x = CLAY_ALIGN_X_CENTER },
                .childGap = 16 },
    .cornerRadius = { 4, 4, 4, 4 },
    .border = { .width = { 1, 1, 1, 1, 0 }, .color = COLOR_WHITE },
    .backgroundColor = BOX_2_COLOR 
};

/* The reason all of the rendering isint handled in
 * the input.c file is because I just want that header
 * to be responsible for storing the data of our input
 * boxes
 */
Clay_ElementDeclaration inputBoxConfig = (Clay_ElementDeclaration) {
    .layout = {
        .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(50) }
    },
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

void MemoryBoxComponent(char* my_str, int address) {

    snprintf(my_str, sizeof(char) * 9, "0x%04X |", address);

    Clay_String reg_txt = (Clay_String){.isStaticallyAllocated = false, .length = 9, .chars = my_str};

    int my_num = 65535;
    my_str+=9;
    snprintf(my_str, sizeof(char) * 10, "s: %'d", my_num);
    Clay_String s_txt = (Clay_String){.isStaticallyAllocated = false, .length = 10, .chars = my_str};

    my_str+=10;
    snprintf(my_str, sizeof(char) * 10, "u: %'u", (uint16_t)my_num);
    Clay_String u_txt = (Clay_String){.isStaticallyAllocated = false, .length = 10, .chars = my_str};

    my_str+=10;
    snprintf(my_str, sizeof(char) * 10, "char: %c", (char)my_num);
    Clay_String c_txt = (Clay_String){.isStaticallyAllocated = false, .length = 10, .chars = my_str};

    const int h_size = 8;
    const int p_size = 8;
    CLAY_AUTO_ID(memoryBoxConfig) {
        CLAY_TEXT(reg_txt, { .fontSize = h_size, .textColor = COLOR_WHITE });
        CLAY_TEXT(s_txt, { .fontSize = p_size, .textColor = COLOR_WHITE });
        CLAY_TEXT(u_txt, { .fontSize = p_size, .textColor = COLOR_WHITE });
        CLAY_TEXT(c_txt, { .fontSize = p_size, .textColor = COLOR_WHITE });
    }
}



void MemoryWindow(char* string_arena) {
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
                .border = { .width = { 16, 16, 16, 16, 0 }, .color = COLOR_WHITE },
                .backgroundColor = BOX_2_COLOR }) {

            /* Place input box in the searchbar */
            InputBoxComponent("Mem Address: ", 0);

        }

        CLAY(CLAY_ID("MemoryBox"), { 
                .layout = { .layoutDirection = CLAY_TOP_TO_BOTTOM,
                            .sizing = { .width = CLAY_SIZING_GROW(0),
                                        .height = CLAY_SIZING_GROW(0) },
                            .padding  = {16, 16, 16, 16},
                            .childGap = 16 },
                .cornerRadius = { 4, 4, 4, 4 },
                .clip = { .vertical = true, .childOffset = Clay_GetScrollOffset() },
                .backgroundColor = BOX_2_COLOR}) {
           
            uint16_t base_addr = GET_INPUT_VALUE(0); 
            if(base_addr > 0x7FFF) base_addr = 0x7FFF;
            uint16_t total_boxes = min(100, (uint16_t)(0x8000 - base_addr));
            for(int i = 0; i < total_boxes; i++) {
                MemoryBoxComponent(string_arena + (39 * i), base_addr + i);
            }
            
        }
    }
}
