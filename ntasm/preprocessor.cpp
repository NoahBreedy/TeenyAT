#include <fstream>
#include <iostream>

#include "preprocessor.h"
#include "errorlog.h"

Preprocessor::Preprocessor(Lexer* root) {
    valid_program = true;
    root_src = root->src;
    root_filename = root->source_file;
}

Lexer& Preprocessor::current_lexer() {
    return *lexers.top();
}

bool Preprocessor::is_active() {
    return cond_stack.empty() || cond_stack.top().enabled;
}

void Preprocessor::reset_lexer() {
    /* add the main lexer to the stack */
    lexers.push(new Lexer(root_src, root_filename));
    while(!cond_stack.empty()) {
          cond_stack.pop();
    }
    macros.clear();
    opened_files.clear();
    opened_files[root_filename] = 1;
}

token Preprocessor::next_token() {
    while (!lexers.empty()) {
        token tok = current_lexer().next_token();

        /* EOF of current lexer --> pop and continue */
        if (tok.type == T_EOL && tok.token_str.empty()) {
            lexers.pop();
            if(!lexers.empty()) {
                /* close the file opened */
                opened_files.erase(tok.source_file);
                oss << "---------- " << "EOF" << " -----------\n";
            }
            continue;
        }

        /* Preprocessor directive */
        if (tok.type == T_AT_DIRECTIVE) {
            handle_directive(tok);
            continue;
        }

        if(tok.type == T_IDENTIFIER) {
            if(macros.contains(tok.token_str)) {
                tok = macros.at(tok.token_str);
            }
        }

        /* Skip tokens if in inactive blocks */
        if (!is_active()) {
            continue;
        }

        return tok;
    }

    return token(T_EOL, "", 0);
}

void Preprocessor::handle_directive(const token& directive) {
    std::string name = directive.token_str.substr(1);

    for(char& c : name) {
        c = std::tolower(c);
    }

    /* need endif to be enabled even if in inactive zone */
    if(!is_active() && name != "endif") return;

    std::string line = token_line_str(current_lexer().src, directive);
    if      (name == "define")  handle_define();
    else if (name == "undef")   handle_undef();
    else if (name == "ifdef")   handle_ifdef(false);
    else if (name == "ifndef")  handle_ifdef(true);
    else if (name == "endif")   handle_endif(directive);
    else if (name == "include") handle_include();
    else {
        valid_program = log_error(directive, ltrim(line) + "\tunknown directive @" + name);
        skip_line();
    }
}

void Preprocessor::handle_define() {
    token name = current_lexer().next_token();
    std::string line = token_line_str(current_lexer().src,name);
    if (name.type != T_IDENTIFIER) {
        valid_program = log_error(name, ltrim(line) + "\texpected identifier after @define");
        skip_line();
        return;
    }

    token tok = current_lexer().next_token();
    /* technically I should sanatize this through RD but im lazy */

    /* I cannot use the [] syntax on my token cuz there is no default */
    macros.insert_or_assign(name.token_str, tok);

    if(tok.type != T_EOL) {
        tok = current_lexer().peek_token();
        if(tok.type != T_EOL) {
            valid_program = log_error(tok, ltrim(line) + "\t@define can only reference one item!");
            macros.erase(name.token_str);
        }
    }
    skip_line();
}

void Preprocessor::handle_undef() {
    token name = current_lexer().next_token();

    std::string line = token_line_str(current_lexer().src,name);
    if(name.type != T_IDENTIFIER) {
        valid_program = log_error(name, ltrim(line) + "\tinvalid identifier!");
    }else if(macros.contains(name.token_str)) {
        macros.erase(name.token_str);
    }else {
        valid_program = log_error(name, ltrim(line) + "\tundefining unknown identifier");
    }

    name = current_lexer().peek_token();
    if(name.type != T_EOL) {
        valid_program = log_error(name, ltrim(line) + "\t@undefine can only reference one item!");
    }

    skip_line();
}

void Preprocessor::handle_ifdef(bool invert) {
    token name = current_lexer().next_token();

    std::string line = token_line_str(current_lexer().src,name);

    if(name.type != T_IDENTIFIER) {
        valid_program = log_error(name, ltrim(line) + "\tinvalid identifier!");
    }else if(current_lexer().peek_token().type != T_EOL) {
        valid_program = log_error(name, ltrim(line) + "\tdirective only takes one parameter!");
    }else {
        bool defined = macros.contains(name.token_str);
        bool enable = invert ? !defined : defined;

        cond_stack.push({ enable });
    }
    skip_line();
}

void Preprocessor::handle_endif(const token& directive) {
    token t = current_lexer().peek_token();
    std::string line = token_line_str(current_lexer().src,t);
    if(t.type != T_EOL) {
        valid_program = log_error(t, ltrim(line) + "\t@endif dosn't take extra parameters");
        skip_line();
        return;
    }
    if (cond_stack.empty()) {
        valid_program = log_error(directive, ltrim(line) + "\t\tunmatched @endif");
        skip_line();
        return;
    }
    cond_stack.pop();
    skip_line();
}

void Preprocessor::handle_include() {
    token path = current_lexer().next_token();
    oss << "------ @include " << path.token_str << " ------\n";
    std::string line = token_line_str(current_lexer().src,path);
    if (path.type != T_STRING) {
        valid_program = log_error(path, ltrim(line) + "\t@include requires string literal");
        skip_line();
        return;
    }

    std::string filename = path.token_str.substr(1, path.token_str.size() - 2);

    if(opened_files.contains(filename)) {
        if(opened_files[filename]++ > INCLUDE_THRESHOLD) {
            std::cerr << "FATAL ERROR: " << path.source_file << " on line " <<
                std::to_string(path.line_num) << " " << ltrim(line) << "\tmax amount of file includes reached (potential cyclic include?)" << std::endl;
            std::exit(EXIT_FAILURE);
        }
        log_warning(path, "file has been included more than once (potential cyclic include?)");
    }else{
        opened_files[filename] = 0;
    }

    token t = current_lexer().peek_token();
    if(t.type != T_EOL) {
        valid_program = log_error(path, ltrim(line) + "\t@include takes only one parameter");
        skip_line();
        return;
    }

    std::ifstream file(filename);
    if (!file) {
        valid_program = log_error(path, ltrim(line) + "\tcould not open include file \"" + filename + "\"");
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

