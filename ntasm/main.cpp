#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <filesystem>

#include "lexer.h"
#include "preprocessor.h"
#include "parser.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "usage: assembler <file.asm | file.S>\n";
        return 1;
    }

    std::filesystem::path asm_filename(argv[1]);

	if(asm_filename.extension() != ".asm" && asm_filename.extension() != ".S") {
        std::cerr << "Expected a \".asm\"  or \".S\" file..." << std::endl;
        std::cerr << "Usage:   ntasm <file>" << std::endl;
        std::exit(EXIT_FAILURE);
	}

	if((!std::filesystem::exists(asm_filename))) {
        std::cerr << "File " << asm_filename << " does not exist." << std::endl;
        std::cerr << "Usage:   ntasm <file>" << std::endl;
		exit(EXIT_FAILURE);
	}

    std::ifstream file(asm_filename);
    if (!file) {
        std::cerr << "could not open file " << asm_filename.string() << "\n";
        return EXIT_FAILURE;
    }

    std::stringstream file_buffer;
    file_buffer << file.rdbuf();
    std::string asm_lines = file_buffer.str();

    Lexer* lexer = new Lexer(asm_lines, asm_filename.string());
    Preprocessor preprocessor(lexer);
    Parser parser(preprocessor, false);
    
    /* First pass identify labels/identifiers */
    parser.parse_program();

    /* Second pass resolve labels/identifiers */
    bool valid_program = parser.parse_program();

    if(parser.debug_mode) {
        std::cout << parser.trace_log << std::endl;    
    }

    if(valid_program) {
        /*for(auto t : parser.bin_words) {
            std::cout << std::hex << t.u << std::endl;
        }*/
        std::cout << parser.binary_listing << std::endl;
        if(parser.warning_log != "") {
            std::cout << "------------------------------------" << std::endl; 
            std::cout << parser.warning_log << std::endl; 
        }
    }else {
        std::cerr << parser.error_log << std::endl;
        std::cout << "There were errors. No binary output.\n";
        return 1;
    }

    return 0;
}

