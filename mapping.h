#ifndef __TNY_MAPPINGS_H__
#define __TNY_MAPPINGS_H__

const char *tny_opcodes_strings[] = {
    "set",
    "lod",
    "str",
    "psh",
    "pop",
    "bts",
    "btc",
    "btf",
    "cal",
    "add",
    "sub",
    "mpy",
    "div",
    "mod",
    "and",
    "or",
    "xor",
    "shf",
    "rot",
    "neg",
    "cmp",
    "jmp",
    "lup",
    "dly"
};

const char *tny_registers_strings[] = {
    "Z",
    "PC",
    "SP",
    "A",
    "B",
    "C",
    "D",
    "E"
};

const char *tny_opcodes_jump_strings[] = {
    "jmp",
    "jg",
    "jl",
    "jne",
    "je",
    "jge",
    "le",
    "jmp"
};

#endif /* __TNY_MAPPINGS_H__ */