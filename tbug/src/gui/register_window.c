#include "gui/register_window.h"

Clay_ElementDeclaration registerBoxConfig(Clay_Color col) {
    return (Clay_ElementDeclaration) {
    .layout = { .layoutDirection = CLAY_TOP_TO_BOTTOM,
                .sizing = { .width = CLAY_SIZING_FIXED(150),
                            .height = CLAY_SIZING_FIXED(70) },
                .padding  = {8, 8, 0, 8},
                .childGap = 10 },
    .cornerRadius = { 12, 12, 12, 12 },
    .border = { .width = { 1, 1, 1, 1, 0 }, .color = col },
    .backgroundColor = BOX_2_COLOR };
}

void RegisterBoxComponent(char* my_str, int id, tny_word reg) {

    const Clay_Color col_index[8] = {COLOR_RED, COLOR_GREEN, COLOR_BLUE, COLOR_YELLOW, COLOR_ORANGE, COLOR_VIOLET, COLOR_CYAN, COLOR_WHITE};

    char* reg_index[8] = {"A", "B", "C", "D", "E", "PC", "SP", "Z"};

    snprintf(my_str, sizeof(char) * 8, "Reg %s:", reg_index[id]);

    Clay_String reg_txt = (Clay_String){.isStaticallyAllocated = false, .length = 8, .chars = my_str};

    my_str+=8;
    snprintf(my_str, sizeof(char) * 10, "s: %'d", reg.s);
    Clay_String s_txt = (Clay_String){.isStaticallyAllocated = false, .length = 10, .chars = my_str};

    my_str+=10;
    snprintf(my_str, sizeof(char) * 10, "u: %'u", reg.u);
    Clay_String u_txt = (Clay_String){.isStaticallyAllocated = false, .length = 10, .chars = my_str};

    my_str+=10;
    snprintf(my_str, sizeof(char) * 10, "char: %c", (char)reg.s);
    Clay_String c_txt = (Clay_String){.isStaticallyAllocated = false, .length = 10, .chars = my_str};

    const int h_size = 12;
    const int p_size = 8;
    CLAY_AUTO_ID(registerBoxConfig(col_index[id])) {
        CLAY_TEXT(reg_txt, { .fontSize = h_size, .textColor = col_index[id] });
        CLAY_TEXT(s_txt, { .fontSize = p_size, .textColor = {255, 255, 255, 255} });
        CLAY_TEXT(u_txt, { .fontSize = p_size, .textColor = {255, 255, 255, 255} });
        CLAY_TEXT(c_txt, { .fontSize = p_size, .textColor = {255, 255, 255, 255} });
    }
}

int RegisterWindow(char* string_arena, teenyat* t) {
    int i = 0;
    uint8_t reg_index[8] = {TNY_REG_A, TNY_REG_B, TNY_REG_C, TNY_REG_D, TNY_REG_E, TNY_REG_PC, TNY_REG_SP, TNY_REG_ZERO};
    CLAY(CLAY_ID("RegisterWindow"), {
            .layout = { .layoutDirection = CLAY_LEFT_TO_RIGHT,
                        .sizing = { .width = CLAY_SIZING_GROW(0),
                                    .height = CLAY_SIZING_FIXED(150) },
                        .padding = {16, 32, 32, 32},
                        .childGap = 20 },

            .clip = {   .vertical = true,
                        .horizontal = true,
                        .childOffset = Clay_GetScrollOffset() },

            .backgroundColor = BOX_COLOR }) {

            for(i = 0; i < 8; i++) {
                RegisterBoxComponent(string_arena + (i * 38), i, t->reg[reg_index[i]]);
            }
    }

    return (i * 38);
}
