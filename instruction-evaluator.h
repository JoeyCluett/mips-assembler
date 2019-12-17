#pragma once

/*
    takes an instruction sequence passed via token list 
    and src vector and returns a fully populated 
    Instruction structure
*/

#include "instruction-format.h"
#include "lexer.h"
#include "assembler-data-tables.h"
#include "evaluator.h"
#include "instruction_list.h"
#include "register_names.h"
#include "number-classifier.h"
#include <functional>
#include <assert.h>

token_iter evaluate_instruction(
        token_iter iter,
        std::vector<lexeme>& tokens,
        std::vector<char>& src,
        AssemblerData* ad) {

    auto opcode = iter->to_string(src);
    std::cout << "\tOPCODE : " << opcode << '\n' << std::flush;

    if(instruction_list.find(opcode) == instruction_list.end())
        throw std::runtime_error("unknown opcode : " + opcode);

    Instruction instruction;

    static const std::map<std::string, std::function<void(void)>> opcodemap = {
        {
            "add",
            [&]() {
                // ensure certain instruction criteria are met
                assert((iter+1)->type == register_reference);
                assert((iter+2)->type == comma);
                assert((iter+3)->type == register_reference);
                assert((iter+4)->type == comma);
                assert((iter+5)->type == register_reference);

                instruction.opcode   = inst_add;
                instruction.des.i32  = get_register_index((iter+1)->to_string(src));
                instruction.src1.i32 = get_register_index((iter+3)->to_string(src));
                instruction.src2.i32 = get_register_index((iter+5)->to_string(src));

                ad->instruction_list.push_back(instruction);
                iter += 6;
            }
        },
        {
            "addi",
            [&]() {
                assert((iter+1)->type == register_reference);
                assert((iter+2)->type == comma);
                assert((iter+3)->type == register_reference);
                assert((iter+4)->type == comma);
                assert((iter+5)->type == number_literal || (iter+5)->type == symbolic_constant);

                instruction.opcode = inst_addi;
                instruction.des.i32  = get_register_index((iter+1)->to_string(src));
                instruction.src1.i32 = get_register_index((iter+3)->to_string(src));

                switch((iter+5)->type) {
                    case number_literal:
                        {
                            auto en = evaluate_number((iter+5)->to_string(src));
                            assert(en.type == NC_int);
                            instruction.imm.i32 = en.i32;
                        }
                        break;
                    case symbolic_constant:
                        {
                            // need to fetch the constant from the constants table
                            auto val = (iter+5)->to_string(src);
                            assert(ad->constant_lookup.find(val) != ad->constant_lookup.end());
                            auto immval = ad->constant_lookup.at(val).to_string(src);
                            auto en = evaluate_number(immval);
                            assert(en.type == NC_int);
                            instruction.imm.i32 = en.i32;
                        }
                        break;
                    default: exit(1); break;
                }

                ad->instruction_list.push_back(instruction);
                iter += 6;
            }
        },
        {
            "addiu",
            [&]() {
                assert((iter+1)->type == register_reference);
                assert((iter+2)->type == comma);
                assert((iter+3)->type == register_reference);
                assert((iter+4)->type == comma);
                assert((iter+5)->type == number_literal || (iter+5)->type == symbolic_constant);

                instruction.opcode = inst_addiu;
                instruction.des.i32  = get_register_index((iter+1)->to_string(src));
                instruction.src1.i32 = get_register_index((iter+3)->to_string(src));

                switch((iter+5)->type) {
                    case number_literal:
                        {
                            auto en = evaluate_number((iter+5)->to_string(src));
                            assert(en.type == NC_int);
                            instruction.imm.u32 = en.u32;
                        }
                        break;
                    case symbolic_constant:
                        {
                            // need to fetch the constant from the constants table
                            auto val = (iter+5)->to_string(src);
                            assert(ad->constant_lookup.find(val) != ad->constant_lookup.end());
                            auto immval = ad->constant_lookup.at(val).to_string(src);
                            auto en = evaluate_number(immval);
                            assert(en.type == NC_int);
                            instruction.imm.u32 = en.u32;
                        }
                        break;
                    default: exit(1); break;
                }

                ad->instruction_list.push_back(instruction);
                iter += 6;
            }
        },
        {
            "addu",
            [&]() {
                // ensure certain instruction criteria are met
                assert((iter+1)->type == register_reference);
                assert((iter+2)->type == comma);
                assert((iter+3)->type == register_reference);
                assert((iter+4)->type == comma);
                assert((iter+5)->type == register_reference);

                instruction.opcode   = inst_addu;
                instruction.des.i32  = get_register_index((iter+1)->to_string(src));
                instruction.src1.i32 = get_register_index((iter+3)->to_string(src));
                instruction.src2.i32 = get_register_index((iter+5)->to_string(src));

                ad->instruction_list.push_back(instruction);
                iter += 6;
            }
        },
        {
            "and",
            [&]() {
                // ensure certain instruction criteria are met
                assert((iter+1)->type == register_reference);
                assert((iter+2)->type == comma);
                assert((iter+3)->type == register_reference);
                assert((iter+4)->type == comma);
                assert((iter+5)->type == register_reference);

                instruction.opcode   = inst_and;
                instruction.des.i32  = get_register_index((iter+1)->to_string(src));
                instruction.src1.i32 = get_register_index((iter+3)->to_string(src));
                instruction.src2.i32 = get_register_index((iter+5)->to_string(src));

                ad->instruction_list.push_back(instruction);
                iter += 6;
            }
        },
        {
            "andi",
            [&]() {
                assert((iter+1)->type == register_reference);
                assert((iter+2)->type == comma);
                assert((iter+3)->type == register_reference);
                assert((iter+4)->type == comma);
                assert((iter+5)->type == number_literal || (iter+5)->type == symbolic_constant);

                instruction.opcode = inst_andi;
                instruction.des.i32  = get_register_index((iter+1)->to_string(src));
                instruction.src1.i32 = get_register_index((iter+3)->to_string(src));

                switch((iter+5)->type) {
                    case number_literal:
                        {
                            auto en = evaluate_number((iter+5)->to_string(src));
                            assert(en.type == NC_int);
                            instruction.imm.i32 = en.i32;
                        }
                        break;
                    case symbolic_constant:
                        {
                            // need to fetch the constant from the constants table
                            auto val = (iter+5)->to_string(src);
                            assert(ad->constant_lookup.find(val) != ad->constant_lookup.end());
                            auto immval = ad->constant_lookup.at(val).to_string(src);
                            auto en = evaluate_number(immval);
                            assert(en.type == NC_int);
                            instruction.imm.i32 = en.i32;
                        }
                        break;
                    default: exit(1); break;
                }

                ad->instruction_list.push_back(instruction);
                iter += 6;
            }
        },
        {
            "beq",
            [&]() {
                assert((iter+1)->type == register_reference);
                assert((iter+2)->type == comma);
                assert((iter+3)->type == register_reference);
                assert((iter+4)->type == comma);
                assert((iter+5)->type == symbolic_constant);

                instruction.opcode = inst_beq;
                instruction.des.i32  = get_register_index((iter+1)->to_string(src));
                instruction.src1.i32 = get_register_index((iter+3)->to_string(src));

                // branch target must be a symbolic reference
                //auto val = (iter+5)->to_string(src);
                //assert(ad->constant_lookup.find(val) != ad->constant_lookup.end());
                //auto immval = ad->constant_lookup.at(val).to_string(src);
                //auto en = evaluate_number(immval);
                //assert(en.type == NC_int);
                //instruction.imm.i32 = en.i32;

                // store the index of the label. later these will all be checked
                instruction.imm.u32 = (iter+5) - tokens.begin();

                ad->instruction_list.push_back(instruction);
                iter += 6;
            }
        },
        {
            "bgez",
            [&]() {
                assert((iter+1)->type == register_reference);
                assert((iter+2)->type == comma);
                assert((iter+3)->type == symbolic_constant);

                instruction.opcode = inst_bgez;
                instruction.des.i32  = get_register_index((iter+1)->to_string(src));
                instruction.imm.u32  = (iter+3) - tokens.begin(); // index of label
                
                ad->instruction_list.push_back(instruction);
                iter += 4;
            }
        },
        {
            "bgezal",
            [&]() {
                assert((iter+1)->type == register_reference);
                assert((iter+2)->type == comma);
                assert((iter+3)->type == symbolic_constant);

                instruction.opcode = inst_bgezal;
                instruction.des.i32  = get_register_index((iter+1)->to_string(src));
                instruction.imm.u32  = (iter+3) - tokens.begin();
                
                ad->instruction_list.push_back(instruction);
                iter += 4;
            }
        },
        {
            "bgtz",
            [&]() {
                assert((iter+1)->type == register_reference);
                assert((iter+2)->type == comma);
                assert((iter+3)->type == symbolic_constant);

                instruction.opcode = inst_bgtz;
                instruction.des.i32  = get_register_index((iter+1)->to_string(src));
                instruction.imm.u32  = (iter+3) - tokens.begin();
                
                ad->instruction_list.push_back(instruction);
                iter += 4;
            }
        },
        {
            "blez",
            [&]() {
                assert((iter+1)->type == register_reference);
                assert((iter+2)->type == comma);
                assert((iter+3)->type == symbolic_constant);

                instruction.opcode = inst_blez;
                instruction.des.i32  = get_register_index((iter+1)->to_string(src));
                instruction.imm.u32  = (iter+3) - tokens.begin();
                
                ad->instruction_list.push_back(instruction);
                iter += 4;
            }
        },
        {
            "bltz",
            [&]() {
                assert((iter+1)->type == register_reference);
                assert((iter+2)->type == comma);
                assert((iter+3)->type == symbolic_constant);

                instruction.opcode = inst_bltz;
                instruction.des.i32  = get_register_index((iter+1)->to_string(src));
                instruction.imm.u32  = (iter+3) - tokens.begin();
                
                ad->instruction_list.push_back(instruction);
                iter += 4;
            }
        },
        {
            "bltzal",
            [&]() {
                assert((iter+1)->type == register_reference);
                assert((iter+2)->type == comma);
                assert((iter+3)->type == symbolic_constant);

                instruction.opcode = inst_bltzal;
                instruction.des.i32  = get_register_index((iter+1)->to_string(src));
                instruction.imm.u32  = (iter+3) - tokens.begin();
                
                ad->instruction_list.push_back(instruction);
                iter += 4;
            }
        },
        {
            "bne",
            [&]() {
                assert((iter+1)->type == register_reference);
                assert((iter+2)->type == comma);
                assert((iter+3)->type == register_reference);
                assert((iter+4)->type == comma);
                assert((iter+5)->type == symbolic_constant);

                instruction.opcode   = inst_bne;
                instruction.src1.i32 = get_register_index((iter+1)->to_string(src));
                instruction.src2.i32 = get_register_index((iter+3)->to_string(src));
                instruction.imm.u32  = (iter+5) - tokens.begin(); // get index to token

                ad->instruction_list.push_back(instruction);
                iter += 6;
            }
        },
        {
            "div",
            [&]() {
                assert((iter+1)->type == register_reference);
                assert((iter+2)->type == comma);
                assert((iter+3)->type == register_reference);

                instruction.opcode = inst_div;
                instruction.src1.i32 = get_register_index((iter+1)->to_string(src));
                instruction.src2.i32 = get_register_index((iter+3)->to_string(src));

                ad->instruction_list.push_back(instruction);
                iter += 4;
            }
        },
        {
            "divu",
            [&]() {
                assert((iter+1)->type == register_reference);
                assert((iter+2)->type == comma);
                assert((iter+3)->type == register_reference);

                instruction.opcode = inst_divu;
                instruction.src1.i32 = get_register_index((iter+1)->to_string(src));
                instruction.src2.i32 = get_register_index((iter+3)->to_string(src));

                ad->instruction_list.push_back(instruction);
                iter += 4;
            }
        },
        {
            "j",
            [&]() {
                assert((iter+1)->type == symbolic_constant);

                instruction.opcode = inst_j;
                instruction.imm.u32 = (iter+1) - tokens.begin();

                ad->instruction_list.push_back(instruction);
                iter += 2;
            }
        },
        {
            "jal",
            [&]() {
                assert((iter+1)->type == symbolic_constant);

                instruction.opcode = inst_jal;
                instruction.imm.u32 = (iter+1) - tokens.begin();

                ad->instruction_list.push_back(instruction);
                iter += 2;
            }
        },
        {
            "jr",
            [&]() {
                assert((iter+1)->type == register_reference);

                instruction.opcode = inst_jr;
                instruction.imm.u32 = get_register_index((iter+1)->to_string(src));

                ad->instruction_list.push_back(instruction);
                iter += 2;
            }
        },
        {
            "lb",
            [&]() {
                assert((iter+1)->type == register_reference);
                assert((iter+2)->type == comma);
                assert((iter+3)->type == number_literal);
                assert((iter+4)->type == open_paren);
                assert((iter+5)->type == register_reference);
                assert((iter+6)->type == close_paren);

                instruction.opcode = inst_lb;
                instruction.des.i32 = get_register_index((iter+1)->to_string(src));
                
                // get the number literal
                auto en = evaluate_number((iter+3)->to_string(src));
                assert(en.type == NC_int);
                instruction.imm.i32 = en.i32;

                ad->instruction_list.push_back(instruction);
                iter += 7;
            }
        },
        {
            "lui",
            [&]() {
                assert((iter+1)->type == register_reference);
                assert((iter+2)->type == comma);
                assert((iter+3)->type == number_literal || (iter+3)->type == symbolic_constant);

                instruction.opcode = inst_lui;
                instruction.des.i32 = get_register_index((iter+1)->to_string(src));

                if((iter+3)->type == number_literal) {
                    auto en = evaluate_number((iter+3)->to_string(src));
                    assert(en.type == NC_int);
                    instruction.imm.i32 = en.i32;
                }
                else {
                    auto it = ad->constant_lookup.find((iter+3)->to_string(src));
                    assert(it != ad->constant_lookup.end());
                    auto en = evaluate_number(it->second.to_string(src));
                    assert(en.type == NC_int);
                    instruction.imm.i32 = en.i32;
                }

                ad->instruction_list.push_back(instruction);
                iter += 4;
            }
        },
        {
            "lw",
            [&]() {
                assert((iter+1)->type == register_reference);
                assert((iter+2)->type == comma);
                assert((iter+3)->type == number_literal || (iter+3)->type == symbolic_constant);
                assert((iter+4)->type == open_paren);
                assert((iter+5)->type == register_reference);
                assert((iter+6)->type == close_paren);

                instruction.opcode = inst_lw;
                instruction.des.i32 = get_register_index((iter+1)->to_string(src));
                instruction.src1.i32 = get_register_index((iter+5)->to_string(src));

                if((iter+3)->type == number_literal) {
                    auto en = evaluate_number((iter+3)->to_string(src));
                    assert(en.type == NC_int);
                    instruction.imm.i32 = en.i32;
                }
                else {
                    auto it = ad->constant_lookup.find((iter+3)->to_string(src));
                    assert(it != ad->constant_lookup.end());
                    auto en = evaluate_number(it->second.to_string(src));
                    assert(en.type == NC_int);
                    instruction.imm.i32 = en.i32;
                }


                ad->instruction_list.push_back(instruction);
                iter += 7;
            }
        },
        {
            "mfhi",
            [&]() {
                assert((iter+1)->type == register_reference);

                instruction.opcode = inst_mfhi;
                instruction.des.i32 = get_register_index((iter+1)->to_string(src));

                ad->instruction_list.push_back(instruction);
                iter += 2;
            }
        },
        {
            "mflo",
            [&]() {
                assert((iter+1)->type == register_reference);

                instruction.opcode = inst_mflo;
                instruction.des.i32 = get_register_index((iter+1)->to_string(src));

                ad->instruction_list.push_back(instruction);
                iter += 2;
            }
        },
        {
            "mult",
            [&]() {
                assert((iter+1)->type == register_reference);
                assert((iter+2)->type == comma);
                assert((iter+3)->type == register_reference);

                instruction.opcode = inst_mult;
                instruction.src1.i32 = get_register_index((iter+1)->to_string(src));
                instruction.src2.i32 = get_register_index((iter+3)->to_string(src));

                ad->instruction_list.push_back(instruction);
                iter += 4;
            }
        },
        {
            "multu",
            [&]() {
                assert((iter+1)->type == register_reference);
                assert((iter+2)->type == comma);
                assert((iter+3)->type == register_reference);

                instruction.opcode = inst_multu;
                instruction.src1.i32 = get_register_index((iter+1)->to_string(src));
                instruction.src2.i32 = get_register_index((iter+3)->to_string(src));

                ad->instruction_list.push_back(instruction);
                iter += 4;
            }
        },
        {
            "noop",
            [&]() {
                instruction.opcode = inst_noop;

                ad->instruction_list.push_back(instruction);
                iter += 1;
            }
        },
        {
            "nop",
            [&]() {
                instruction.opcode = inst_noop;

                ad->instruction_list.push_back(instruction);
                iter += 1;
            }
        },
        {
            "or",
            [&]() {
                assert((iter+1)->type == register_reference);
                assert((iter+2)->type == comma);
                assert((iter+3)->type == register_reference);
                assert((iter+4)->type == comma);
                assert((iter+5)->type == register_reference);

                instruction.opcode   = inst_or;
                instruction.des.i32  = get_register_index((iter+1)->to_string(src));
                instruction.src1.i32 = get_register_index((iter+3)->to_string(src));
                instruction.src2.i32 = get_register_index((iter+5)->to_string(src));

                ad->instruction_list.push_back(instruction);
                iter += 6;

            }
        },
        {
            "ori",
            [&]() {
                assert((iter+1)->type == register_reference);
                assert((iter+2)->type == comma);
                assert((iter+3)->type == register_reference);
                assert((iter+4)->type == comma);
                assert((iter+5)->type == number_literal || (iter+5)->type == symbolic_constant);

                instruction.opcode   = inst_ori;
                instruction.des.i32  = get_register_index((iter+1)->to_string(src));
                instruction.src1.i32 = get_register_index((iter+3)->to_string(src));

                if((iter+5)->type == number_literal) {
                    auto en = evaluate_number((iter+5)->to_string(src));
                    assert(en.type == NC_int);
                    instruction.imm.i32 = en.i32;
                }
                else {
                    auto it = ad->constant_lookup.find((iter+5)->to_string(src));
                    assert(it != ad->constant_lookup.end());
                    auto en = evaluate_number(it->second.to_string(src));
                    assert(en.type == NC_int);
                    instruction.imm.i32 = en.i32;
                }

                ad->instruction_list.push_back(instruction);
                iter += 6;
            }
        },
        {
            "sb",
            [&]() {
                assert((iter+1)->type == register_reference);
                assert((iter+2)->type == comma);
                assert((iter+3)->type == number_literal || (iter+3)->type == symbolic_constant);
                assert((iter+4)->type == open_paren);
                assert((iter+5)->type == register_reference);
                assert((iter+6)->type == close_paren);

                instruction.opcode = inst_sb;
                instruction.des.i32 = get_register_index((iter+1)->to_string(src));
                instruction.src1.i32 = get_register_index((iter+5)->to_string(src));

                if((iter+3)->type == number_literal) {
                    auto en = evaluate_number((iter+3)->to_string(src));
                    assert(en.type == NC_int);
                    instruction.imm.i32 = en.i32;
                }
                else {
                    auto it = ad->constant_lookup.find((iter+3)->to_string(src));
                    assert(it != ad->constant_lookup.end());
                    auto en = evaluate_number(it->second.to_string(src));
                    assert(en.type == NC_int);
                    instruction.imm.i32 = en.i32;
                }

                ad->instruction_list.push_back(instruction);
                iter += 7;
            }
        },
        {
            "sll",
            [&]() {
                assert((iter+1)->type == register_reference);
                assert((iter+2)->type == comma);
                assert((iter+3)->type == register_reference);
                assert((iter+4)->type == comma);
                assert((iter+5)->type == number_literal || (iter+5)->type == symbolic_constant);

                instruction.opcode   = inst_sll;
                instruction.des.i32  = get_register_index((iter+1)->to_string(src));
                instruction.src1.i32 = get_register_index((iter+3)->to_string(src));
                
                if((iter+5)->type == number_literal) {
                    auto en = evaluate_number((iter+5)->to_string(src));
                    assert(en.type == NC_int);
                    instruction.imm.i32 = en.i32;
                }
                else {
                    auto it = ad->constant_lookup.find((iter+5)->to_string(src));
                    assert(it != ad->constant_lookup.end());
                    auto en = evaluate_number(it->second.to_string(src));
                    assert(en.type == NC_int);
                    instruction.imm.i32 = en.i32;
                }

                ad->instruction_list.push_back(instruction);
                iter += 6;
            }
        },
        {
            "sllv",
            [&]() {
                assert((iter+1)->type == register_reference);
                assert((iter+2)->type == comma);
                assert((iter+3)->type == register_reference);
                assert((iter+4)->type == comma);
                assert((iter+5)->type == register_reference);

                instruction.opcode = inst_sllv;
                instruction.des.i32  = get_register_index((iter+1)->to_string(src));
                instruction.src1.i32 = get_register_index((iter+3)->to_string(src));
                instruction.src2.i32 = get_register_index((iter+5)->to_string(src));

                ad->instruction_list.push_back(instruction);
                iter += 6;
            }
        },
        {
            "slt",
            [&]() {
                assert((iter+1)->type == register_reference);
                assert((iter+2)->type == comma);
                assert((iter+3)->type == register_reference);
                assert((iter+4)->type == comma);
                assert((iter+5)->type == register_reference);

                instruction.opcode = inst_slt;
                instruction.des.i32  = get_register_index((iter+1)->to_string(src));
                instruction.src1.i32 = get_register_index((iter+3)->to_string(src));
                instruction.src2.i32 = get_register_index((iter+5)->to_string(src));

                ad->instruction_list.push_back(instruction);
                iter += 6;
            }
        },
        {
            "slti",
            [&]() {
                assert((iter+1)->type == register_reference);
                assert((iter+2)->type == comma);
                assert((iter+3)->type == register_reference);
                assert((iter+4)->type == comma);
                assert((iter+5)->type == number_literal || (iter+5)->type == symbolic_constant);

                instruction.opcode = inst_slti;
                instruction.des.i32 = get_register_index((iter+1)->to_string(src));
                instruction.src1.i32 = get_register_index((iter+3)->to_string(src));

                if((iter+5)->type == number_literal) {
                    auto en = evaluate_number((iter+5)->to_string(src));
                    assert(en.type == NC_int);
                    instruction.imm.i32 = en.i32;
                }
                else {
                    auto it = ad->constant_lookup.find((iter+5)->to_string(src));
                    assert(it != ad->constant_lookup.end());
                    auto en = evaluate_number(it->second.to_string(src));
                    assert(en.type == NC_int);
                    instruction.imm.i32 = en.i32;
                }

                ad->instruction_list.push_back(instruction);
                iter += 6;

            }
        },
        {
            "sltiu",
            [&]() {
                assert((iter+1)->type == register_reference);
                assert((iter+2)->type == comma);
                assert((iter+3)->type == register_reference);
                assert((iter+4)->type == comma);
                assert((iter+5)->type == number_literal || (iter+5)->type == symbolic_constant);

                instruction.opcode   = inst_sltiu;
                instruction.des.i32  = get_register_index((iter+1)->to_string(src));
                instruction.src1.i32 = get_register_index((iter+3)->to_string(src));

                if((iter+5)->type == number_literal) {
                    auto en = evaluate_number((iter+5)->to_string(src));
                    assert(en.type == NC_int);
                    instruction.imm.i32 = en.i32;
                }
                else {
                    auto it = ad->constant_lookup.find((iter+5)->to_string(src));
                    assert(it != ad->constant_lookup.end());
                    auto en = evaluate_number(it->second.to_string(src));
                    assert(en.type == NC_int);
                    instruction.imm.i32 = en.i32;
                }

                ad->instruction_list.push_back(instruction);
                iter += 6;

            }
        },
        {
            "sltu",
            [&]() {
                assert((iter+1)->type == register_reference);
                assert((iter+2)->type == comma);
                assert((iter+3)->type == register_reference);
                assert((iter+4)->type == comma);
                assert((iter+5)->type == register_reference);

                instruction.opcode = inst_sltu;
                instruction.des.i32  = get_register_index((iter+1)->to_string(src));
                instruction.src1.i32 = get_register_index((iter+3)->to_string(src));
                instruction.src2.i32 = get_register_index((iter+5)->to_string(src));

                ad->instruction_list.push_back(instruction);
                iter += 6;

            }
        },
        {
            "sra",
            [&]() {
                
            }
        }
    };

    auto institer = opcodemap.find(opcode);
    assert(institer != opcodemap.end()); // does no good if instruction is not implemented
    institer->second(); // call the instruction assembler callback for this opcode
    return iter;
}
