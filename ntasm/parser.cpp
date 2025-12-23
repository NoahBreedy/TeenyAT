#include <iostream>
#include <cstdlib>

#include "parser.h"
#include "errorlog.h"

Parser::Parser(Preprocessor& p, bool debug)
    : pp(p), current(T_EOL, "", 0), previous(T_EOL, "", 0) {

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
    jump_inst = false;
    error_log = "";
    warning_log = "";
    binary_listing = "";
    max_lines = 2;
    oss << "";
    raw_line_index = 0;
    is_raw_line = false;
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
    previous = current;
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

void Parser::collect_raw_listing(int index, tny_word value) {
    if(!(index % 2) && index != 0) {
        oss << "]\n";
        oss << std::setw(10 + max_lines) << ": " <<  "[ ";
    }
    oss << word_to_hex(value) << " ";
}

/* just see if it fits into 4 bits */
bool is_teeny(tny_sword n) {
    tny_word small;
    small.instruction.immed4 = n;
    return small.instruction.immed4 == n;
}

void Parser::push_binary_value(tny_word value) {
    bin_words.push_back(value);
    address.u++;
}

void Parser::push_binary_instruction() {
    /* map our processed values into its associated fields */

    bin_word_0.instruction.opcode =  p_opcode.u;
    bin_word_0.instruction.teeny  =  0;
    bin_word_0.instruction.reg1   =  p_reg1.u;
    bin_word_0.instruction.reg2   =  p_reg2.u;
    bin_word_0.instruction.immed4 =  p_condition_flags.u;

    bin_word_1.s = p_immed.s;

    /* jmp instructions cannot be teeny unfortunately */
    bool teeny = is_teeny(p_immed.s) && !jump_inst;

    /* jmp instructions cannot be teeny unfortunately */
    if(teeny) {
       bin_word_0.instruction.immed4 = p_immed.s;
       bin_word_0.instruction.teeny  = 1;
    }

    /* print listing before address increment */
    std::string line = token_line_str(pp, previous);

    oss << "0x" << word_to_hex(address) << "  " << std::setw(max_lines) << current.line_num << ": [ ";
    oss << word_to_hex(bin_word_0) << " ";

    bin_words.push_back(bin_word_0);
    address.u++;

    if(!teeny) {
        bin_words.push_back(bin_word_1);
        address.u++;
    }

    if(!teeny) {
        oss << word_to_hex(bin_word_1) + " ]  ";
    }else{
        oss << "     ]  ";
    }
    oss << line << '\n';

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
    jump_inst = false;
    switch(t) {
        case T_SET: return tny_word{u: TNY_OPCODE_SET};
        case T_LOD: return tny_word{u: TNY_OPCODE_LOD};
        case T_STR: return tny_word{u: TNY_OPCODE_STR};
        case T_PSH: return tny_word{u: TNY_OPCODE_PSH};
        case T_POP: return tny_word{u: TNY_OPCODE_POP};
        case T_BTS: return tny_word{u: TNY_OPCODE_BTS};
        case T_BTC: return tny_word{u: TNY_OPCODE_BTC};
        case T_BTF: return tny_word{u: TNY_OPCODE_BTF};
        case T_CAL: return tny_word{u: TNY_OPCODE_CAL};
        case T_ADD: return tny_word{u: TNY_OPCODE_ADD};
        case T_SUB: return tny_word{u: TNY_OPCODE_SUB};
        case T_MPY: return tny_word{u: TNY_OPCODE_MPY};
        case T_DIV: return tny_word{u: TNY_OPCODE_DIV};
        case T_MOD: return tny_word{u: TNY_OPCODE_MOD};
        case T_AND: return tny_word{u: TNY_OPCODE_AND};
        case T_OR:  return tny_word{u: TNY_OPCODE_OR };
        case T_XOR: return tny_word{u: TNY_OPCODE_XOR};
        case T_SHF: return tny_word{u: TNY_OPCODE_SHF};
        case T_SHL: return tny_word{u: TNY_OPCODE_SHF};
        case T_SHR: return tny_word{u: TNY_OPCODE_SHF};
        case T_ROT: return tny_word{u: TNY_OPCODE_ROT};
        case T_ROL: return tny_word{u: TNY_OPCODE_ROT};
        case T_ROR: return tny_word{u: TNY_OPCODE_ROT};
        case T_NEG: return tny_word{u: TNY_OPCODE_MPY};
        case T_CMP: return tny_word{u: TNY_OPCODE_CMP};
        case T_DLY: return tny_word{u: TNY_OPCODE_DLY};
        case T_INT: return tny_word{u: TNY_OPCODE_INT};
        case T_RTI: return tny_word{u: TNY_OPCODE_RTI};
        case T_JMP: return tny_word{u: TNY_OPCODE_JMP};
        case T_JE:  return tny_word{u: TNY_OPCODE_JMP};
        case T_JNE: return tny_word{u: TNY_OPCODE_JMP};
        case T_JL:  return tny_word{u: TNY_OPCODE_JMP};
        case T_JLE: return tny_word{u: TNY_OPCODE_JMP};
        case T_JG:  return tny_word{u: TNY_OPCODE_JMP};
        case T_JGE: return tny_word{u: TNY_OPCODE_JMP};
        default: std::cerr << "Fatal error unknown opcode (should never see this)" << std::endl; std::exit(EXIT_FAILURE);
    }
}

void Parser::set_destination(token token, tny_word* dest) {
    switch(token.type) {
        case T_REGISTER:        *dest = register_to_value(token.token_str); break;
        case T_PLUS:            *dest = tny_word{u: 0}; break;
        case T_MINUS:           *dest = tny_word{u: 1}; break;
        case T_NUMBER:          *dest = process_number(token.token_str); break;
        case T_LABEL:           *dest = process_label(token.token_str); break;
        case T_IDENTIFIER:      *dest = process_identifier(token.token_str); break;
        case T_CHARACTER:       *dest = process_character(token.token_str); break;
        case T_STRING:          *dest = process_string(token.token_str); break;
        case T_PACKED_STRING:   *dest = process_packed_string(token.token_str); break;
        /* this means its an opcode */
        default: *dest = token_to_opcode(token.type); break;
   }
   return;
}

tny_word Parser::process_packed_string(std::string s) {
    tny_word return_val;
    return_val.u = address.u;

    std::string val = " ";
    tny_word string_value;
    tny_uword index = 0;
    if(is_raw_line) {
        index = raw_line_index;
    }
    for(size_t i = 1; i < s.size() - 1; i++) {
        string_value.u = 0;
        val = " ";  // the space is important
        if(s[i] == '\\') {
           val += "\\";
           val += s[i+1];
           i += 1;
        }else {
           val += s[i];
        }

        tny_word bin0 = process_character(val);
        string_value.bytes.byte0 =  bin0.u;
        i += 1;

        if(i < s.size()-1) {
            val = " ";  // the space is important
            if(s[i] == '\\') {
                val += "\\";
                val += s[i+1];
                i += 1;
            }else {
                val += s[i];
            }
            tny_word bin1 = process_character(val);
            string_value.bytes.byte1 =  bin1.u;
        }

        collect_raw_listing(index, string_value);
        index++;

        push_binary_value(string_value);
    }

    /* null terminate the string */
    if(string_value.bytes.byte1 != 0) {
        tny_word zero;
        zero.u = 0;
        collect_raw_listing(index, zero);
        index++;

        push_binary_value(zero);
    }
    string_length = index;
    if(is_raw_line) {
        raw_line_index = index;
    }

    return return_val;
}

tny_word Parser::process_string(std::string s) {
    tny_word return_val;
    return_val.u = address.u;

    std::string val = " ";
    tny_word bin;
    tny_uword index = 0;
    if(is_raw_line) {
        index = raw_line_index;
    }
    for(size_t i = 1; i < s.size() - 1; i++) {
        val = " ";  // the space is important
        if(s[i] == '\\') {
           val += "\\";
           val += s[i+1];
           i += 1;
        }else {
           val += s[i];
        }
        bin = process_character(val);

        collect_raw_listing(index, bin);
        index++;

        push_binary_value(bin);
    }

    /* null terminate the string */
    if(bin.u != 0) {
        tny_word zero;
        zero.u = 0;
        collect_raw_listing(index, zero);
        index++;

        push_binary_value(zero);
    }

    string_length = index;

    if(is_raw_line) {
        raw_line_index = index;
    }

    return return_val;
}

tny_word Parser::process_identifier(std::string s) {
    tny_word result;

    if(consts_and_vars.contains(s)) {
        result.s =  consts_and_vars[s].value.s;
    }else {
        std::string line = token_line_str(pp, current);
        valid_program = log_error(current, ltrim(line) + "\tunknown identifier \"" + s + "\"");
    }

    if(p_negative.u) {
       result.s *= -1;
    }

    return result;
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
    running_warning_log = "";
    oss.str("");
    address.u = 0;
    raw_line_index = 0;
    is_raw_line = false;
    jump_inst = false;
    trace_log         = "";
    warning_log         = "";
    binary_listing = "";
    /* clear all binary words */
    bin_words.clear();
    label_resolutions++;

    /* reset instance count for labels */
    for (const auto& elm : labels) {
        labels[elm.first].instances = 0;
    }

    /* reset instance count for vars/constants */
    for (const auto& elm : consts_and_vars) {
        consts_and_vars[elm.first].instances = 0;
    }

    reset_lexer();
}

bool Parser::parse_program() {
    setup_program();

    while (current.type != T_EOL || !current.token_str.empty()) {
        parse_line();
        trace_parser(true);
    }

    error_log      =  running_error_log;      // let our error_log match the running one
    warning_log    =  running_warning_log;    // let our warning_log match the running one
    binary_listing =  oss.str();  // match our binary listing to the running one
    max_lines = std::to_string(previous.line_num).size(); // get total amount of maximum lines

    return (valid_program && pp.valid_program);
}

void Parser::parse_line() {
    if (match(T_EOL)) {
        return;
    }

    token saved = current;
    bool matched = parse_statement();

    if(!matched) {
        std::string line = token_line_str(pp,saved);
        valid_program = log_error(saved, ltrim(line) + "\tinvalid syntax!");
    }

    bool valid_statement = expect(T_EOL);

    if(!valid_statement && matched) {
        std::string line = token_line_str(pp,saved);
        valid_program = log_error(saved, ltrim(line) + "\tinvalid syntax!");
        skip_line();
    }
}

bool Parser::parse_statement() {
    p_reg1.u = 0;
    p_reg2.u = 0;
    p_immed.s = 0;
    p_condition_flags.u = 0;
    jump_inst = false;
    bool matched_statement = parse_label_line() ||
                             parse_constant_line() ||
                             parse_variable_line() ||
                             parse_raw_line() ||
                             parse_code_line();

    return matched_statement;
}

bool Parser::parse_label_line() {
    token label = current;

    if(!match(T_LABEL)) {
        return false;
    }

    std::string label_name = label.token_str;

    /* create listing entry */
    std::string line = token_line_str(pp, label);
    oss << "        " << std::setw(max_lines) << current.line_num << ": [           ]  " << line << "\n";

    if(!labels.contains(label_name)) {
        container obj = {value: address, instances: 1, line_num: (tny_uword)label.line_num, file_name: label.source_file};
        labels.insert({ label_name, obj });
    }else {
        labels[label_name].instances++;
        tny_uword line_num = labels[label_name].line_num;
        std::string file_name = labels[label_name].file_name;
        if(labels[label_name].instances > 1) {
            std::string line = token_line_str(pp, label);
            valid_program    =  log_error(label, ltrim(line) +
                                "\tduplicate label definition (defined in " + file_name +" on line " + std::to_string(line_num) + ")");
        }
    }

   return true;
}

bool Parser::parse_constant_line() {
    if(!match(T_CONSTANT)) {
        return false;
    }

    token constant_token = current;
    std::string name = constant_token.token_str;
    std::string line = token_line_str(pp, constant_token);

    /* create listing entry */
    oss << "        " << std::setw(max_lines) << current.line_num << ": [           ]  " << line << "\n";

    if(!match(T_IDENTIFIER)) {
        valid_program = log_error(constant_token, ltrim(line) + "\tinvalid identifier \"" + name + "\"");
        skip_line();
        return true;
    }

    tny_word value;
    bool valid_immed = parse_immediate(&value);
    if(!valid_immed) {
        valid_program = log_error(constant_token, ltrim(line) + "\tinvalid constant value");
        skip_line();
        return true;
    }

    if(!consts_and_vars.contains(name)) {
        container obj = {value: value, instances: 1, line_num: (tny_uword)constant_token.line_num, file_name:constant_token.source_file};
        consts_and_vars.insert({ name, obj });
    }else {
        consts_and_vars[name].instances++;
        tny_uword line_num = consts_and_vars[name].line_num;
        std::string file_name = consts_and_vars[name].file_name;
        if(consts_and_vars[name].instances > 1) {
            std::string line = token_line_str(pp, constant_token);
            valid_program    =  log_error(constant_token, ltrim(line) +
                                "\tduplicate identifier \"" + name + "\" (defined in " + file_name + " on line " + std::to_string(line_num) + ")");
        }
    }

    return true;

}

bool Parser::parse_variable_format() {
    tny_word value;
    tny_word zero;
    zero.s = 0;

    std::string line = token_line_str(pp, current);
    oss << "0x" << word_to_hex(address) << "  " << std::setw(max_lines) << current.line_num << ": [ ";

    /* if nothing push zero */
    if(current.type == T_EOL) {
        /* create listing entry */
        oss << word_to_hex(zero) << std::setw(9) << " ]  " << line << "\n";

        push_binary_value(zero);
        return true;
    }

    /* This means we are going to push zeros into memeory X times */
    if(match(T_LBRACKET) && parse_no_sign_immediate(&value) && match(T_RBRACKET)) {
         for(int i = 0; i < value.u; i++) {
            collect_raw_listing(i, zero);
            push_binary_value(zero);
         }
         tny_uword pad = value.u % 2 ? 8 : 0;
         oss << std::setw(pad) << "]  " << line << "\n";
         return true;
    }

    if(match(T_STRING, &value) || match(T_PACKED_STRING, &value)) {
        tny_uword pad = string_length % 2 ? 8 : 0;
        oss << std::setw(pad) << "]  " << line << "\n";
        return true;
    }

    /* loop through our immediates */
    tny_uword i = 0;
    while(parse_immediate(&value)) {
        collect_raw_listing(i, value);
        i++;
        push_binary_value(value);
    }

    tny_uword pad = i % 2 ? 8 : 0;
    oss << std::setw(pad) << "]  " << line << "\n";

    return current.type == T_EOL;

}

bool Parser::parse_variable_line() {
    if(!match(T_VARIABLE)) {
        return false;
    }

    token variable_token = current;
    std::string name = variable_token.token_str;
    std::string line = token_line_str(pp, variable_token);

    if(address.u == 0) {
        log_warning(current, "A variable at address 0x0000 is unsafe. It will be executed as code.");
    }

    if(!match(T_IDENTIFIER)) {
        valid_program = log_error(variable_token, ltrim(line) + "\tinvalid identifier \"" + name + "\"");
        skip_line();
        return false;
    }

    /* save the starting address */
    tny_word base_address = address;

    bool valid = parse_variable_format();

    if(!valid) {
        valid_program = log_error(variable_token, ltrim(line) + "\tinvalid variable format");
        skip_line();
        return true;
    }

    if(!consts_and_vars.contains(name)) {
        container obj = {value: base_address, instances: 1, line_num: (tny_uword)variable_token.line_num, file_name:variable_token.source_file};
        consts_and_vars.insert({ name, obj });
    }else {
        consts_and_vars[name].instances++;
        tny_uword line_num = consts_and_vars[name].line_num;
        std::string file_name = consts_and_vars[name].file_name;
        if(consts_and_vars[name].instances > 1) {
            std::string line = token_line_str(pp, variable_token);
            valid_program    =  log_error(variable_token, ltrim(line) +
                                "\tduplicate identifier \"" + name + "\" (defined in " + file_name + " on line " + std::to_string(line_num) + ")");
        }
    }

    return true;

}

bool Parser::parse_raw_line_value() {
    tny_word value;
    if(match(T_STRING, &value) || match(T_PACKED_STRING, &value)) {
        return true;
    }

    if(parse_immediate(&value)) {
        collect_raw_listing(raw_line_index,value);
        raw_line_index++;

        push_binary_value(value);
        return true;
    }

    return false;
}

bool Parser::parse_raw_line() {
    if(!match(T_RAW)) {
        return false;
    }
    is_raw_line = true;

    if(address.u == 0) {
        log_warning(current, "Data at address 0x0000 is unsafe. It will be executed as code.");
    }

    raw_line_index = 0;
    token t = current;
    oss << "0x" << word_to_hex(address) << "  " << std::setw(max_lines) << current.line_num << ": [ ";
    while(parse_raw_line_value()) {
        t = current;
    }

    std::string line = token_line_str(pp,current);

    tny_uword pad = raw_line_index % 2 ? 8 : 0;
    oss << std::setw(pad) << "]  " << line << "\n";

    if(current.type != T_EOL) {
        valid_program = log_error(current, ltrim(line) + "\t invalid raw line value \"" + t.token_str + "\"");
        skip_line();
    }

    is_raw_line = false;

    return true;
}

bool Parser::parse_raw_value(tny_word* immed) {
    if(match(T_CHARACTER, immed) || match(T_IDENTIFIER, immed) ||
       match(T_NUMBER, immed)    || match(T_LABEL, immed)) {
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
    /* all no sign immediates are positive */
    p_negative.u = 0;
    return parse_raw_value(immed);
}

bool Parser::parse_includes_immediate(tny_word* immed) {
    if(match(T_PLUS, &p_negative) || match(T_MINUS, &p_negative)) {
        return parse_no_sign_immediate(immed);
    }
    immed->u = 0;
    return true;
}

bool Parser::parse_includes_register(tny_word* reg) {
    if(match(T_PLUS, &p_negative) && match(T_REGISTER, reg)) {
        return true;
    }
    reg->u = TNY_REG_ZERO;
    return true;
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

bool Parser::parse_code_line() {
    bool matched_code_line = parse_set_instruction() ||
                             parse_lod_instruction() ||
                             parse_str_instruction() ||
                             parse_psh_instruction() ||
                             parse_pop_instruction() ||
                             parse_bts_instruction() ||
                             parse_btc_instruction() ||
                             parse_btf_instruction() ||
                             parse_cal_instruction() ||
                             parse_add_instruction() ||
                             parse_sub_instruction() ||
                             parse_mpy_instruction() ||
                             parse_div_instruction() ||
                             parse_mod_instruction() ||
                             parse_and_instruction() ||
                             parse_or_instruction()  ||
                             parse_xor_instruction() ||
                             parse_shf_instruction() ||
                             parse_shl_instruction() ||
                             parse_shr_instruction() ||
                             parse_rot_instruction() ||
                             parse_rol_instruction() ||
                             parse_ror_instruction() ||
                             parse_neg_instruction() ||
                             parse_cmp_instruction() ||
                             parse_dly_instruction() ||
                             parse_int_instruction() ||
                             parse_rti_instruction() ||
                             parse_jmp_instruction() ||
                             parse_je_instruction()  ||
                             parse_jne_instruction() ||
                             parse_jl_instruction()  ||
                             parse_jle_instruction() ||
                             parse_jg_instruction()  ||
                             parse_jge_instruction();


    return matched_code_line;
}

bool Parser::parse_set_instruction() {
    if(match(T_SET, &p_opcode)) {
        if(match(T_REGISTER, &p_reg1) && match(T_COMMA) && parse_register_and_immediate(&p_reg2, &p_immed)) {
                push_binary_instruction();
                return true;
        }
        skip_line();
    }
    return false;
}

bool Parser::parse_lod_instruction() {
    if(match(T_LOD, &p_opcode)) {
        if(match(T_REGISTER, &p_reg1) && match(T_COMMA) && match(T_LBRACKET)
        && parse_register_and_immediate(&p_reg2, &p_immed) && match(T_RBRACKET)) {
                push_binary_instruction();
                return true;
        }
        skip_line();
    }
    return false;
}

bool Parser::parse_str_instruction() {
    if(match(T_STR, &p_opcode)) {
        if(match(T_LBRACKET) && parse_register_and_immediate(&p_reg1, &p_immed)
            && match(T_RBRACKET) && match(T_COMMA) && match(T_REGISTER, &p_reg2)) {
                push_binary_instruction();
                return true;
        }
        skip_line();
    }
    return false;
}

bool Parser::parse_psh_format() {
    if(match(T_LBRACKET) && match(T_REGISTER, &p_reg1) && match(T_RBRACKET) && match(T_COMMA)
      && parse_register_and_immediate(&p_reg2, &p_immed)) {
        return true;
    }

    /* The stack pointer is the default register for psh */
    p_reg1.u = TNY_REG_SP;

    if(parse_register_and_immediate(&p_reg2, &p_immed)) {
        return true;
    }

    if(parse_immediate(&p_immed)) {
        p_reg2.u = TNY_REG_ZERO;
        return true;
    }

    return false;
}

bool Parser::parse_psh_instruction() {
    if(match(T_PSH, &p_opcode)) {
        if(parse_psh_format()) {
            push_binary_instruction();
            return true;
        }
        skip_line();
    }
    return false;
}

bool Parser::parse_pop_format() {
    /* pop will always have zeroes in the immed section */
    p_immed.s = 0;

    if(current.type == T_EOL) {
        /* pop uses the SP by default */
        p_reg2.u = TNY_REG_SP;
        return true;
    }

    if(match(T_COMMA) && match(T_LBRACKET) && match(T_REGISTER, &p_reg2) && match(T_RBRACKET)) {
        return true;
    }

    return false;
}

bool Parser::parse_pop_instruction() {
    if(match(T_POP, &p_opcode)) {
        if(match(T_REGISTER, &p_reg1) && parse_pop_format()) {
            push_binary_instruction();
            return true;
        }
        skip_line();
    }
    return false;
}

bool Parser::parse_bts_instruction() {
    if(match(T_BTS, &p_opcode)) {
        if(match(T_REGISTER, &p_reg1) && match(T_COMMA) && parse_register_and_immediate(&p_reg2, &p_immed)) {
                push_binary_instruction();
                return true;
        }
        skip_line();
    }
    return false;
}

bool Parser::parse_btc_instruction() {
    if(match(T_BTC, &p_opcode)) {
        if(match(T_REGISTER, &p_reg1) && match(T_COMMA) && parse_register_and_immediate(&p_reg2, &p_immed)) {
                push_binary_instruction();
                return true;
        }
        skip_line();
    }
    return false;
}

bool Parser::parse_btf_instruction() {
    if(match(T_BTF, &p_opcode)) {
        if(match(T_REGISTER, &p_reg1) && match(T_COMMA) && parse_register_and_immediate(&p_reg2, &p_immed)) {
                push_binary_instruction();
                return true;
        }
        skip_line();
    }
    return false;
}

bool Parser::parse_cal_instruction() {
    if(match(T_CAL, &p_opcode)) {
        p_reg1.u = TNY_REG_ZERO;
        if(parse_register_and_immediate(&p_reg2, &p_immed)) {
                push_binary_instruction();
                return true;
        }
        skip_line();
    }
    return false;
}

bool Parser::parse_add_instruction() {
    if(match(T_ADD, &p_opcode)) {
        if(match(T_REGISTER, &p_reg1) && match(T_COMMA) && parse_register_and_immediate(&p_reg2, &p_immed)) {
                push_binary_instruction();
                return true;
        }
        skip_line();
    }
    return false;
}

bool Parser::parse_sub_instruction() {
    if(match(T_SUB, &p_opcode)) {
        if(match(T_REGISTER, &p_reg1) && match(T_COMMA) && parse_register_and_immediate(&p_reg2, &p_immed)) {
                push_binary_instruction();
                return true;
        }
        skip_line();
    }
    return false;
}

bool Parser::parse_mpy_instruction() {
    if(match(T_MPY, &p_opcode)) {
        if(match(T_REGISTER, &p_reg1) && match(T_COMMA) && parse_register_and_immediate(&p_reg2, &p_immed)) {
                push_binary_instruction();
                return true;
        }
        skip_line();
    }
    return false;
}

bool Parser::parse_div_instruction() {
    if(match(T_DIV, &p_opcode)) {
        if(match(T_REGISTER, &p_reg1) && match(T_COMMA) && parse_register_and_immediate(&p_reg2, &p_immed)) {
                push_binary_instruction();
                return true;
        }
        skip_line();
    }
    return false;
}

bool Parser::parse_mod_instruction() {
    if(match(T_MOD, &p_opcode)) {
        if(match(T_REGISTER, &p_reg1) && match(T_COMMA) && parse_register_and_immediate(&p_reg2, &p_immed)) {
                push_binary_instruction();
                return true;
        }
        skip_line();
    }
    return false;
}

bool Parser::parse_and_instruction() {
    if(match(T_AND, &p_opcode)) {
        if(match(T_REGISTER, &p_reg1) && match(T_COMMA) && parse_register_and_immediate(&p_reg2, &p_immed)) {
                push_binary_instruction();
                return true;
        }
        skip_line();
    }
    return false;
}

bool Parser::parse_or_instruction() {
    if(match(T_OR, &p_opcode)) {
        if(match(T_REGISTER, &p_reg1) && match(T_COMMA) && parse_register_and_immediate(&p_reg2, &p_immed)) {
                push_binary_instruction();
                return true;
        }
        skip_line();
    }
    return false;
}

bool Parser::parse_xor_instruction() {
    if(match(T_XOR, &p_opcode)) {
        if(match(T_REGISTER, &p_reg1) && match(T_COMMA) && parse_register_and_immediate(&p_reg2, &p_immed)) {
                push_binary_instruction();
                return true;
        }
        skip_line();
    }
    return false;
}

bool Parser::parse_shf_instruction() {
    if(match(T_SHF, &p_opcode)) {
        if(match(T_REGISTER, &p_reg1) && match(T_COMMA) && parse_register_and_immediate(&p_reg2, &p_immed)) {
                push_binary_instruction();
                return true;
        }
        skip_line();
    }
    return false;
}

bool Parser::parse_shl_instruction() {
    if(match(T_SHL, &p_opcode)) {
        if(match(T_REGISTER, &p_reg1) && match(T_COMMA) && parse_immediate(&p_immed)) {
                /* negate the value */
                p_immed.s *= -1;
                p_reg2.u = TNY_REG_ZERO;
                push_binary_instruction();
                return true;
        }
        skip_line();
    }
    return false;
}

bool Parser::parse_shr_instruction() {
    if(match(T_SHR, &p_opcode)) {
        if(match(T_REGISTER, &p_reg1) && match(T_COMMA) && parse_immediate(&p_immed)) {
                p_reg2.u = TNY_REG_ZERO;
                push_binary_instruction();
                return true;
        }
        skip_line();
    }
    return false;
}

bool Parser::parse_rot_instruction() {
    if(match(T_ROT, &p_opcode)) {
        if(match(T_REGISTER, &p_reg1) && match(T_COMMA) && parse_register_and_immediate(&p_reg2, &p_immed)) {
                push_binary_instruction();
                return true;
        }
        skip_line();
    }
    return false;
}

bool Parser::parse_rol_instruction() {
    if(match(T_ROL, &p_opcode)) {
        if(match(T_REGISTER, &p_reg1) && match(T_COMMA) && parse_immediate(&p_immed)) {
                /* negate the value */
                p_immed.s *= -1;
                p_reg2.u = TNY_REG_ZERO;
                push_binary_instruction();
                return true;
        }
        skip_line();
    }
    return false;
}

bool Parser::parse_ror_instruction() {
    if(match(T_ROR, &p_opcode)) {
        if(match(T_REGISTER, &p_reg1) && match(T_COMMA) && parse_immediate(&p_immed)) {
                p_reg2.u = TNY_REG_ZERO;
                push_binary_instruction();
                return true;
        }
        skip_line();
    }
    return false;
}

bool Parser::parse_neg_instruction() {
    if(match(T_NEG, &p_opcode)) {
        if(match(T_REGISTER, &p_reg1)) {
                /*  NEG rX == MPY rX, rZ - 1 */
                p_reg2.u = TNY_REG_ZERO;
                p_immed.s = -1;
                push_binary_instruction();
                return true;
        }
        skip_line();
    }
    return false;
}

bool Parser::parse_cmp_instruction() {
    if(match(T_CMP, &p_opcode)) {
        if(match(T_REGISTER, &p_reg1) && match(T_COMMA) && parse_register_and_immediate(&p_reg2, &p_immed)) {
                push_binary_instruction();
                return true;
        }
        skip_line();
    }
    return false;
}

bool Parser::parse_dly_register_format() {
    if(match(T_COMMA)) {
        return parse_register_and_immediate(&p_reg2, &p_immed);
    }

    /* we need to swap reg1 and reg2 */
    p_reg2.u = p_reg1.u;
    p_reg1.u = TNY_REG_ZERO;
    if(parse_immediate(&p_immed)) {
        return true;
    }

    return current.type == T_EOL;
}

bool Parser::parse_dly_format() {

    if(match(T_REGISTER, &p_reg1)) {
        return parse_dly_register_format();
    }

    if(parse_immediate(&p_immed)) {
        return true;
    }

    return false;
}

bool Parser::parse_dly_instruction() {
    if(match(T_DLY, &p_opcode)) {
        if(parse_dly_format()) {
                push_binary_instruction();
                return true;
        }
        skip_line();
    }
    return false;
}

bool Parser::parse_int_instruction() {
    if(match(T_INT, &p_opcode)) {
        if(parse_register_and_immediate(&p_reg2, &p_immed)) {
                push_binary_instruction();
                return true;
        }
        skip_line();
    }
    return false;
}

bool Parser::parse_rti_instruction() {
    if(match(T_RTI, &p_opcode)) {
        if(current.type == T_EOL) {
                push_binary_instruction();
                return true;
        }
        skip_line();
    }
    return false;
}

bool Parser::parse_jmp_instruction() {
    if(match(T_JMP, &p_opcode)) {
        if(parse_register_and_immediate(&p_reg1, &p_immed)) {
                jump_inst = true;
                p_condition_flags.u = 0;
                push_binary_instruction();
                return true;
        }
        skip_line();
    }
    return false;
}

bool Parser::parse_jne_instruction() {
    if(match(T_JNE, &p_opcode)) {
        if(parse_register_and_immediate(&p_reg1, &p_immed)) {
                jump_inst = true;
                p_condition_flags.u = 3;
                push_binary_instruction();
                return true;
        }
        skip_line();
    }
    return false;
}

bool Parser::parse_je_instruction() {
    if(match(T_JE, &p_opcode)) {
        if(parse_register_and_immediate(&p_reg1, &p_immed)) {
                jump_inst = true;
                p_condition_flags.u = 4;
                push_binary_instruction();
                return true;
        }
        skip_line();
    }
    return false;
}

bool Parser::parse_jl_instruction() {
    if(match(T_JL, &p_opcode)) {
        if(parse_register_and_immediate(&p_reg1, &p_immed)) {
                jump_inst = true;
                p_condition_flags.u = 2;
                push_binary_instruction();
                return true;
        }
        skip_line();
    }
    return false;
}

bool Parser::parse_jle_instruction() {
    if(match(T_JLE, &p_opcode)) {
        if(parse_register_and_immediate(&p_reg1, &p_immed)) {
                jump_inst = true;
                p_condition_flags.u = 6;
                push_binary_instruction();
                return true;
        }
        skip_line();
    }
    return false;
}

bool Parser::parse_jg_instruction() {
    if(match(T_JG, &p_opcode)) {
        if(parse_register_and_immediate(&p_reg1, &p_immed)) {
                jump_inst = true;
                p_condition_flags.u = 1;
                push_binary_instruction();
                return true;
        }
        skip_line();
    }
    return false;
}

bool Parser::parse_jge_instruction() {
    if(match(T_JGE, &p_opcode)) {
        if(parse_register_and_immediate(&p_reg1, &p_immed)) {
                jump_inst = true;
                p_condition_flags.u = 5;
                push_binary_instruction();
                return true;
        }
        skip_line();
    }
    return false;
}
