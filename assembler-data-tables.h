#pragma once

#include "lexer.h"
#include "instruction-format.h"
#include <map>
#include <vector>
#include <string>

struct AssemblerData {

    std::map<std::string, lexeme>      constant_lookup;    // symbolic constants
    std::vector<char>                  data_vector;        // location for data used in the program
    std::map<std::string, std::size_t> data_vector_lookup; // know where to look in the data vector for things (PIC)

    std::map<std::string, std::size_t> jump_labels; // symbolic jump locations. will turn into offsets at the end (PIC)
    std::vector<Instruction> instruction_list;

};

struct GeneratedCode {
    
};
