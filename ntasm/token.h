#pragma once
#include <string>

enum token_type {
    T_IGNORE,
    T_EOL,
    T_LABEL,
    T_CONSTANT,
    T_VARIABLE,
    T_RAW,
    T_REGISTER,
    T_SET,
    T_LOD,
    T_STR,
    T_PSH,
    T_POP,
    T_BTS,
    T_BTC,
    T_BTF,
    T_CAL,
    T_ADD,
    T_SUB,
    T_MPY,
    T_DIV,
    T_MOD,
    T_AND,
    T_OR,
    T_XOR,
    T_SHF,
    T_SHL,
    T_SHR,
    T_ROT,
    T_ROL,
    T_ROR,
    T_NEG,
    T_CMP,
    T_DLY,
    T_INT,
    T_RTI,
    T_JMP,
    T_JE,
    T_JNE,
    T_JL,
    T_JLE,
    T_JG,
    T_JGE,
    T_LUP,
    T_INC,
    T_DEC,
    T_INV,
    T_RET,
    T_IDENTIFIER,
    T_AT_DIRECTIVE,
    T_NUMBER,
    T_PLUS,
    T_MINUS,
    T_CHARACTER,
    T_STRING,
    T_PACKED_STRING,
    T_COMMA,
    T_LBRACKET,
    T_RBRACKET,
    T_BAD,
    T_OTHER
};

inline std::string tstr[] = {
    "T_IGNORE",
    "T_EOL",
    "T_LABEL",
    "T_CONSTANT",
    "T_VARIABLE",
    "T_RAW",
    "T_REGISTER",
    "T_SET",
    "T_LOD",
    "T_STR",
    "T_PSH",
    "T_POP",
    "T_BTS",
    "T_BTC",
    "T_BTF",
    "T_CAL",
    "T_ADD",
    "T_SUB",
    "T_MPY",
    "T_DIV",
    "T_MOD",
    "T_AND",
    "T_OR",
    "T_XOR",
    "T_SHF",
    "T_SHL",
    "T_SHR",
    "T_ROT",
    "T_ROL",
    "T_ROR",
    "T_NEG",
    "T_CMP",
    "T_DLY",
    "T_INT",
    "T_RTI",
    "T_JMP",
    "T_JE",
    "T_JNE",
    "T_JL",
    "T_JLE",
    "T_JG",
    "T_JGE",
    "T_LUP",
    "T_INC",
    "T_DEC",
    "T_INV",
    "T_RET",
    "T_IDENTIFIER",
    "T_AT_DIRECTIVE",
    "T_NUMBER",
    "T_PLUS",
    "T_MINUS",
    "T_CHARACTER",
    "T_STRING",
    "T_PACKED_STRING",
    "T_COMMA",
    "T_LBRACKET",
    "T_RBRACKET",
    "T_BAD",
    "T_OTHER"
};

struct token {
    token_type type;
    std::string token_str;
    std::string source_file;

    int line_num;
    int ram_addr;

    /**
     * token struct initializer for when we create new instances
     *
     * Rememebr that std::move() is dumb because in c++ stuffs are passeed by value not reference meaning that strings
     * are deep copied implicitly
     */
    token(token_type t, std::string l, int ln)
        : type(t), token_str(std::move(l)), line_num(ln) {}
};

