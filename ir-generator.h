#pragma once


#include <fstream>
#include <iostream>
#include "assembler-data-tables.h"

void ir_generator(AssemblerData* ad, std::string filename, size_t text_start_address, size_t data_start_address) {

    std::ofstream of(filename);

    for(auto& p : ad->instruction_list) {
        switch(p.opcode) {

            case inst_add:
                of 
                    << p.opcode << ' ' << p.des.i32 << ' '
                    << p.src1.i32 << ' ' << p.src2.i32 << std::endl;
                break;
            case inst_addi:
                of
                    << p.opcode << ' ' << p.des.i32 << ' '
                    << p.src1.i32 << ' ' << p.imm.i32 << '\n';
                break;
            case inst_addiu:
            case inst_addu:
            case inst_and:
            case inst_andi:
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
            case inst_multu:
            case inst_noop:
            case inst_or:
            case inst_ori:
            case inst_sb:
            case inst_sll:
            case inst_sllv:
            case inst_slt:
            case inst_slti:
            case inst_sltiu:
            case inst_sltu:
            case inst_sra:
            case inst_srl:
            case inst_srlv:
            case inst_sub:
            case inst_subu:
            case inst_sw:
            case inst_syscall:
            case inst_xor:
            case inst_xori:
            default:
                break;

        }
    }

}
