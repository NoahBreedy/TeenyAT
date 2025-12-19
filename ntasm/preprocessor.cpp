#include <fstream>
#include <iostream>

#include "preprocessor.h"
#include "errorlog.h"

Preprocessor::Preprocessor(Lexer* root) {
    valid_program = true;
    lexers.push(root); // move is pretty cool actually since we know we wont need root to hold the info no mo
}

Lexer& Preprocessor::current_lexer() {
    return *lexers.top();
}

bool Preprocessor::is_active() {
    return cond_stack.empty() || cond_stack.top().enabled;
}


token Preprocessor::next_token() {
    while (!lexers.empty()) {
        token tok = current_lexer().next_token();

        /* EOF of current lexer --> pop and continue */
        if (tok.type == T_EOL && tok.token_str.empty()) {
            lexers.pop();
            continue;
        }

        /* Preprocessor directive */
        if (tok.type == T_AT_DIRECTIVE) {
            handle_directive(tok);
            continue;
        }

        /* Skip tokens if in inactive blocks */
        if (!is_active()) {
            if (tok.type == T_EOL)
                continue;
            continue;
        }

        return tok;
    }

    return token(T_EOL, "", 0);
}

void Preprocessor::handle_directive(const token& directive) {
    std::string name = directive.token_str.substr(1);

    if      (name == "define")  handle_define();
    else if (name == "undef")   handle_undef();
    else if (name == "ifdef")   handle_ifdef(false);
    else if (name == "ifndef")  handle_ifdef(true);
    else if (name == "endif")   handle_endif(directive);
    else if (name == "include") handle_include();
    else {
        valid_program = log_error(directive,"unknown directive @" + name);
        skip_line();
    }
}

void Preprocessor::handle_define() {
    token name = current_lexer().next_token();

    if (name.type != T_IDENTIFIER) {
        valid_program = log_error(name, "Expected identifier after @define");
        skip_line();
        return;
    }

    std::string value;
    token tok = current_lexer().next_token();

    while (tok.type != T_EOL) {
        value += tok.token_str;
        tok = current_lexer().next_token();
    }

    macros[name.token_str] = value;
}

void Preprocessor::handle_undef() {
    token name = current_lexer().next_token();
    macros.erase(name.token_str);
    skip_line();
}

void Preprocessor::handle_ifdef(bool invert) {
    token name = current_lexer().next_token();

    bool defined = macros.contains(name.token_str);
    bool enable = invert ? !defined : defined;

    cond_stack.push({ enable });
    skip_line();
}

void Preprocessor::handle_endif(const token& directive) {
    if (cond_stack.empty()) {
        valid_program = log_error(directive, "Unmatched @endif");
        return;
    }
    cond_stack.pop();
    skip_line();
}

void Preprocessor::handle_include() {
    token path = current_lexer().next_token();

    if (path.type != T_STRING) {
        valid_program = log_error(path, "@include requires string literal");
        skip_line();
        return;
    }

    std::string filename = path.token_str.substr(1, path.token_str.size() - 2);

    std::ifstream file(filename);
    if (!file) {
        valid_program = log_error(path, "Could not open include file \"" + filename + "\"");
        skip_line();
        return;
    }

    /* Open our file and dump all of its words */
    std::string contents(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );

    lexers.push(new Lexer(contents, filename));
}

void Preprocessor::skip_line() {
    token tok = token(T_EOL, "", 0);
    do {
        tok = current_lexer().next_token();
    } while (tok.type != T_EOL);
}

