#include"head.h"
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<map>

using namespace std;

std::ifstream readin()
{
	string FILENAME;
	cin >> FILENAME;

	ifstream fin(FILENAME, ios::binary);

	//这种读入方式适用于助教给的测试文件
	vector<char> insset((istreambuf_iterator<char>(fin)),istreambuf_iterator<char>());
	int bytes = 0;
	for(vector<char>::iterator iter = insset.begin(); iter != insset.end(); iter++,bytes++)
	MEM[bytes] =  insset[bytes];	
	//下面这种读取文件的方式适用于：汇编代码用二进制表示的txt文件
	/*
	int bytes = 0;
	string ins;
	while (fin >> ins)
	{
		int inscode = 0;
		for (int bits = 0; bits < 8; bits++)
			inscode = (inscode << 1) + ins[bits] - '0';
		MEM[bytes] = inscode;
		bytes++;
	}
	*/
	return fin;
}

