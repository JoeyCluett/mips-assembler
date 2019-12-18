#pragma once

#include <fstream>
#include <iostream>
#include "assembler-data-tables.h"

void ir_generator(AssemblerData* ad, std::string filename, size_t text_start_address, size_t data_start_address) {

    std::ofstream of(filename);

    of << "TEXT " << ad->instruction_list.size() << '\n';

    for(auto& p : ad->instruction_list) {
        switch(p.opcode) {

            case inst_add:
                of 
                    << p.opcode << ' ' << p.des.i32 << ' '
                    << p.src1.i32 << ' ' << p.src2.i32 << '\n';
                break;
            case inst_addi:
                of
                    << p.opcode << ' ' << p.des.i32 << ' '
                    << p.src1.i32 << ' ' << p.imm.i32 << '\n';
                break;
            case inst_addiu:
                of
                    << p.opcode << ' ' << p.des.i32 << ' '
                    << p.src1.i32 << ' ' << p.imm.u32 << '\n';
                break;
            case inst_addu:
                of 
                    << p.opcode << ' ' << p.des.i32 << ' '
                    << p.src1.i32 << ' ' << p.src2.i32 << '\n';
                break;
            case inst_and:
                of 
                    << p.opcode << ' ' << p.des.i32 << ' '
                    << p.src1.i32 << ' ' << p.src2.i32 << '\n';
                break;
            case inst_andi:
                of
                    << p.opcode << ' ' << p.des.i32 << ' '
                    << p.src1.i32 << ' ' << p.imm.i32 << '\n';
                break;
            case inst_beq:
            case inst_bgez:
            case inst_bgezal:
            case inst_bgtz:
            case inst_blez:
            case inst_bltz:
            case inst_bltzal:
            case inst_bne:
            case inst_div:
            case inst_divu:
            case inst_j:
            case inst_jal:
            case inst_jr:
            case inst_lb:
            case inst_lui:
            case inst_lw:
            case inst_mfhi:
            case inst_mflo:
            case inst_mult:
            case inst_multu: break;
            case inst_noop:
                of << p.opcode << '\n';
                break;
            case inst_or:
                of 
                    << p.opcode << ' ' << p.des.i32 << ' '
                    << p.src1.i32 << ' ' << p.src2.i32 << '\n';
                break;
            case inst_ori:
            case inst_sb:
            case inst_sll:
            case inst_sllv: break;
            case inst_slt:
                of 
                    << p.opcode << ' ' << p.des.i32 << ' '
                    << p.src1.i32 << ' ' << p.src2.i32 << '\n';
                break;
            case inst_slti:
            case inst_sltiu: break;
            case inst_sltu:
                of 
                    << p.opcode << ' ' << p.des.i32 << ' '
                    << p.src1.i32 << ' ' << p.src2.i32 << '\n';
                break;
            case inst_sra:
            case inst_srl:
            case inst_srlv: break;
            case inst_sub:
                of 
                    << p.opcode << ' ' << p.des.i32 << ' '
                    << p.src1.i32 << ' ' << p.src2.i32 << '\n';
                break;
            case inst_subu:
                of 
                    << p.opcode << ' ' << p.des.i32 << ' '
                    << p.src1.i32 << ' ' << p.src2.i32 << '\n';
                break;
            case inst_sw: break;
            case inst_syscall:
                of << p.opcode << '\n';
                break;
            case inst_xor:
                of 
                    << p.opcode << ' ' << p.des.i32 << ' '
                    << p.src1.i32 << ' ' << p.src2.i32 << '\n';
                break;
            case inst_xori:
            default:
                break;

        }
    }

    of << "DATA " << ad->data_vector.size() << '\n';

    for(char c : ad->data_vector) {
        auto clut = "0123456789ABCDEF";
        of << clut[(c >> 4) & 0xF] << clut[c & 0xF] << ' ';
    }

}
