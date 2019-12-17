#pragma once

#include <iostream>
#include <unordered_set>

const std::unordered_set<std::string> instruction_list = {
    "add",     // add (with overflow)
    "addi",    // add immediate (with overflow)
    "addiu",   // add immediate unsigned (no overflow)
    "addu",    // add unsigned (no overflow)
    "and",     // bitwise AND
    "andi",    // bitwise AND immediate
    "beq",     // branch on equal
    "bgez",    // branch on greater than or equal to zero
    "bgezal",  // branch on greater than or equal to zero and link
    "bgtz",    // branch on greater than zero
    "blez",    // branch on less than or equal to zero
    "bltz",    // branch on less than zero
    "bltzal",  // branch on less than zero and link
    "bne",     // branch on not equal
    "div",     // divide
    "divu",    // divide unsigned
    "j",       // jump
    "jal",     // jump and link
    "jr",      // jump register
    "lb",      // load byte
    "lui",     // load upper immediate
    "lw",      // load word
    "mfhi",    // move from HI register
    "mflo",    // move from LO register
    "mult",    // multiply
    "multu",   // multiply unsigned
    "noop",    // no operation
    "or",      // bitwise OR
    "ori",     // bitwise OR immediate
    "sb",      // store byte
    "sll",     // shift left logical
    "sllv",    // shift left logical variable
    "slt",     // set on less than (signed)
    "slti",    // set on less than immediate (signed)
    "sltiu",   // set on less than immediate unsigned
    "sltu",    // set on less than unsigned
    "sra",     // shift right arithmetic
    "srl",     // shift right logical
    "srlv",    // shift right logical variable
    "sub",     // subtract
    "subu",    // subtract unsigned
    "sw",      // store word
    "syscall", // generate sw interrupt
    "xor",     // bitwise XOR
    "xori",    // bitwise XOR immediate

    // psuedo instructions
    "abs",     // absolute value
    "blt",     // branch on less than
    "bgt",     // branch on greater than
    "ble",     // branch on less than or equal to
    "neg",
    "not",
    "bge",     // branch on greater than or equal to
    "li",      // load immediate
    "la",      // load address
    "move",    // move register to register
    "sge",     // set on greater than or equal to
    "sgt",     // set on greater than

};
