#include "errorlog.h"

void log_error(const token& tok, const std::string& message) {
   std::cerr << "ERROR, ";
   std::cerr << tok.source_file << " on line " << tok.line_num << ": ";
   std::cerr << message << std::endl;
   return;
}
