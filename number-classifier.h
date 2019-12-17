#pragma once

#include <string>

const int NC_int   = 0; // positive int
const int NC_real  = 1; // positive real
const int NC_octal = 2; // sign meaningless
const int NC_hex   = 3; // sign meaningless
const int NC_mint  = 4; // minus int
const int NC_mreal = 5; // minus real

struct NumberClassification {
    int type;
    std::string str;
};

struct EvaluatedNumber {
    int type;
    union {
        int i32;
        unsigned int u32;
        float f32;
        double f64;
    };
};

typedef NumberClassification NumClass;
typedef EvaluatedNumber      EvalNum;

auto classify_number(std::string num) -> NumClass;
auto evaluate_number(std::string num) -> EvalNum;

auto classify_number(std::string num) -> NumClass {

    switch(num.size()) {
        case 0:
            throw std::runtime_error("classify_number : invalid size");
            break;
        case 1:
            if(num[0] >= '0' && num[0] <= '9')
                return { NC_int, num };
            else
                throw std::runtime_error("classify_number : invalid number format");
            break;
        default:
            {
                int num_per = 0;
                int num_x   = 0;
                int num_e   = 0;
                int num_min = 0;

                for(char c : num) {
                    // take a quick survey of the characters in the string
                    if(c == '.')      num_per++;
                    else if(c == 'x') num_x++;
                    else if(c == 'e') num_e++;
                    else if(c == '-') num_min++;
                }

                // check a bunch of things
                if(
                        num_min > 2 || num_x > 1 ||
                        num_e > 1 || num_per > 1 ||
                        (num_per && num_x)) // cant be real AND hex at the same time
                    throw std::runtime_error("classify_number : invalid number format");

                if(num_min) {
                    // negative number. cant be hex or octal
                    if(num_per || num_e) return { NC_mreal, num };
                    else return { NC_mint, num };
                }
                else {
                    // positive number
                    if(num[0] == '0' && num[1] == 'x') return { NC_hex, num };
                    if(num_e || num_per)               return { NC_real, num };
                    if(num[0] == '0' && num[1] != 'x' && !num_per && !num_e)
                        return { NC_octal, num };

                    // if nothing else fits, its an integer
                    return { NC_int, num };
                }
            }
            break;
    }
}

auto evaluate_number(std::string num) -> EvalNum {
    auto classified_num = classify_number(num);

    switch(classified_num.type) {
        case NC_int:   return { NC_int,  std::stoi(num) };
        case NC_octal: return { NC_int,  std::stoi(num, 0, 8) };
        case NC_hex:   return { NC_int,  std::stoi(num, 0, 16) };
        case NC_mint:  return { NC_int,  std::stoi(num) };

        case NC_real:
        case NC_mreal:
            {
                EvaluatedNumber en;
                en.type = NC_real;
                en.f64 = std::stod(num);
                return en;
            }
            break;
        default:
            throw std::runtime_error("evaluate_number : unknown internal error");
    }

}
