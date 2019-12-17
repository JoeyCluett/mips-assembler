#pragma once

/*

    evaluator.h

    for an assembler, it is less essential that an abstract syntax
    tree is created. the instruction sequence can be evaulated
    in-place and proper lexemes can be checked in a single pass. i
    use a simple state machine here to guide that process

*/

#include "lexer.h"
#include "number-classifier.h"
#include "assembler-data-tables.h"
#include "eval-exception.h"
#include "instruction-evaluator.h"
#include <map>
#include <string>
#include <iostream>

static void place_string_in_vector(std::string str, std::vector<char>& datavector) {

    auto iter = str.begin();
    auto end = str.end();

    while(iter != end) {

        if(*iter == '\\') {
            iter++;
            switch(*iter) {
                case 'n': datavector.push_back('\n'); break;
                case 't': datavector.push_back('\t'); break;
                case '"': datavector.push_back('"'); break;
                case '\\': datavector.push_back('\\'); break;
                default:
                    throw std::runtime_error("malformed string literal");
            }
        }
        else {
            datavector.push_back(*iter);
        }
        
        iter++;
    }

}

auto generate_number_vector(
        token_iter& iter,
        std::vector<lexeme>& tokens,
        std::vector<char>& src,
        AssemblerData* ad) -> std::vector<EvaluatedNumber> {

    std::vector<EvaluatedNumber> v;

    //iter++;
    if(iter->type != number_literal) 
        throw std::runtime_error("generate_number_vector : expecting a leading number literal in data directive");
    v.push_back(evaluate_number(iter->to_string(src)));

    iter++;
    while(iter->type == comma) {
        iter++;
        if(iter->type != number_literal) throw std::runtime_error("generate_number_vector : expecting a number literal");
        v.push_back(evaluate_number(iter->to_string(src)));
        iter++;
    }

    return v;
    // iter now points to the next token after the comma seperated list
}

//.word .half .byte .float .double .space .ascii .asciiz
auto eval_data_directive(
        token_iter iter,
        std::vector<lexeme>& tokens,
        std::vector<char>& src,
        AssemblerData* ad) -> token_iter {

    auto val = iter->to_string(src);
    iter++;

    std::cout << "\t" << val << "\n" << std::flush;

    if(val == ".word") { // 32 bits
        auto v = generate_number_vector(iter, tokens, src, ad);

        //std::cout << "generated vector of size " << v.size() << "\n" << std::flush;
        //std::cout << "next lexeme( " << iter->to_string(src) << " )\n" << std::flush;
        //for(auto n : v)
        //    std::cout << n.i32 << ' ';
        //std::cout << "\n" << std::flush;
        //exit(1);

        for(auto n : v) {
            if(n.type != NC_int)
                throw std::runtime_error("expecting signed or unsigned integer");

            union {
                int i32;
                char buf[4];
            };

            i32 = n.i32;

            for(int i : {0, 1, 2, 3})
                ad->data_vector.push_back(buf[i]);
        }
    }
    else if(val == ".half") { // 16 bits
        auto v = generate_number_vector(iter, tokens, src, ad);

        for(auto n : v) {
            if(n.type != NC_int)
                throw std::runtime_error("expecting signed or unsigned half word");

            union {
                int16_t i16;
                char buf[2];
            };
        
            i16 = n.u32 & 0x0000FFFF;

            for(int i : {0, 1})
                ad->data_vector.push_back(buf[i]);
        }

    }
    else if(val == ".byte") { // 8 bits
        auto v = generate_number_vector(iter, tokens, src, ad);

        for(auto n : v) {
            if(n.type != NC_int)
                throw std::runtime_error("expecting signed or unsigned byte value");

            union {
                uint8_t u8;
                char buf;
            };

            u8 = n.u32 & 0x000000FF;

            ad->data_vector.push_back(buf);
        }

    }
    else if(val == ".float") { // 32 bits
        auto v = generate_number_vector(iter, tokens, src, ad);

        for(auto n : v) {
            if(n.type != NC_real)
                throw std::runtime_error("expecting signed or unsigned float value");

            union {
                float f32;
                char buf[4];
            };

            f32 = (float)n.f64;

            for(int i : {0, 1, 2, 3})
                ad->data_vector.push_back(buf[i]);
        }

    }
    else if(val == ".double") { // 64 bits
        auto v = generate_number_vector(iter, tokens, src, ad);

        for(auto n : v) {
            if(n.type != NC_real)
                throw std::runtime_error("expecting signed or unsigned double value");

            union {
                double f64;
                char buf[8];
            };

            f64 = n.f64;

            for(int i = 0; i < 8; i++)
                ad->data_vector.push_back(buf[i]);
        }

    }
    else if(val == ".ascii") {

        if(iter->type != string_literal) throw std::runtime_error("expecting string literal");
        std::string strval = iter->to_string(src);
        place_string_in_vector(strval, ad->data_vector);

        iter++;

    }
    else if(val == ".asciiz") {

        if(iter->type != string_literal) throw std::runtime_error("expecting string literal");
        std::string strval = iter->to_string(src);
        place_string_in_vector(strval, ad->data_vector);
        ad->data_vector.push_back('\0'); // null-terminate

        iter++;

    }

    return iter;

}

