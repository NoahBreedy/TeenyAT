#include "input.h"
#include <stdio.h>

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
    if(id < total_input_boxes) {
        current_input = id;
    }
    return;
}

void string_strip(char* dest, char* src, char* delim, uint32_t size) {
    char* my_ptr = calloc(size, sizeof(char));
    my_ptr = strncpy(my_ptr, src, size);

    my_ptr = strtok(my_ptr, delim);
    while(my_ptr != NULL) {
        strncat(dest, my_ptr, strlen(my_ptr));
        my_ptr = strtok(NULL, delim);
    }

    free(my_ptr);
    return;
}

int32_t GET_INPUT_VALUE(uint16_t id) {
    uint32_t result = 0;
    if(id < total_input_boxes) {
        char* value = input_boxes[id]->value;
        uint32_t size = input_boxes[id]->size;

        char* delimited = calloc(size, sizeof(char));

        string_strip(delimited, value, ",", size);

        result = strtol(delimited, NULL, 0);

        free(delimited);
    }

    return result;
}
