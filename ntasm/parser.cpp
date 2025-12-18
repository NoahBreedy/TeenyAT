#include <iostream>
#include <cstdlib>

#include "parser.h"
#include "errorlog.h"

Parser::Parser(Preprocessor& p)
    : pp(p), current(T_EOL, "", 0){
    /* we advance here because we don't want our first token to be an T_EOL */
    advance();
}

void Parser::advance() {
    current = pp.next_token();
}

bool Parser::match(token_type t) {
    if (current.type == t) {
        advance();
        return true;
    }
    return false;
}

void Parser::expect(token_type t, const std::string& msg) {
    if (!match(t)) {
        log_error(current, msg);
        std::exit(EXIT_FAILURE);
    }
}

void Parser::parse_program() {
    while (current.type != T_EOL || !current.token_str.empty()) {
        parse_line();
    }
}

void Parser::parse_line() {
    if (match(T_EOL)) {
        return;
    }

    parse_statement();

    if (current.type == T_EOL)
        advance();
}

void Parser::parse_statement() {
    bool matched_statement = parse_label_line() ||
                             parse_code_line();
    if(!matched_statement) {
        log_error(current, "invalid syntax!");
        std::exit(EXIT_FAILURE);
    }

    expect(T_EOL, "missing end of line character!"); 
}

bool Parser::parse_label_line() {
   if(!match(T_LABEL)) {
        return false; 
   }
   /* handle label nonsense like adding it to some dictionary */
   return true;
}

bool Parser::parse_code_line() {
    bool matched_code_line = parse_set_instruction();    
    return matched_code_line;
}

bool Parser::parse_number() {
    if(match(T_IDENTIFIER)) {
        return true;
    }
    if(match(T_NUMBER)) {
        return true;
    }
    if(match(T_PLUS) || match(T_MINUS)) {
        return match(T_NUMBER);
    }
    return false;
}

bool Parser::parse_raw_value() {
   if(match(T_CHARACTER)) {
       return true;
   }
   return parse_number();
}

bool Parser::parse_immediate() {
   if(match(T_LABEL)) {
     return true;
   }
   return parse_raw_value();
}

bool Parser::parse_includes_immediate() {
    if(match(T_PLUS) || match(T_MINUS)) {
       return parse_immediate();
    }

    return current.type == T_EOL;
}

bool Parser::parse_includes_register() {
    if(match(T_PLUS) && match(T_REGISTER)) {
        return true;
    }
    return current.type == T_EOL;
}

bool Parser::parse_register_and_immediate() {

   if(match(T_REGISTER)) {
        return parse_includes_immediate();     
   }

   if(parse_immediate()) {
        return parse_includes_register();
   }

   return false;
}

bool Parser::parse_set_instruction() {
    if(match(T_SET) && match(T_REGISTER) && match(T_COMMA)) {
        return parse_register_and_immediate();
    }
    return false;
}
