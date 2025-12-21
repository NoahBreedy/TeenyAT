#include <iostream>
#include <cstdlib>

#include "parser.h"
#include "errorlog.h"

Parser::Parser(Preprocessor& p, bool debug)
    : pp(p), current(T_EOL, "", 0) {

    address.u = 0;
    label_resolutions = 0;
    debug_mode  = debug;
    valid_program = true;
    p_opcode.u = 0;
    p_reg1.u = 0;
    p_reg2.u = 0;
    p_immed.s = 0;
    p_negative.u = 0;
    p_condition_flags.u = 0;
    error_log = "";
}

void Parser::reset_lexer() {
    pp.reset_lexer();
    /* get our first token since current is initialized to EOF */
    advance();
}

void Parser::skip_line() {
    while(current.type != T_EOL) {
        trace_parser(false);
        advance();
    }
}

void Parser::advance() {
    current = pp.next_token();
}

void Parser::trace_parser(bool print_new_line) {
    if(debug_mode) {
        if(print_new_line) {
            trace_log += '\n';
        }else {
            std::string s = current.token_str;
            if(s == "\n") s = "\\n";
            trace_log += tstr[current.type] + "( " + s + " ) ";
        }
    }
}

/* just see if it fits into 4 bits */
bool is_teeny(tny_sword n) {
    tny_word small;
    small.instruction.immed4 = n;
    return small.instruction.immed4 == n;
}

void Parser::push_binary_instruction() {
    /* map our processed values into its associated fields */

    bin_word_0.instruction.opcode =  p_opcode.u;
    bin_word_0.instruction.teeny  =  0;
    bin_word_0.instruction.reg1   =  p_reg1.u;
    bin_word_0.instruction.reg2   =  p_reg2.u;
    bin_word_0.instruction.immed4 =  p_condition_flags.u;

    bin_word_1.s = p_immed.s;

    bool teeny = is_teeny(p_immed.s);
    /* we check if condition flags is zero since and jmp instruction will have all 1s in it */
    if(teeny && p_condition_flags.u == 0) {
       bin_word_0.instruction.immed4 = p_immed.s;
       bin_word_0.instruction.teeny  = 1;
    }

    bin_words.push_back(bin_word_0);
    address.u++;

    if(!teeny) {
        bin_words.push_back(bin_word_1);
        address.u++;
    }

}

bool Parser::match(token_type t) {
    if (current.type == t) {
        trace_parser(false);
        advance();
        return true;
    }
    return false;
}

bool Parser::match(token_type t, tny_word* dest) {
    if (current.type == t) {
        trace_parser(false);
        set_destination(current, dest);
        advance();
        return true;
    }
    return false;
}

bool Parser::expect(token_type t) {
    if (!match(t)) {
        trace_parser(false);
        valid_program = false;
        /* we advance here since if we don't well end up in a looooop (canceled the early exit)*/
        advance();
        return false;
    }
    return true;
}

tny_word Parser::token_to_opcode(token_type t) {
    switch(t) {
        case T_SET: return tny_word{u: TNY_OPCODE_SET};
        default: std::cerr << "Fatal error unkownn opcode (should never see this)" << std::endl; std::exit(EXIT_FAILURE);
    }
}

void Parser::set_destination(token token, tny_word* dest) {
    switch(token.type) {
        case T_REGISTER:        *dest = register_to_value(token.token_str); break;
        case T_PLUS:            *dest = tny_word{u: 0}; break;
        case T_MINUS:           *dest = tny_word{u: 1}; break;
        case T_NUMBER:          *dest = process_number(token.token_str); break;
        case T_LABEL:           *dest = process_label(token.token_str); break;
        case T_IDENTIFIER:      *dest = tny_word{u: 69}; break;
        case T_CHARACTER:       *dest = process_character(token.token_str); break;
        case T_STRING:          *dest = tny_word{u: 421}; break;
        case T_PACKED_STRING:   *dest = tny_word{u: 422}; break;
        /* this means its an opcode */
        default: *dest = token_to_opcode(token.type); break;
   }
   return;
}

tny_word Parser::process_label(std::string s) {
    tny_word result;
    result.u = 0;

    if(labels.contains(s)) {
        result.u =  labels[s].value.u;
    }else {
        std::string line = token_line_str(pp, current);
        valid_program = log_error(current, ltrim(line) + "\t unknown label \"" + s + "\"");
    }

    if(p_negative.u) {
       result.s *= -1;
    }

    return result;
}

