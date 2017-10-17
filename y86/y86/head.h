#pragma once
#ifndef HEAD_H
#define HEAD_H

#include<fstream>
#include<vector>
#include<map>
using namespace std;


//icode
#define	halt 0
#define nop 1
#define	rrmovl 2
#define	irmovl 3
#define	rmmovl 4
#define	mrmovl 5
#define	opl 6
#define	jxx 7
#define	call 8
#define	ret 9
#define pushl 0xA
#define popl 0xB

//ifun
#define addl 0x0
#define	subl 0x1
#define	andl 0x2
#define	xorl 0x3
#define jmp 0x0
#define	jle 0x1
#define jl 0x2
#define je 0x3
#define	jne 0x4
#define jge 0x5
#define	jg 0x6

//reg
#define	reax 0
#define recx 1
#define	redx 2
#define	rebx 3
#define	resp 4
#define	rebp 5
#define	resi 6
#define	redi 7
#define	rnone 8

/*enum state
{
SADR=0,
SINS=1,
SHLT=2,
SAOK=3
};*/

//store register
extern int REG[9];
//store instruction
extern std::map<int, int> MEM;
//store state
extern int STATE[3];

//flag
extern bool ZF;
extern bool SF;
extern bool OF;

//state
extern int imem_error, instr_valid;

//FETCH
extern int f_icode, f_ifun, f_rA, f_rB, f_valC, f_valP;
extern int F_predPC, f_predPC, f_PC, f_stat;

//DECOUDE
extern int D_icode, D_ifun, D_rA, D_rB, D_valC, D_valP, D_stat;
extern int d_valA, d_valB, d_srcA, d_srcB, d_dstE, d_dstM;

//EXECUTE
extern int E_icode, E_ifun, E_valC, E_valA, E_valB, E_dstE, E_dstM, E_srcA, E_srcB, E_stat;
extern int e_valE;
extern int ALUA, ALUB;
extern int ALU_fun;
extern bool NEED_ALU;
extern bool e_Cnd;

//MEMORY
extern int M_icode, M_Cnd, M_valE, M_valA, M_dstE, M_dstM, M_stat;
extern int m_valM, m_Addr;
extern bool m_read, m_write, m_stat;

//WRITE_BACK
extern int	W_icode, W_valE, W_valM, W_dstE, W_dstM, W_stat;

//CONTROL
extern bool D_bubble, E_bubble;
extern bool F_stall, D_stall;

//HALT
extern bool halt_flag;
extern bool halt_state;


//readin.cpp
std::ifstream readin();
//y86.cpp
void fetch();
void decode();
void excute();
void memory();
void write_back();
void control();
void pass_parameter();
void clock_cycle();
void initial();
#endif