#pragma once

#include <string>
#include <vector>
#include <regex>
#include "token.h"

class Lexer {
public:
    Lexer(const std::string& source,
          const std::string& filename = "<input>");

    token next_token();

    token peek_token();

private:
    struct regex_rule {
        std::regex pattern;
        token_type type;
    };

    std::string src;
    std::string source_file;

    size_t pos;
    int line;
    int addr;

    std::vector<regex_rule> rules;

    void init_rules();
    bool match_rule(const regex_rule& rule, token& out);
};

