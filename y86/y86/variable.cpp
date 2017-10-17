#include"head.h"
#include<map>

//store register
int REG[9];
//store instruction
map<int, int> MEM;

//flag
bool ZF = false;
bool SF = false;
bool OF = false;

//state
int imem_error, instr_valid;

//FETCH
int f_icode, f_ifun, f_rA, f_rB, f_valC, f_valP;
int F_predPC, f_predPC, f_PC, f_stat;

//DECOUDE
int D_icode, D_ifun, D_rA, D_rB, D_valC, D_valP, D_stat;
int d_valA, d_valB, d_srcA, d_srcB, d_dstE, d_dstM;

//EXECUTE
int E_icode, E_ifun, E_valC, E_valA, E_valB, E_stat;
int  E_dstE = rnone;
int E_dstM = rnone;
int E_srcA = rnone;
int	E_srcB = rnone;
int e_valE;
int ALUA, ALUB;
int ALU_fun;
bool NEED_ALU = true;
bool e_Cnd;

//MEMORY
int M_icode, M_Cnd, M_valE, M_valA, M_stat;
int M_dstE = rnone;
int M_dstM = rnone;
int m_valM, m_Addr;
bool m_read, m_write, m_stat;

//WRITE_BACK
int	W_icode, W_valE, W_valM, W_stat;
int W_dstE = rnone;
int W_dstM = rnone;

//CONTROL
bool D_bubble, E_bubble;
bool F_stall, D_stall;

//HALT
bool halt_flag = false;
bool halt_state = false;

