#include<iostream>
#include<string>
#include<fstream>
#include"Lexical.h"
#include "LRAnalysis2.h"
using namespace std;

LexicalAnalysis lex;			// 词法分析
GrammaticalAnalysis gra;		// 语法分析
string filePath = "testData/";

int main() {

	string inputstr;				// 输入源程序字符串

	// 文件流读写
	ifstream fin(filePath + "input_1.txt");
	ofstream fout1(filePath + "output_1_lex.txt");
	ofstream fout2(filePath + "output_1_gra.txt");
	// 源文件读取
	string inbuffer;				// 输入缓冲区
	while (getline(fin, inbuffer) && inbuffer != "#") {
		inputstr += inbuffer;
	}
	// 依次对字符串进行词法分析识别完所有符号
	cout << "\n词法分析识别结果：" << endl;
	lex.reSetLexTable();
	lex.lexicalAnalysis(inputstr);
	vector<word> result = lex.getLexicalTable();
	for (vector<word>::iterator iter = result.begin(); iter != result.end(); iter++) {
		fout1 << iter->sym << " " << iter->token << endl;
	}
	result.push_back(word{ 20, "#" });		// 词法分析结果中加入一个终止符
	gra.reSetStack();		// 初始化
	gra.setLexicalTable(result);
	cout << "\n中间代码详见output_txt：" << endl;
	gra.grammaticalTranslation();

	vector<quaternion> midCodeResult = gra.getMidCode();

	stack<string> Mstack;
	for (auto iter = midCodeResult.begin(); iter != midCodeResult.end(); iter++) {
		if (iter->op == "while{}") {
			Mstack.push(iter->result.symValue);
			continue;
		}
		Mstack.push("err");
	}
	int tempN = 0;
	for (auto iter = midCodeResult.begin(); iter != midCodeResult.end(); iter++) {

		fout2 << ++tempN << "  ";
		if (iter->op == ">" || iter->op == "<" || iter->op == ">=" || iter->op == "<=" || iter->op == "==") {
			fout2 << "if" << "  " << iter->arg1.symName << "  " << iter->op << "  " << iter->arg2.symName << "  " << "goto" << "  " << iter->result.position << endl;
			iter++;
			if (iter->op == "goto") {
				fout2 << ++tempN << "  ";
				if (Mstack.top() == "err") {
					fout2 << "err analysis";
					break;
				}
				fout2 << "goto" << "  " << Mstack.top() << endl;
			}
			Mstack.pop();

			continue;
		}
		if (iter->op == "while{}") {
			fout2 << "goto" << "  " << iter->result.symName << "  " << endl;;
			continue;
		}
		if (iter->op == "=") {
			fout2 << iter->result.symName << "  " << iter->op << "  " << iter->arg1.symName << "  " << endl;
			continue;
		}
		else {
			fout2 << iter->result.symName << "  " << "=" << "  " << iter->arg1.symName << "  " << iter->op << "  " << iter->arg2.symName << "  " << endl;
			continue;
		}
	}
	fout2 << ++tempN << "  " << "end" << endl;

	//关闭文件流
	fin.close();
	fout1.close();
	fout2.close();
	return 0;
}