#include "errorlog.h"

void log_error(const token& tok, const std::string& message) {
   std::cerr << "ERROR, ";
   std::cerr << tok.source_file << " on line " << tok.line_num << ": ";
   std::cerr << message << std::endl;
   return;
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
