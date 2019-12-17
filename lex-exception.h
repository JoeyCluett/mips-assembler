#pragma once

#include <string>
#include <vector>
#include <iostream>
#include "lex-data.h"

class LexException {
private:

    std::vector<char>* v;
    size_t idx;

public:

    LexException(c_iter iter, std::vector<char>& src) {
        v = &src;
        idx = iter - src.begin();
    }

    void print(void) {

        auto iter = v->begin() + this->idx;
        while(iter != v->begin() && *iter != '\n')
            --iter;

        if(*iter == '\n')
            ++iter;

        while(iter != v->end() && *iter != '\n')
            std::cerr << char(*iter) << std::flush;

        std::cerr << std::endl << std::flush;

    }

};
