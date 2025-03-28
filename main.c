/*
 * Name	   : main.c
 * Author	 : William "Amos" Confer
 *
 * License	: Copyright (C) 2023 All rights reserved
 *
 * This brief program is meant to serve as an ultra simplistic example of
 * how the TeenyAT header and source can be quickly used to create a
 * simulated system.  In this case, the system loads whatever binary image
 * is provided and runs it for up 123456 cycles.  Any attempt to read from
 * the bus will result in a 9-cycle access to the number 42, and writes to
 * the bus will have no side effect other than wasting 7 cycles.
 */

#include <stdio.h>
#include <stdlib.h>

#include "teenyat.h"

void bus_read(teenyat *t, tny_uword addr, tny_word *data, uint16_t *delay);
void bus_write(teenyat *t, tny_uword addr, tny_word data, uint16_t *delay);

int main(int argc, char *argv[]) {
	if(argc < 2) {
		fprintf(stderr, "usage:  %s <bin_file>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	int loop_cnt = (1 << 6);
	if(argc == 3){
		loop_cnt = atoi(argv[2]);
	}

	FILE *bin_file = fopen(argv[1], "rb");
	teenyat t;
	tny_init_from_file(&t, bin_file, bus_read, bus_write);
	//int old_pc = 0; 
	for(int i = 0; i < loop_cnt; i++) {
		tny_clock(&t);
		// if(t.reg[TNY_REG_B].u == 1 ){  // rB is our flag variable
		// 	printf("CYCLES:  %d\n", t.cycle_cnt);
		// 	printf("PC: %d\n",t.reg[TNY_REG_PC].u);
		// 	printf("DELAY: %d\n\n", t.delay_cycles);
		// 	break;
		// }
		//old_pc = t.reg[TNY_REG_PC].u;
	}
	
	return EXIT_SUCCESS;
}

void bus_read(teenyat *t, tny_uword addr, tny_word *data, uint16_t *delay) {
	data->s = +42;  /* That's what the answer has always been */
	*delay = 9;  /* add a delay overhead of 9 cycles */

	return;
}

void bus_write(teenyat *t, tny_uword addr, tny_word data, uint16_t *delay) {
	*delay = 7;  /* add a delay overhead of 7 cycles */

	return;
}
