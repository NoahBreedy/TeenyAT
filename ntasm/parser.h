#pragma once

#include <string>
#include "preprocessor.h"
#include "token.h"

class Parser {
public:
    explicit Parser(Preprocessor& pp);

    void parse_program();

private:
    Preprocessor& pp;
    token current;

    void advance();
    bool match(token_type t);
    void expect(token_type t, const std::string& msg);

    void parse_line();
    void parse_statement();
    bool parse_label_line();
    bool parse_code_line();

    bool parse_number();
    bool parse_raw_value();
    bool parse_immediate();
    bool parse_includes_immediate();
    bool parse_includes_register();
    bool parse_register_and_immediate();

    bool parse_set_instruction();
};

