#pragma once

#include <string>
#include <iostream>
#include <map>

int get_register_index(std::string r) {
    if(r[0] != '$') 
        throw std::runtime_error("get_register_index : invalid register format");

    std::string name = r.substr(1, std::string::npos);

    const std::map<std::string, int> register_lookup = {

        { "zero", 0 },
        { "at",   1 },
        { "v0",   2 },  { "v1",   3 },
        { "a0",   4 },  { "a1",   5 },  { "a2",   6 },  { "a3",   7 },
        { "t0",   8 },  { "t1",   9 },  { "t2",   10 }, { "t3",   11 }, { "t4",   12 }, { "t5",   13 }, { "t6",   14 }, { "t7",   15 },
        { "s0",   16 }, { "s1",   17 }, { "s2",   18 }, { "s3",   19 }, { "s4",   20 }, { "s5",   21 }, { "s6",   22 }, { "s7",   23 },
        { "t8",   24 }, { "t9",   25 },
        { "k0",   26 }, { "k1",   27 },
        { "gp",   28 }, { "sp",   29 }, { "fp",   30 }, { "ra",   31 },

    };

    if(register_lookup.find(name) == register_lookup.end())
        throw std::runtime_error("get_register_index : invalid register format");

    return register_lookup.at(name);
}
