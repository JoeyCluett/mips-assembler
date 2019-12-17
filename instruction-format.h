#pragma once

#include <iostream>
#include <vector>
#include <map>

const int inst_add     = 0;  // add (with overflow)
const int inst_addi    = 1;  // add immediate (with overflow)
const int inst_addiu   = 2;  // add immediate unsigned (no overflow)
const int inst_addu    = 3;  // add unsigned (no overflow)
const int inst_and     = 4;  // bitwise AND
const int inst_andi    = 5;  // bitwise AND immediate
const int inst_beq     = 6;  // branch on equal
const int inst_bgez    = 7;  // branch on greater than or equal to zero
const int inst_bgezal  = 8;  // branch on greater than or equal to zero and link
const int inst_bgtz    = 9;  // branch on greater than zero
const int inst_blez    = 10; // branch on less than or equal to zero
const int inst_bltz    = 11; // branch on less than zero
const int inst_bltzal  = 12; // branch on less than zero and link
const int inst_bne     = 13; // branch on not equal
const int inst_div     = 14; // divide
const int inst_divu    = 15; // divide unsigned
const int inst_j       = 16; // jump
const int inst_jal     = 17; // jump and link
const int inst_jr      = 18; // jump register
const int inst_lb      = 19; // load byte
const int inst_lui     = 20; // load upper immediate
const int inst_lw      = 21; // load word
const int inst_mfhi    = 22; // move from HI register
const int inst_mflo    = 23; // move from LO register
const int inst_mult    = 24; // multiply
const int inst_multu   = 25; // multiply unsigned
const int inst_noop    = 26; // no operation
const int inst_or      = 27; // bitwise OR
const int inst_ori     = 28; // bitwise OR immediate
const int inst_sb      = 29; // store byte
const int inst_sll     = 30; // shift left logical
const int inst_sllv    = 31; // shift left logical variable
const int inst_slt     = 32; // set on less than (signed)
const int inst_slti    = 33; // set on less than immediate (signed)
const int inst_sltiu   = 34; // set on less than immediate unsigned
const int inst_sltu    = 35; // set on less than unsigned
const int inst_sra     = 36; // shift right arithmetic
const int inst_srl     = 37; // shift right logical
const int inst_srlv    = 38; // shift right logical variable
const int inst_sub     = 39; // subtract
const int inst_subu    = 40; // subtract unsigned
const int inst_sw      = 41; // store word
const int inst_syscall = 42; // generate sw interrupt
const int inst_xor     = 43; // bitwise XOR
const int inst_xori    = 44; // bitwise XOR immediate

// psuedo instructions
const int inst_abs  = 45; // absolute value
const int inst_blt  = 46; // branch on less than
const int inst_bgt  = 47; // branch on greater than
const int inst_ble  = 48; // branch on less than or equal to
const int inst_neg  = 49;
const int inst_not  = 50;
const int inst_bge  = 51; // branch on greater than or equal to
const int inst_li   = 52; // load immediate
const int inst_la   = 53; // load address
const int inst_move = 54; // move register to register
const int inst_sge  = 55; // set on greater than or equal to
const int inst_sgt  = 56; // set on greater than

// purely symbolic representation of instruction. does not store pseudo instructions
struct Instruction {

    int opcode;

    union field_t {
        int32_t  i32;
        uint32_t u32;
        float f32;
    };

    field_t des;
    field_t src1;
    field_t reg2;
    field_t src2;
    field_t imm; 
    field_t addr;

};
