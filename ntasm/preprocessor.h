#pragma once

#include <stack>
#include <unordered_map>
#include <memory>
#include <string>
#include <sstream>
#include <iomanip>

#include "lexer.h"
#include "token.h"

// how many times you can @include the same file till error
#define INCLUDE_THRESHOLD 10

class Preprocessor {
public:
    explicit Preprocessor(Lexer* root);

    token next_token();
    Lexer& current_lexer();
    void reset_lexer();
    bool valid_program;

private:

    /* Stack of lexers (for @include) */
    std::stack<Lexer*> lexers;

    std::string root_src;
    std::string root_filename;

    /* Dictionary to hold our macros */
    std::unordered_map<std::string, token> macros;
    std::unordered_map<std::string, int>   opened_files;

    /**
     * Conditional compilation stack
     * this helps prevent tokens from being parsed if they
     * some condition isint satisfied
     * */
    struct cond_state {
        bool enabled;
    };

    std::stack<cond_state> cond_stack;

    bool is_active();

    void handle_directive(const token& directive);

    void handle_define();
    void handle_undef();
    void handle_ifdef(bool invert);
    void handle_endif(const token& directive);
    void handle_include();

    void skip_line();
};

