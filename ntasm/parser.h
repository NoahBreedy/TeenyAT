#pragma once

#include <string>
#include <vector>

#include "preprocessor.h"
#include "token.h"
#include "../teenyat.h"

#define MAX_IDENTIFIER_INSTANCES 1

class Parser {
public:
    explicit Parser(Preprocessor& pp, bool debug);

    bool parse_program();

    std::vector<tny_word> bin_words;

    std::string binary_listing;
    std::string error_log;
    std::string trace_log;
    std::string warning_log;

    bool debug_mode;

private:
    Preprocessor& pp;
    token current;
    token previous;

    bool valid_program;

    tny_word bin_word_0;
    tny_word bin_word_1;

    tny_word address;
    tny_uword label_resolutions;
    tny_uword max_lines;
    tny_uword string_length;
    tny_uword raw_line_index;
    bool is_raw_line;

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
    bool     jump_inst;

    void trace_parser(bool print_new_line);
    void collect_raw_listing(int index, tny_word value);

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
    tny_word process_string(std::string s);
    tny_word process_packed_string(std::string s);

    void parse_line();
    bool parse_statement();
    bool parse_label_line();
    bool parse_constant_line();
    bool parse_code_line();
    bool parse_variable_format();
    bool parse_variable_line();
    bool parse_raw_line();
    bool parse_raw_line_value();

    bool parse_raw_value(tny_word* immed);
    bool parse_immediate(tny_word* immed);
    bool parse_no_sign_immediate(tny_word* immed);
    bool parse_includes_immediate(tny_word* immed);
    bool parse_includes_register(tny_word* reg);
    bool parse_register_and_immediate(tny_word* reg, tny_word* immed);

    bool parse_set_instruction();
    bool parse_lod_instruction();
    bool parse_str_instruction();
    bool parse_psh_instruction();
    bool parse_psh_format();
    bool parse_pop_instruction();
    bool parse_pop_format();
    bool parse_bts_instruction();
    bool parse_btc_instruction();
    bool parse_btf_instruction();
    bool parse_cal_instruction();
    bool parse_add_instruction();
    bool parse_sub_instruction();
    bool parse_mpy_instruction();
    bool parse_div_instruction();
    bool parse_mod_instruction();
    bool parse_and_instruction();
    bool parse_or_instruction();
    bool parse_xor_instruction();
    bool parse_shf_instruction();
    bool parse_shl_instruction();
    bool parse_shr_instruction();
    bool parse_rot_instruction();
    bool parse_rol_instruction();
    bool parse_ror_instruction();
    bool parse_neg_instruction();
    bool parse_cmp_instruction();
    bool parse_dly_instruction();
    bool parse_dly_format();
    bool parse_dly_register_format();
    bool parse_int_instruction();
    bool parse_rti_instruction();
    bool parse_jmp_instruction();
    bool parse_je_instruction();
    bool parse_jne_instruction();
    bool parse_jl_instruction();
    bool parse_jle_instruction();
    bool parse_jg_instruction();
    bool parse_jge_instruction();
    bool parse_lup_instruction();
    bool parse_inc_instruction();
    bool parse_dec_instruction();
    bool parse_inv_instruction();
    bool parse_ret_instruction();

};

