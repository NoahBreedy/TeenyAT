#pragma once

#include <string>
#include "preprocessor.h"
#include "token.h"
#include "teenyat.h"

class Parser {
public:
    explicit Parser(Preprocessor& pp);

    void parse_program();

private:
    Preprocessor& pp;
    token current;

    tny_word bin_word_0;
    tny_word bin_word_1;
    
    /* Im making these teeny_words because I don't want the overload */
    tny_word p_opcode;
    bool     p_teeny;
    tny_word p_reg1;
    tny_word p_reg2;
    tny_word p_immed;
    tny_word p_negative;

    void advance();
    bool match(token_type t);
    bool match(token_type t, tny_word* dest);
    void expect(token_type t, const std::string& msg);

    void set_destination(token token, tny_word* dest);
    tny_word register_to_value(std::string s);
    tny_word token_to_opcode(token_type t);

    void parse_line();
    void parse_statement();
    bool parse_label_line();
    bool parse_code_line();

    bool parse_number(tny_word* immed);
    bool parse_raw_value(tny_word* immed);
    bool parse_immediate(tny_word* immed);
    bool parse_includes_immediate(tny_word* immed);
    bool parse_includes_register(tny_word* reg);
    bool parse_register_and_immediate(tny_word* reg, tny_word* immed);

    bool parse_set_instruction();
};

