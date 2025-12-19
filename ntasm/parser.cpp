#include <iostream>
#include <cstdlib>

#include "parser.h"
#include "errorlog.h"

Parser::Parser(Preprocessor& p)
    : pp(p), current(T_EOL, "", 0){
    /* we advance here because we don't want our first token to be an T_EOL */
    p_opcode.u = 0;
    p_reg1.u = 0;
    p_reg2.u = 0;
    p_immed.s = 0;
    p_negative.u = 0;
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

bool Parser::match(token_type t, tny_word* dest) {
    if (current.type == t) {
        set_destination(current, dest);
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

tny_word Parser::token_to_opcode(token_type t) {
    switch(t) {
        case T_SET: return tny_word{u: TNY_OPCODE_SET};
        default: std::cerr << "unknon opcode" << std::endl; std::exit(EXIT_FAILURE);
    }
}

void Parser::set_destination(token token, tny_word* dest) {
    char* c; 
    switch(token.type) {
        case T_REGISTER:    *dest = register_to_value(token.token_str); break;
        case T_PLUS:        *dest = tny_word{u: 0}; break;
        case T_MINUS:       *dest = tny_word{u: 1}; break;
        case T_NUMBER:      *dest = tny_word{s: (short int) strtol(token.token_str.c_str(), &c, 10)}; break;
        case T_LABEL:       *dest = tny_word{u: 1}; break;
        case T_IDENTIFIER:  *dest = tny_word{u: 1}; break;
        /* this means its an opcode */
        default: *dest = token_to_opcode(token.type); break;
   }
   return;
}

tny_word Parser::register_to_value(std::string s) {
    for (char& c : s) {
        c = std::tolower(c);     
    }
    
    tny_word register_value;
    if (s == "pc") {
        register_value.u = TNY_REG_PC;
    } else if (s == "sp") {
        register_value.u = TNY_REG_SP;
    } else if (s == "rz") {
        register_value.u = TNY_REG_ZERO;
    }else if(isdigit(s[1])) {
        register_value.u =  (int)(s[1] - '0');
    }else {
        register_value.u = (int)(s[1] - 'a') + TNY_REG_A;
    }

    return register_value;
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
    
    std::cout << p_opcode.u << " " << p_reg1.u << " " << p_reg2.u << " " << p_immed.s << std::endl;

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
   /* handle label nonsense like adding it to some dictionary and the current address */
   return true;
}

bool Parser::parse_code_line() {
    bool matched_code_line = parse_set_instruction();    
    return matched_code_line;
}

bool Parser::parse_number(tny_word* immed) {
    if(match(T_IDENTIFIER, immed)) {
        return true;
    }
    if(match(T_NUMBER, immed)) {
        return true;
    }
    if(match(T_PLUS, &p_negative) || match(T_MINUS, &p_negative)) {
        return match(T_NUMBER, immed);
    }
    return false;
}

bool Parser::parse_raw_value(tny_word* immed) {
   if(match(T_CHARACTER, immed)) {
       /* return the number of the char and be wary of p_negative and escape chars */
       return true;
   }
   return parse_number(immed);
}

bool Parser::parse_immediate(tny_word* immed) {
   if(match(T_LABEL, immed)) {
     /* return the address of the label and be wary of p_negative */
     return true;
   }
   return parse_raw_value(immed);
}

bool Parser::parse_includes_immediate(tny_word* immed) {
    if(match(T_PLUS, &p_negative) || match(T_MINUS, &p_negative)) { 
       return parse_immediate(immed);
    }
     
    return immed->u = 0, current.type == T_EOL;
}

bool Parser::parse_includes_register(tny_word* reg) {
    if(match(T_PLUS, &p_negative) && match(T_REGISTER, reg)) {
        return true;
    }
    return reg->u = TNY_REG_ZERO, current.type == T_EOL;
}

bool Parser::parse_register_and_immediate(tny_word* reg, tny_word* immed) {

   if(match(T_REGISTER, reg)) {
        return parse_includes_immediate(immed);     
   }

   if(parse_immediate(immed)) {
        return parse_includes_register(reg);
   }
 
   return false;
}

bool Parser::parse_set_instruction() {
    if(match(T_SET, &p_opcode) && match(T_REGISTER, &p_reg1) && match(T_COMMA)) {
        return parse_register_and_immediate(&p_reg2, &p_immed);
    }
    return false;
}
