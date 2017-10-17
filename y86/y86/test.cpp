#include"head.h"
#include<iostream>
#include<fstream>

using namespace std;

int main()
{
	cout << "INPUT FILENAME:" << endl;
	readin();
	initial();//读取之后先初始化
	while (W_icode != halt)
	{
		write_back();
		memory();
		excute();
		decode();
		fetch();

		cout << "EAX:" << REG[reax] << '\t' << "EBX:" << REG[rebx] <<
			"\tECX:" << REG[recx] << "\tESP:" << REG[resp] << endl << "EDX:" << REG[redx] << "\tESI:" << REG[resi]
			<< "\tEDI:" << REG[redi] << "\tEBP:" << REG[rebp] << endl;
		cout << "f_icode:" << f_icode << "\tf_ifun:" << f_ifun << "\tf_rA:" << f_rA << "\tf_rB:" << f_rB << "\tf_valC:" << f_valC << "\tf_predPC:" << f_predPC << "\tf_PC:" << f_PC << endl;
		cout << "D_icode:" << D_icode << "\tD_ifun:" << D_ifun << "\td_srcA:" << d_srcA << "\td_srcB:" << d_srcB << "\td_dstE:" << d_dstE << "\td_dstM:" << d_dstM << "\td_valA:" << d_valA
			<< endl << "d_valB:" << d_valB << endl;
		cout << "E_icode:" << E_icode << "\tE_ifun:" << E_ifun << "\tE_dstE:" << E_dstE << "\tE_dstM:" << E_dstM << "\tE_valA:" << E_valA << "\tE_valB:" << E_valB <<
			endl << "e_valE:" << e_valE << "\te_Cnd:" << e_Cnd << "\tOF:" << OF << "\tZF:" << ZF << "\tSF:" << SF << endl;
		cout << "M_icode:" << M_icode << "\tM_dstE:" << M_dstE << "\tM_dstM:" << M_dstM << "\tM_valE:" << M_valE << "\tm_valM:" << m_valM << "\tM_valA:" << M_valA << "\tM_Cnd:" << M_Cnd << endl;
		cout << "W_icode:" << W_icode << "\tW_dstE:" << W_dstE << "\tW_dstM:" << W_dstM << "\tW_valE:" << W_valE << "\tW_valM:" << W_valM << endl;

		control();
		pass_parameter();
	}
	return 0;
}