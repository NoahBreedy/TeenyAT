#pragma once

#include <string>
#include <vector>

#include "preprocessor.h"
#include "token.h"
#include "teenyat.h"

class Parser {
public:
    explicit Parser(Preprocessor& pp, bool debug);

    bool parse_program();

    std::vector<tny_word> bin_words;
    std::string error_log;
    std::string trace_log;

    bool debug_mode;

private:
    Preprocessor& pp;
    token current;

    bool valid_program;

    tny_word bin_word_0;
    tny_word bin_word_1;

    tny_word address;
    tny_uword label_resolutions;

    typedef struct container {
        tny_word  value;
        tny_uword instances;
        tny_uword line_num;
        std::string file_name;
    } container;

    std::unordered_map<std::string, container> labels;
    std::unordered_map<std::string, container> consts_and_vars;

    /* Im making these teeny_words because I don't want the overload */
    tny_word p_opcode;
    bool     p_teeny;
    tny_word p_reg1;
    tny_word p_reg2;
    tny_word p_immed;
    tny_word p_condition_flags;
    tny_word p_negative;

    void trace_parser(bool print_new_line);

    void push_binary_instruction();
    void push_binary_value(tny_word value);
    void setup_program();

    void reset_lexer();
    void advance();
    void skip_line();
    bool match(token_type t);
    bool match(token_type t, tny_word* dest);
    bool expect(token_type t);

    void set_destination(token token, tny_word* dest);
    tny_word register_to_value(std::string s);
    tny_word token_to_opcode(token_type t);
    tny_word process_number(std::string s);
    tny_word process_character(std::string s);
    tny_word process_label(std::string s);
    tny_word process_identifier(std::string s);

    void parse_line();
    bool parse_statement();
    bool parse_label_line();
    bool parse_constant_line();
    bool parse_code_line();
    bool parse_variable_format();
    bool parse_variable_line();

    bool parse_raw_value(tny_word* immed);
    bool parse_immediate(tny_word* immed);
    bool parse_no_sign_immediate(tny_word* immed);
    bool parse_includes_immediate(tny_word* immed);
    bool parse_includes_register(tny_word* reg);
    bool parse_register_and_immediate(tny_word* reg, tny_word* immed);

    bool parse_set_instruction();
};

