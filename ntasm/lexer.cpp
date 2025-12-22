/**
 * NOTES:
 *   So technically I could handle certain errors here like in improper preprocessor directive
 *   but what the readings are telling me is that the Lexer should only be responisble for the
 *   actual collection of tokens and its the corresponding syntax analysis or parser step that should
 *   report these errors.
 *
 *   Basically what this lexer does is similar to tnasm except in that it discared matched input after its
 *   resolved it TokenType all this means is that you be extra certain that what you are looking at is indeed
 *   that token because its achored at the start of the stream so all of my expressions can start with ^
 */
#include "lexer.h"
#include <iostream>

/* Lexer initializer to create a new instance of our lexer with context to its source filename */
Lexer::Lexer(const std::string& source, const std::string& filename)
    : src(source),
      source_file(filename),
      pos(0),
      line(1) {
    init_rules();
}

void Lexer::init_rules() {
    rules.clear();

    // Whitespaces
    rules.push_back({ std::regex("^[ \\t\\b\\v\\f\\r]+"), T_IGNORE });

    // Comments
    rules.push_back({ std::regex("^;.*"), T_IGNORE });

    // End of line
    rules.push_back({ std::regex("^\\r?\\n"), T_EOL });

    // Preprocessor directives
    rules.push_back({ std::regex("^@[A-Za-z_][A-Za-z0-9_]*"), T_AT_DIRECTIVE });

    // Labels
    rules.push_back({ std::regex("^![A-Za-z_][A-Za-z0-9_]*"), T_LABEL });
    
    // Constants
    rules.push_back({ std::regex("^\\.const(ant)?", std::regex_constants::icase), T_CONSTANT });

    // Variables
    rules.push_back({ std::regex("^\\.var(iable)?", std::regex_constants::icase), T_VARIABLE });

    // Named registers (pc, sp, rz)
    rules.push_back({ std::regex("^pc|sp|rz", std::regex_constants::icase), T_REGISTER });

    // Generic Registers (rA-E & r0-7)
    rules.push_back({ std::regex("^[rR][a-eA-E0-7]"), T_REGISTER });

    // Opcodes (our instructions)
    rules.push_back({ std::regex("^set\\b", std::regex_constants::icase), T_SET });
    rules.push_back({ std::regex("^add\\b", std::regex_constants::icase), T_ADD });

    // Hexadecimal number
    rules.push_back({ std::regex("^0x(_*[0-9a-f]+)+", std::regex_constants::icase), T_NUMBER });

    // Binary Number
    rules.push_back({ std::regex("^0b(_*[0-1]+)+"), T_NUMBER });

    // Octal Number
    rules.push_back({ std::regex("^0o(_*[0-7]+)+"), T_NUMBER });

    // Decimal Numbers
    rules.push_back({ std::regex("^[0-9]+(_*[0-9]+)*"), T_NUMBER });

    // Number Prefixes
    rules.push_back({ std::regex("^\\+"), T_PLUS });
    rules.push_back({ std::regex("^-"), T_MINUS });

    // Strings
    rules.push_back({ std::regex("^\"([^\"\\\\]|\\\\.)*\""), T_STRING });

    // Characters
    rules.push_back({ std::regex("^'([^'\\\\]|\\\\.)'"), T_CHARACTER });

    // Packed strings
    rules.push_back({ std::regex("^'[^'\\n\\r]{2,}'"), T_PACKED_STRING });

    // Punctuation
    rules.push_back({ std::regex("^,"), T_COMMA });
    rules.push_back({ std::regex("^\\["), T_LBRACKET });
    rules.push_back({ std::regex("^\\]"), T_RBRACKET });

    // Identifier (this should also be last so that we dont end up matching it first over something else)
    rules.push_back({ std::regex("^[A-Za-z_][A-Za-z0-9_]*"), T_IDENTIFIER });
}

bool Lexer::match_rule(const regex_rule& rule, std::cmatch& match_out) {
    std::cmatch match;
    const char* start = src.c_str() + pos;

    if (std::regex_search(start, match_out, rule.pattern, std::regex_constants::match_continuous)) {
        return true;
    }
    return false;
}

token Lexer::next_token() {
    if (pos >= src.size()) {
        return token(T_EOL, "", line);
    }

    const regex_rule* best_rule = nullptr;
    std::cmatch best_match;
    size_t best_len = 0;

    for (const auto& rule : rules) {
        std::cmatch m;
        if (match_rule(rule, m)) {
            size_t len = m.length();
            if(len > best_len) {
                best_len = len;
                best_match = m;
                best_rule = &rule;                
            }
        }
    }

    /**
     * No rule matched means BAD token and then we can report this as
     * a potential error later during parsing...
     *
     * Also probably should have put the source_file into the constructor
     */
    if (!best_rule) {
        char bad = src[pos++];
        token bad_tok = token(T_BAD, std::string(1, bad), line);
        bad_tok.source_file = source_file;
        return bad_tok;
    }

    std::string lexeme = best_match.str();
    pos += best_len;

    token tok = token(best_rule->type, lexeme, line);
    tok.source_file = source_file;

    if (tok.type == T_EOL) {
        line++;
    }

    if (tok.type == T_IGNORE) {
        return next_token();
    }

    return tok;

}

token Lexer::peek_token() {
    size_t saved_pos = pos;
    int saved_line = line;

    token t = next_token();

    pos = saved_pos;
    line = saved_line;

    return t;
}

