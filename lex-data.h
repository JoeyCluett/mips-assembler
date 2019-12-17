#pragma once

#include <vector>
#include <string>

const int register_reference = 0;
const int comma              = 1;
const int directive          = 2;
const int instruction        = 3;
const int symbolic_constant  = 4;
const int jump_label         = 5;
const int string_literal     = 6;
const int number_literal     = 7;
const int equals             = 8;
const int open_paren         = 9;
const int close_paren        = 10;

struct lexeme {
    int type;
    int start;
    int end;

    std::string to_string(std::vector<char>& src) {
        return std::string( src.begin() + start, src.begin() + end );
    }
};

typedef std::vector<lexeme>::iterator token_iter;
typedef std::vector<char>::iterator c_iter;
