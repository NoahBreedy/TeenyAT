#include <cctype>
#include <string>
#include <iostream>

#include "token.h"
#include "preprocessor.h"

bool log_error(const token& tok, const std::string& message);
void log_warning(const token& tok, const std::string& message);
std::string token_line_str(Preprocessor& pp, const token& tok);
std::string token_line_str(std::string text, const token& tok);
std::string ltrim(std::string s);

extern std::string running_error_log;
extern std::string running_warning_log;
