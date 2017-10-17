#include "head.h"

void initial()
{
	//bubble&stall
	D_bubble = false;
	E_bubble = false;
	F_stall = false;
	D_stall = false;
	//halt_stat&halt_flag
	halt_flag = false;
	halt_state = false;
	//CC
	ZF = false;
	SF = false;
	OF = false;
	e_Cnd = true;
	//icode&ifun
	f_icode = nop;
	f_ifun = 0;
	D_icode = nop;
	D_ifun = 0;
	E_icode = nop;
	E_ifun = 0;
	M_icode = nop;
	W_icode = nop;
	//reg file
	REG[reax] = 0;
	REG[rebx] = 0;
	REG[recx] = 0;
	REG[redx] = 0;
	REG[resi] = 0;
	REG[rebp] = 0;
	REG[resp] = 0;
	REG[redi] = 0;
	//fetch
	f_rA = rnone;
	f_rB = rnone;
	f_valC = 0;
	f_valP = 0;
	f_predPC = 0;
	f_PC = 0;
	//decode
	D_rA = rnone;
	D_rB = rnone;
	D_valC = 0;
	D_valP = 0;
	d_valA = 0;
	d_valB = 0;
	d_srcA = rnone;
	d_srcB = rnone;
	d_dstE = rnone;
	d_dstM = rnone;
	//execute
	E_valC = 0;
	E_valA = 0;
	E_valB = 0;
	E_dstE = rnone;
	E_dstM = rnone;
	E_srcA = rnone;
	E_srcB = rnone;
	e_valE = 0;
	ALUA = 0;
	ALUB = 0;
	ALU_fun = 0;
	NEED_ALU = true;
	e_Cnd = true;
	//memory
	M_Cnd = true;
	M_valE = 0;
	M_valA = 0;
	M_dstE = rnone;
	M_dstM = rnone;
	m_valM = 0;
	m_Addr = 0;
	m_read = false;
	m_write = false;
	//write_back
	W_valE = 0;
	W_valM = 0;
	W_dstE = rnone;
	W_dstM = rnone;

}
