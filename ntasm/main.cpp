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
    Parser parser(preprocessor);

    parser.parse_program();

    std::cout << "Parse completed successfully.\n";
    return 0;
}

