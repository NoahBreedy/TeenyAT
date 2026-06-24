#include "input.h"

InputBox** input_boxes = NULL;
uint16_t  total_input_boxes = 0;
int16_t  current_input = -1;

void CREATE_INPUT_BOX(int text_length) {
    InputBox* new_input_box  = malloc(sizeof(InputBox));

    if(!new_input_box) {
        return;
    }

    /* text_length + 1 for the null operator */
    new_input_box->value = calloc(text_length + 1, sizeof(char));
    if(!new_input_box->value) {
        free(new_input_box);
        return;
    }

    total_input_boxes++;
    input_boxes = realloc(input_boxes, sizeof(InputBox*) * total_input_boxes);

    if(!input_boxes) {
        free(new_input_box->value);
        free(new_input_box);
        return;
    }

    new_input_box->size = text_length;
    new_input_box->focused = false;
    new_input_box->id = total_input_boxes - 1;
    new_input_box->cursor = 0;

    input_boxes[total_input_boxes - 1] = new_input_box;

    return;
}

void FREE_INPUTS() {
    if(input_boxes) {
        for(int i = 0; i < total_input_boxes; i++) {
            free(input_boxes[i]->value);
            free(input_boxes[i]);
        }
        free(input_boxes);
        input_boxes = NULL;
        total_input_boxes = 0;
    }
    return;
}

void TURN_OFF_FOCUS() {
    for(int i = 0; i < total_input_boxes; i++) {
        input_boxes[i]->focused = false;
    }
    current_input = -1;
}

void SET_CURRENT_FOCUSED_ID(uint16_t id) {
    if(id >= 0 && id < total_input_boxes) {
        current_input = id;
    }
    return;
}