tny_word Parser::process_character(std::string s) {
    tny_word result;
    bool invalid = false;
    result.u = s[1];

    if(s[1] == '\\') {
        switch(s[2]) {
            case 'n':  result.u = '\n'; break;
            case 't':  result.u = '\t'; break;
            case 'v':  result.u = '\v'; break;
            case 'r':  result.u = '\r'; break;
            case 'f':  result.u = '\f'; break;
            case '\\': result.u = '\\'; break;
            case '\'': result.u = '\''; break;
            case '\"': result.u = '\"'; break;
            case '0':  result.u = '\0'; break;
            default:   invalid = true;  break;
        }
    }

    if(invalid) {
        std::string line = token_line_str(pp, current);
        valid_program = log_error(current, ltrim(line) + "\tinvalid escape character");
    }

    if(p_negative.u) {
       result.s *= -1;
    }

    return result;
}

tny_word Parser::process_number(std::string s) {
    const char table[] = "0123456789abcdef";
    tny_word result;
    result.u = 0;

    int base = 10;

    if(s.size() > 2) {
        switch(s[1]) {
                case 'b': base = 2;  break;
                case 'o': base = 8; break;
                case 'x': base = 16; break;
                default:  base = 10; break;
        }
    }

    if(base != 10) s = s.substr(2);

    for(auto c : s) {
        if(c != '_') {
            result.s *= base;
            result.s += (std::strchr(table, std::tolower(c)) - table) / sizeof(char);
        }
    }

    /* negate the value if its supposed to be negative */
    if(p_negative.u) {
        result.s *= -1;
    }

    return result;
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
    }else if(std::isdigit(s[1])) {
        register_value.u =  (int)(s[1] - '0');
    }else {
        register_value.u = (int)(s[1] - 'a') + TNY_REG_A;
    }

    return register_value;
}

void Parser::setup_program() {
    valid_program = true;
    running_error_log = "";
    address.u = 0;
    trace_log         = "";
    /* clear all binary words */
    bin_words.clear();
    label_resolutions++;

    /* reset instance count for labels */
    for (const auto& elm : labels) {
        labels[elm.first].instances = 0;
    }

    reset_lexer();
}

bool Parser::parse_program() {
    setup_program();

    while (current.type != T_EOL || !current.token_str.empty()) {
        parse_line();
        trace_parser(true);
    }

    error_log =  running_error_log; // let our error_log match the running one

    return (valid_program && pp.valid_program);
}

void Parser::parse_line() {
    if (match(T_EOL)) {
        return;
    }

    bool matched = parse_statement();

    if(!matched) {
        std::string line = token_line_str(pp,current);
        valid_program = log_error(current, ltrim(line) + "\tinvalid syntax!");
    }

    bool valid_statement = expect(T_EOL);

    if(!valid_statement && matched) {
        std::string line = token_line_str(pp,current);
        valid_program = log_error(current, ltrim(line) + "\tinvalid syntax!");
    }
}

bool Parser::parse_statement() {
    bool matched_statement = parse_label_line() ||
                             parse_code_line();

    return matched_statement;
}

bool Parser::parse_label_line() {
    token label = current;

    if(!match(T_LABEL)) {
        return false;
    }

    std::string label_name = label.token_str;
    if(!labels.contains(label_name)) {
        container obj = {value: address, instances: 1, line_num: (tny_uword)label.line_num};
        labels.insert({ label_name, obj });
    }else {
        labels[label_name].instances++;
        tny_uword line_num = labels[label_name].line_num;
        if(labels[label_name].instances >= 2) {
            std::string line = token_line_str(pp, label);
            valid_program    =  log_error(label, ltrim(line) +
                                "\tduplicate label definition (defined on line " + std::to_string(line_num) + ")");
        }
    }

   return true;
}

bool Parser::parse_code_line() {
    bool matched_code_line = parse_set_instruction();
    return matched_code_line;
}

bool Parser::parse_raw_value(tny_word* immed) {
    if(match(T_CHARACTER, immed)) {
        return true;
    }

    if(match(T_IDENTIFIER, immed)) {
        return true;
    }

    if(match(T_NUMBER, immed)) {
        return true;
    }

    if(match(T_LABEL, immed)) {
        return true;
    }

    return false;
}

bool Parser::parse_immediate(tny_word* immed) {
   if(match(T_PLUS, &p_negative) || match(T_MINUS, &p_negative)) {
        return parse_raw_value(immed);
   }

   /* this means its just the value given no negation needed */
   p_negative.u = 0;
   return parse_raw_value(immed);
}

bool Parser::parse_no_sign_immediate(tny_word* immed) {
    return parse_raw_value(immed);
}

bool Parser::parse_includes_immediate(tny_word* immed) {
    if(match(T_PLUS, &p_negative) || match(T_MINUS, &p_negative)) {
        return parse_no_sign_immediate(immed);
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
        bool valid_inst = parse_register_and_immediate(&p_reg2, &p_immed);
        if(valid_inst) {
            push_binary_instruction();
            std::cout << p_opcode.u << " " << p_reg1.u << " " << p_reg2.u << " " << p_immed.s << std::endl;
        }else {
            skip_line();
        }
        return valid_inst;
    }
    return false;
}
