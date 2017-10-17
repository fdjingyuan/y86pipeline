/*the main part:
run instructions and show the details
about register,condition code,memory and PC
*/
//试图用英文写注释……然后放弃了……
#include "head.h"
#include<fstream>
#include<iostream>
#include<cstdio>

using namespace std;

int INS[6];

//所有的指令相关数据都已经存入MEM中，每一个byte（两个十六进制）存在一起，读取也是一个byte为单位读取

//stage fetch
//先读入六个bytes进ins，然后对icode做判断继续对rA、rB、valC、valP做取值操作
//然后实现prePC功能块

void fetch()
{
	if (halt_flag)	return;

	//read 6 bytes
	for (int i = 0; i < 6; i++)
		INS[i] = MEM[f_PC + i];
	f_icode = (INS[0] & 0Xf0) >> 4;//取出两个十六进制的高位是icode
	f_ifun = INS[0] & 0x0f;
	f_rA = rnone;
	f_rB = rnone;
	f_valC = 0;

	//f_valP,f_valC,f_rA,f_rB			
	switch (f_icode) {
	case halt:
	case nop:
	case ret:
		f_valP = f_PC + 1;
		break;
	case pushl:
	case popl:
	case opl:
	case rrmovl:
		f_valP = f_PC + 2;
		f_rA = (INS[1] & 0xf0) >> 4;//高位
		f_rB = INS[1] & 0x0f;//低位
		break;
	case jxx:
	case call:
		f_valP = f_PC + 5;
		f_valC = INS[1] + (INS[2] << 8) + (INS[3] << 16) + (INS[4] << 24);//注意是小端法，如：67 45 23 01
		break;
	default:
		f_valP = f_PC + 6;
		f_valC = INS[2] + (INS[3] << 8) + (INS[4] << 16) + (INS[5] << 24);
		f_rA = (INS[1] & 0xf0) >> 4;
		f_rB = INS[1] & 0x0f;
		break;
	}

	//f_predPC  预测PC
	if (f_icode == jxx || f_icode == call)
		f_predPC = f_valC;//函数调用或跳转时，选择valC，否则valP
	else
		f_predPC = f_valP;

	//f_PC  PC选择逻辑从三个程序计数器源中进行选择，当预测错误的分支进入访存阶段时
	if (E_icode == jxx && !e_Cnd)
		f_PC = E_valA;//下一条指令的地址
	else if (W_icode == ret)
		f_PC = W_valM;//返回地址
	else
		f_PC = f_predPC; //其他情况使用存放在寄存器F中的PC预测值

}

//stage decode
void decode()
{
	if (D_icode == halt)	halt_flag = true;

	//关于D_icode，D_ifun的传值放在clock_cycle里进行
	//初始化
	d_srcA = rnone;
	d_srcB = rnone;
	d_dstE = rnone;
	d_dstM = rnone;
	d_valA = 0;
	d_valB = 0;

	//d_dstE
	if (D_icode == irmovl || D_icode == rrmovl || D_icode == opl)
		d_dstE = D_rB;
	else if (D_icode == pushl || D_icode == popl || D_icode == call || D_icode == ret)
		d_dstE = resp;

	//d_dstM
	if (D_icode == mrmovl || D_icode == popl)
		d_dstM = D_rA;

	//d_srcA
	if (D_icode == rrmovl || D_icode == rmmovl || D_icode == pushl || D_icode == opl)
		d_srcA = D_rA;
	else if (D_icode == popl || D_icode == ret)
		d_srcA = resp;

	//d_srcB
	if (D_icode == opl || D_icode == mrmovl || D_icode == rmmovl)
		d_srcB = D_rB;
	else if (D_icode == pushl || D_icode == popl || D_icode == ret || D_icode == call)
		d_srcB = resp;

	//Sel+FwdA
	if (D_icode == call || D_icode == jxx)
		d_valA = D_valP;
	else if (d_srcA == E_dstE && d_srcA != rnone)//ALU输出
		d_valA = e_valE;
	else if (d_srcA == M_dstM && d_srcA != rnone)//存储器输出
		d_valA = m_valM;
	else if (d_srcA == M_dstE && d_srcA != rnone)//访存阶段对端口E未进行的写
		d_valA = M_valE;
	else if (d_srcA == W_dstM && d_srcA != rnone)//写回阶段对端口M未进行的写
		d_valA = W_valM;
	else if (d_srcA == W_dstE && d_srcA != rnone)//写回阶段对端口E未进行的写
		d_valA = W_valE;
	else if (d_srcA != rnone)
		d_valA = REG[d_srcA];  //排除一下rnone的情况，就可以把reg file的大小减一个寄存器的大小了	
								//判断是否与之后阶段也可以排除一下none的情况
	//FwdB
	if (d_srcB == E_dstE && d_srcB != rnone)
		d_valB = e_valE;
	else if (d_srcB == M_dstM && d_srcB != rnone)
		d_valB = m_valM;
	else if (d_srcB == M_dstE && d_srcB != rnone)
		d_valB = M_valE;
	else if (d_srcB == W_dstM && d_srcB != rnone)
		d_valB = W_valM;
	else if (d_srcB == W_dstE && d_srcB != rnone)
		d_valB = W_valE;
	else if (d_srcB != rnone)
		d_valB = REG[d_srcB];			  //同上
}

