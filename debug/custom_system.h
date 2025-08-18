#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "teenyat.h"

void custom_signal_2(int sig) {
  tny_debugger_ctx->debug_mode = false;
}

void custom_signal_1(int sig) {
  tny_debugger_ctx->debug_mode = true;
  tny_debugger_ctx->signal_clock = true;
  tny_clock(tny_debugger_ctx);
  tny_debugger_ctx->signal_clock = false;
  printf("------------FREAKY_TEENY_BUG-----------------\n");
  printf("Cycle-Count: %ld\n\n", tny_debugger_ctx->cycle_cnt);

  printf("Registers:\n");
  printf("rA: %d\n", tny_debugger_ctx->reg[TNY_REG_A].s);
  printf("rB: %d\n", tny_debugger_ctx->reg[TNY_REG_B].s);
  printf("rC: %d\n", tny_debugger_ctx->reg[TNY_REG_C].s);
  printf("rD: %d\n", tny_debugger_ctx->reg[TNY_REG_D].s);
  printf("rE: %d\n", tny_debugger_ctx->reg[TNY_REG_E].s);
  printf("PC: %d\n", tny_debugger_ctx->reg[TNY_REG_PC].u);
  printf("SP: %d\n", tny_debugger_ctx->reg[TNY_REG_SP].u);
  printf("--------------------------------------\n");
}
