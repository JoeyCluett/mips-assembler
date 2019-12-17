#pragma once

#include <vector>
#include "lexer.h"

void print_lexeme_list(std::vector<lexeme>& v, std::vector<char>& src) {
    for(auto& l : v) {
        switch(l.type) {
            case register_reference:
                std::cout << "REGISTER_REFERENCE : "; break;
            case comma:
                std::cout << "COMMA              : "; break;
            case directive:
                std::cout << "DIRECTIVE          : "; break;
            case instruction:
                std::cout << "INSTRUCTION        : "; break;
            case symbolic_constant:
                std::cout << "SYMBOLIC CONSTANT  : "; break;
            case jump_label:
                std::cout << "JUMP LABEL         : "; break;
            case string_literal:
                std::cout << "STRING LITERAL     : "; break;
            case number_literal:
                std::cout << "NUMBER LITERAL     : "; break;
            case equals:
                std::cout << "EQUALS             : "; break;
            case open_paren:
                std::cout << "OPEN PARENTHESES   : "; break;
            case close_paren:
                std::cout << "CLOSE PARENTHESES  : "; break;
            default:
                throw std::runtime_error("unknown lexeme type: " + std::to_string(l.type));
        }

        std::cout << l.to_string(src) << "\n" << std::flush;
    }
}