//stage excute
void excute()
{
	//initial
	NEED_ALU = true;
	e_Cnd = true;

	//ALUA
	switch (E_icode) {
	case rrmovl:
	case opl:
		ALUA = E_valA;
		break;
	case irmovl:
	case rmmovl:
	case mrmovl:
		ALUA = E_valC;
		break;
	case call:
	case pushl:
		ALUA = -4;
		break;
	case ret:
	case popl:
		ALUA = 4;
		break;
	default:
		NEED_ALU = false;
		break;
	}
	//ALUB
	switch (E_icode) {
	case rmmovl:
	case mrmovl:
	case opl:
	case call:
	case pushl:
	case ret:
	case popl:
		ALUB = E_valB;
		break;
	case rrmovl:
	case irmovl:
		ALUB = 0;
		break;
	default:
		NEED_ALU = false;
	}
	//ALU
	if (NEED_ALU)
	{
		ALU_fun = E_icode == opl ? E_ifun : addl;
		switch (ALU_fun) {
		case addl:
			e_valE = ALUA + ALUB;
			ZF = (e_valE == 0);
			OF = (ALUA > 0 && ALUB > 0 && e_valE < 0) || (ALUA < 0 && ALUB < 0 && e_valE > 0);
			SF = (e_valE < 0);
			break;
		case subl:
			e_valE = ALUB - ALUA;
			ZF = (e_valE == 0);
			OF = (ALUA > 0 && ALUB > 0 && e_valE < 0) || (ALUA < 0 && ALUB < 0 && e_valE > 0);
			SF = (e_valE < 0);
			break;
		case andl:
			e_valE = ALUA & ALUB;
			ZF = (e_valE == 0);
			OF = (ALUA > 0 && ALUB > 0 && e_valE < 0) || (ALUA < 0 && ALUB < 0 && e_valE > 0);
			SF = (e_valE < 0);
			break;
		case xorl:
			e_valE = ALUA ^ ALUB;
			ZF = (e_valE == 0);
			OF = (ALUA > 0 && ALUB > 0 && e_valE < 0) || (ALUA < 0 && ALUB < 0 && e_valE > 0);
			SF = (e_valE < 0);
			break;
		}
	}
	//CC
	if (E_icode == jxx)
	{
		switch (E_ifun) {
		case jmp:
			e_Cnd = true;
			break;
		case jle:
			e_Cnd = ((SF^OF) | ZF);
			break;
		case jl:
			e_Cnd = SF^OF;
			break;
		case je:
			e_Cnd = ZF;
			break;
		case jne:
			e_Cnd = !ZF;
			break;
		case jge:
			e_Cnd = !(SF^OF);
			break;
		case jg:
			e_Cnd = (!(SF^OF)) | ZF;
			break;
		}
	}

}

