#include <cctype>
#include <string>
#include <iostream>

#include "token.h"
#include "preprocessor.h"

void log_error(const token& tok, const std::string& message);
std::string token_line_str(Preprocessor& pp, const token& tok);
std::string ltrim(std::string s);
