
// tokens

#define TOKEN_COUNT 7

#define TOKEN_INSTRUCTION 0 // instruction
#define TOKEN_REGISTER    1 // register

#define TOKEN_RESERVED    2 // reserved
#define TOKEN_RES_POS     3 // reserved position 

#define TOKEN_ADDRESS     4 // address
#define TOKEN_BYTE_ADDR   5 // byte address

#define TOKEN_NUMBER      6 // number
#define TOKEN_BYTE        7 // byte

// instructions

#define INSTRUCTION_COUNT 19

#define INSTRUCTION_CLA 0  // calculate address
#define INSTRUCTION_MOV 1  // move

#define INSTRUCTION_CND 2  // condition
#define INSTRUCTION_CMP 3  // compare

#define INSTRUCTION_JMP 4  // jump
#define INSTRUCTION_CAL 5  // call
#define INSTRUCTION_RET 6  // return

#define INSTRUCTION_PSH 7  // push
#define INSTRUCTION_POP 8  // pop

#define INSTRUCTION_ADD 9  // add
#define INSTRUCTION_SUB 10 // subtract
#define INSTRUCTION_MUL 11 // multiply
#define INSTRUCTION_DIV 12 // divide

#define INSTRUCTION_AND 13 // and
#define INSTRUCTION_OR  14 // or
#define INSTRUCTION_XOR 15 // exclusive or
#define INSTRUCTION_NOT 16 // not

#define INSTRUCTION_SHL 17 // left shift
#define INSTRUCTION_SHR 18 // right shift

// registers

#define REGISTER_COUNT 16

#define REGISTER_IP 0  // instruction pointer
#define REGISTER_SP 1  // stack pointer
#define REGISTER_BP 2  // base pointer

#define REGISTER_AD 3  // address

#define REGISTER_G0 4  // general 0
#define REGISTER_G1 5  // general 1
#define REGISTER_G2 6  // general 2
#define REGISTER_G3 7  // general 3

#define REGISTER_A0 8  // argument 0
#define REGISTER_A1 9  // argument 1
#define REGISTER_A2 10 // argument 2
#define REGISTER_A3 11 // argument 3

#define REGISTER_SF 12 // sign flag
#define REGISTER_ZF 13 // zero flag
#define REGISTER_OF 14 // overflow flag
#define REGISTER_CF 15 // carry flag

// conditions

#define CONDITION_COUNT 0 /// TODO
