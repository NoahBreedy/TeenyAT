#ifndef __INPUT_BOXES__
#define __INPUT_BOXES__

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* The input box library is going
 * to be a global manager of textinput boxes
 * where you can declare an input box using the
 * CREATE_INPUT_BOX() function and it will take in
 * something like the max amount of text the box should
 * be able to take in.
 */
typedef struct InputBox {
    char*       value;
    uint32_t    size;
    uint32_t    cursor;
    uint16_t    id;
    bool        focused;
} InputBox;

/* Stretchy buffer to hold input boxes */
extern InputBox** input_boxes;
extern uint16_t  total_input_boxes;
extern int16_t  current_input;

/* Created input_box and adds it to list */
void CREATE_INPUT_BOX(int text_length);

/* Frees all created input_boxes */
void FREE_INPUTS();

/* Sets focus of all input_boxes to false
 *
 * this function also sets current_input to -1
 * */
void TURN_OFF_FOCUS();

/* Sets the current_input to the value passed in
 * current_input of -1 means no input is focused
 *
 * this function also rejects values greater than total_input_boxes-1
 * */
void SET_CURRENT_FOCUSED_ID(uint16_t id);

#endif /* __INPUT_BOXES__ */