//stage memory
void memory()
{
	//读写控制器 
	m_write = false;
	m_read = false;
	//m_write
	if (M_icode == call || M_icode == pushl || M_icode == rmmovl)
		m_write = true;
	else m_write = false;
	//m_read
	if (M_icode == popl || M_icode == ret || M_icode == mrmovl)
		m_read = true;
	else m_read = false;

	//m_Addr
	if (M_icode == call || M_icode == pushl || M_icode == rmmovl || M_icode == mrmovl)
		m_Addr = M_valE;
	else if (M_icode == popl || M_icode == ret)
		m_Addr = M_valA;

	m_valM = 0;
	if (m_read)
		m_valM = MEM[m_Addr] + (MEM[m_Addr + 1] << 8) + (MEM[m_Addr + 2] << 16) + (MEM[m_Addr + 3] << 24);
	if (m_write) {
		MEM[m_Addr + 3] = (M_valA >> 24) & 0xff;//小端法存储
		MEM[m_Addr + 2] = (M_valA >> 16) & 0xff;
		MEM[m_Addr + 1] = (M_valA >> 8) & 0xff;
		MEM[m_Addr] = M_valA & 0xff;
	}
}

//stage write back
void write_back()
{
	if (W_icode == halt)
		halt_state = true;
	if (W_dstE != rnone)
		REG[W_dstE] = W_valE;
	if (W_dstM != rnone)
		REG[W_dstM] = W_valM;
}

//bubble and stall
void control()
{
	//F_stall
	if ((D_icode == ret || E_icode == ret || W_icode == ret) ||
		(E_icode == mrmovl || E_icode == popl) && (E_dstM == d_srcA || E_dstM == d_srcB))
		F_stall = true;
	else F_stall = false;

	//D_stall
	if ((E_icode == mrmovl || E_icode == popl) && (E_dstM == d_srcA || E_dstM == d_srcB))
		D_stall = true;
	else D_stall = false;
 
	//D_bubble
	if ((E_icode == jxx && !e_Cnd) ||
		!((E_icode == mrmovl || E_icode == popl) && (E_dstM == d_srcA || E_dstM == d_srcB))
		&& D_icode == ret || E_icode == ret || M_icode == ret)
		D_bubble = true;
	else D_bubble = false;

	//E_bubble
	if (((E_icode == mrmovl || E_icode == popl) && (E_dstM == d_srcA || E_dstM == d_srcB))
		|| (E_icode == jxx && !e_Cnd))
		E_bubble = true;
	else E_bubble = false;

}

//clock_cycle
void pass_parameter()
{

	// write_back
	W_icode = M_icode;
	W_valE = M_valE;
	W_valM = m_valM;
	W_dstE = M_dstE;
	W_dstM = M_dstM;

	// memory
	M_icode = E_icode;
	M_valE = e_valE;
	M_valA = E_valA;
	M_dstE = E_dstE;
	M_dstM = E_dstM;
	M_Cnd = e_Cnd;

	// execute
	E_icode = D_icode;
	E_ifun = D_ifun;
	E_valC = D_valC;
	E_valA = d_valA;
	E_valB = d_valB;
	E_dstE = d_dstE;
	E_dstM = d_dstM;
	E_srcA = d_srcA;
	E_srcB = d_srcB;

	if (E_bubble)
	{
		E_icode = nop;
		E_ifun = 0;
		E_dstE = rnone;
		E_dstM = rnone;
		E_srcA = rnone;
		E_srcB = rnone;
	}

	//decode
	if (!D_stall) {
		D_icode = f_icode;
		D_ifun = f_ifun;
		D_rA = f_rA;
		D_rB = f_rB;
		D_valC = f_valC;
		D_valP = f_valP;
	}
	if (D_bubble) {
		D_icode = nop;
		D_ifun = 0;
	}

	//fetch
	if (!F_stall)
		F_predPC = f_predPC;

}

void clock_cycle()
{

	write_back();
	memory();
	excute();
	decode();
	fetch();

	control();
	pass_parameter();
}
