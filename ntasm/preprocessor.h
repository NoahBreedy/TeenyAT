#pragma once

#include <stack>
#include <unordered_map>
#include <memory>
#include <string>

#include "lexer.h"
#include "token.h"

class Preprocessor {
public:
    explicit Preprocessor(Lexer* root);

    token next_token();

private:

    /* Stack of lexers (for @include) */
    std::stack<Lexer*> lexers;

    /* Dictionary to hold our macros */
    std::unordered_map<std::string, std::string> macros;

    /**
     * Conditional compilation stack
     * this helps prevent tokens from being parsed if they
     * some condition isint satisfied
     * */
    struct cond_state {
        bool enabled;
    };

    std::stack<cond_state> cond_stack;

    Lexer& current_lexer();
    bool is_active();

    void handle_directive(const token& directive);

    void handle_define();
    void handle_undef();
    void handle_ifdef(bool invert);
    void handle_endif(const token& directive);
    void handle_include();

    void skip_line();
};

