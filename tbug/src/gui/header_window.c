#include "gui/header_window.h"

static uint8_t action_ids[3] = {0, 1, 2};

bool stop_mode  = true;
bool step_mode  = false;
bool reset_mode = false;

void HandleActionBoxInteraction(Clay_ElementId elementId, Clay_PointerData pointerInfo, void *userData) {
    uint8_t actionID = *((uint8_t*)userData);

    // Pointer state allows you to detect mouse down / hold / release
    if (pointerInfo.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {

        if(actionID == 0) { // RUN or STOP
            stop_mode = !stop_mode;
        }else if(actionID == 1) { // STEP
            step_mode = true;
        }else { // RESET
            reset_mode = true;
        }

    }
}

Clay_ElementDeclaration actionBoxConfig = (Clay_ElementDeclaration) {
    .layout = { .layoutDirection = CLAY_LEFT_TO_RIGHT,
                .sizing = { .width = CLAY_SIZING_FIXED(180),
                            .height = CLAY_SIZING_FIXED(80) },
                .padding  = {8, 0, 0, 0},
                .childAlignment = { .y = CLAY_ALIGN_Y_CENTER },
                .childGap = 16 },
    .backgroundColor = ACTION_BOX_COLOR
};

void ActionBoxComponent(const char* my_str, char* img_url, uint8_t* component) {
    Clay_String text = (Clay_String){.isStaticallyAllocated = true, .length = strlen(my_str), .chars = my_str};
    CLAY_AUTO_ID(actionBoxConfig) {
        Clay_OnHover(HandleActionBoxInteraction, component);

        CLAY_AUTO_ID({
                    .layout = {
                        .sizing = {
                            .width = CLAY_SIZING_FIXED(80),
                            .height = CLAY_SIZING_FIXED(80) }
                        },
                    .image = { .imageData = img_url } }) {

        }
        CLAY_TEXT(text, { .fontSize = 16, .textColor = {255, 255, 255, 255} });
    }
}

Clay_ElementDeclaration infoBoxConfig = (Clay_ElementDeclaration) {
    .layout = { .layoutDirection = CLAY_TOP_TO_BOTTOM,
                .sizing = { .width = CLAY_SIZING_FIXED(100),
                            .height = CLAY_SIZING_FIXED(80) },
                .padding  = {8, 8, 8, 8},
                .childAlignment = { .x = CLAY_ALIGN_X_CENTER },
                .childGap = 16 },
    .backgroundColor = ACTION_BOX_COLOR
};

void InfoBoxComponent(const char* my_str, char* buff, uint64_t cycle_cnt) {
    Clay_String text = (Clay_String){.isStaticallyAllocated = true, .length = strlen(my_str), .chars = my_str};
    snprintf(buff, 32, "%'ld", cycle_cnt);
    Clay_String value = (Clay_String){.isStaticallyAllocated = false, .length = strlen(buff), .chars = buff};
    CLAY_AUTO_ID(infoBoxConfig) {
        CLAY_TEXT(text, { .fontSize = 8, .textColor = {255, 255, 255, 255} });
        CLAY_TEXT(value, { .fontSize = 8, .textColor = {255, 255, 255, 255} });
    }
}

int HeaderWindow(char* string_arena, teenyat* t) {
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

            if(stop_mode)
                ActionBoxComponent("Run", "resources/Run_Button.png", action_ids);
            else
                ActionBoxComponent("Stop", "resources/Stop_Button.png", action_ids);

            ActionBoxComponent("Step", "resources/Step_Button.png", action_ids + 1);

            ActionBoxComponent("Reset", "resources/Reset_Button.png", action_ids + 2);
            
            InfoBoxComponent("Cycle Cnt:", string_arena, t->cycle_cnt);

    }

    /* 32 chars */
    return 32;
}