auto evaluate(std::vector<lexeme>& tokens, std::vector<char>& src) -> AssemblerData* {

    const int state_default = 0;
    const int state_data    = 1;
    const int state_text    = 2;

    int global_state = state_default;
    auto cur = tokens.begin();
    auto end_iter = tokens.end();

    AssemblerData* ad_ptr = new AssemblerData;

    auto& constant_lookup    = ad_ptr->constant_lookup;    // symbolic constants
    auto& data_vector        = ad_ptr->data_vector;        // location for data used in the program
    auto& data_vector_lookup = ad_ptr->data_vector_lookup; // know where to look in the vector for things (PIC)
    auto& jump_labels        = ad_ptr->jump_labels;        // symbolic jump locations. will turn into offsets at the end (PIC)

    while(cur != end_iter) {
        switch(global_state) {
            case state_default:
                std::cout << "default segment\n" << std::flush;
                {
                    if(cur->type == directive) {
                        auto val = cur->to_string(src);
                        if(val == ".text") global_state = state_text;
                        else if(val == ".data") global_state = state_data;
                        else
                            throw std::runtime_error("invalid directive : " + cur->to_string(src));

                        cur++;
                    }
                    else if(cur->type == symbolic_constant) {
                        // next lexeme is an equals sign
                        if((cur+1)->type != equals)
                            throw std::runtime_error("expecting an equals sign");

                        // next next lexeme is a number literal
                        if((cur+2)->type != number_literal)
                            throw std::runtime_error("expecting a number literal");

                        // if the constant already exists, throw error
                        if(constant_lookup.find(cur->to_string(src)) != constant_lookup.end())
                            throw std::runtime_error("constant already defined");

                        constant_lookup.insert({ cur->to_string(src), *(cur+2) });
                        cur+=3;
                    }
                    else {
                        throw std::runtime_error("expecting either a directive or symbolic constant");
                    }

                }
                break;
            case state_data:
                std::cout << "data segment\n" << std::flush;
                {
                    if(cur->type == directive) {
                        // may be any of [ .word .half .byte .float .double .space .ascii .asciiz ]
                        auto val = cur->to_string(src);
                        if(val == ".align") {
                            std::cout << "\t.align\n" << std::flush;
                            // next lexeme is constant number
                            if((cur+1)->type != number_literal)
                                throw std::runtime_error("expecting number literal");

                            int align_by = std::stoi((cur+1)->to_string(src));
                            int bytes = (1 << align_by);

                            int rem = data_vector.size() % bytes;
                            bytes -= rem;

                            // place null bytes in data vector until padded
                            for(int i = 0; i < bytes; i++)
                                data_vector.push_back('\0');

                            cur += 2;
                        }
                        else if(val == ".space") {
                            
                            std::cout << "\t.space\n" << std::flush;

                            if((cur+1)->type != number_literal)
                                throw std::runtime_error("expecting a number literal");

                            auto en = evaluate_number((cur+1)->to_string(src));
                            if(en.type == NC_int) {
                                int spc = en.i32;
                                for(int i = 0; i < spc; i++) {
                                    data_vector.push_back('\0');
                                }
                            }
                            else {
                                throw std::runtime_error("expecting an integer");
                            }

                            cur += 2;

                        }
                        else if(val == ".text") {
                            std::cout << "\t.text\n" << std::flush;
                            cur++;
                            global_state = state_text;
                        }
                        else {

                            //std::cout << "data directive\n" << std::flush;
                            cur = eval_data_directive(cur, tokens, src, ad_ptr);

                        }
                    }
                    else if(cur->type == jump_label) {
                        
                        std::cout << "\tlabel(  " << cur->to_string(src) << "  )\n" << std::flush;

                        // create an entry in the data lut
                        data_vector_lookup.insert({ cur->to_string(src), data_vector.size() });
                        cur++;

                    }
                }
                break;
            case state_text:
                std::cout << "text segment\n" << std::flush;
                //exit(1);
                //throw EvalException(ad_ptr);
                {
                    if(cur->type == jump_label) {
                        std::cout << "\tlabel(  " << cur->to_string(src) << "  )\n" << std::flush;

                        ad_ptr->jump_labels.insert({ cur->to_string(src), ad_ptr->instruction_list.size() }); 
                        cur++;
                    }
                    else if(cur->type == instruction) {
                        //throw EvalException(ad_ptr);
                    
                        cur = evaluate_instruction(cur, tokens, src, ad_ptr);
                    }
                }
                break;
            default:
                throw std::runtime_error("evaluate : invalid internal state. current token: '" + cur->to_string(src) + "'");
        }
    }

    return ad_ptr;

}
