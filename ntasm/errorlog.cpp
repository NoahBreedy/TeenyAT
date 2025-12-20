#include "errorlog.h"

std::string running_error_log = "";

bool log_error(const token& tok, const std::string& message) {
   running_error_log += "ERROR, ";
   running_error_log += tok.source_file + " on line " + std::to_string(tok.line_num) + ": ";
   running_error_log += message + "\n";
   return false;
}

/* dumb old turbo pascale to the rescue lol */
std::string ltrim(std::string s) {
    size_t i = 0;
    while (i < s.size() && std::isspace(s[i])){
        ++i;
        s.erase(0, i);
    }
    return s;
}

std::string token_line_str(Preprocessor& pp, const token& tok) {
    /* get the current lexers assembly lines */
    std::string text = pp.current_lexer().src;
    std::istringstream iss(text);
    std::string line;
    int current_line_num = 1;
    while (std::getline(iss, line)) {
        if (current_line_num == tok.line_num)
            return line;
        ++current_line_num;
    }
    return "";
}

std::string token_line_str(std::string text, const token& tok) {
    std::istringstream iss(text);
    std::string line;
    int current_line_num = 1;
    while (std::getline(iss, line)) {
        if (current_line_num == tok.line_num)
            return line;
        ++current_line_num;
    }
    return "";
}
