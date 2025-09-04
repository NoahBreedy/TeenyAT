#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "teenyat.h"
const char *tny_opnames[] = {
    "set", // 0
    "lod", // 1
    "str", // 2
    "psh", // 3
    "pop", // 4
    "bts", // 5
    "btc", // 6
    "btf", // 7
    "cal", // 8
    "add", // 9
    "sub", // 10
    "mpy", // 11
    "div", // 12
    "mod", // 13
    "and", // 14
    "or",  // 15
    "xor", // 16
    "shf", // 17
    "rot", // 18
    "neg", // 19
    "cmp", // 20
    "jmp", // 21
    "lup", // 22
    "dly"  // 23
};

const char *tny_regnames[] = {
    "rZ",  // 0
    "PC",  // 1
    "SP",  // 2
    "rA",  // 3
    "rB",  // 4
    "rC",  // 5
    "rD",  // 6
    "rE"   // 7
};

const char *tny_jumpnames[] = {
    "jmp",
    "jg",
    "jl",
    "jne",
    "je",
    "jge",
    "jle",
    "jmp",
    "jmp",
    "jmp",
    "jmp",
    "jmp",
    "jmp",
    "jmp",
    "jmp",
    "jmp"
};

void decode_instruction() {
  int pc =  tny_debugger_ctx->reg[TNY_REG_PC].u;
  tny_word IR = tny_debugger_ctx->ram[pc];
  const char* op = tny_opnames[IR.instruction.opcode];
  const char* reg1 = tny_regnames[IR.instruction.reg1];
  const char* reg2 = tny_regnames[IR.instruction.reg2];
  int num_op = IR.instruction.opcode;
  int teeny = IR.instruction.teeny;
  int immed = IR.instruction.immed4;

  if(!teeny) {
    immed = tny_debugger_ctx->ram[pc+1].u;
  }

  if(num_op == TNY_OPCODE_LOD) {
   printf("%s %s, [ %d + %s ]\n",op,reg1, immed, reg2);
  }else if(num_op == TNY_OPCODE_STR) {
   printf("%s [ %d + %s ], %s\n",op,immed,reg1,reg2);
  }else if(num_op == TNY_OPCODE_JMP) {
   const char* jmp = tny_jumpnames[IR.instruction.immed4];
   printf("%s %d\n",jmp,immed);
  }else {
   printf("%s %s, %s + %d\n",op,reg1,reg2,immed);
  }
}

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
  printf("Instrunction: ");
  decode_instruction();

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


