#include<iostream>
#include<string>
#include<fstream>
#include"Lexical.h"
#include "LRAnalysis2.h"
using namespace std;

LexicalAnalysis lex;			// �ʷ�����
GrammaticalAnalysis gra;		// �﷨����
string filePath = "testData/";

int main() {

	string inputstr;				// ����Դ�����ַ���

	// �ļ�����д
	ifstream fin(filePath + "input_1.txt");
	ofstream fout1(filePath + "output_1_lex.txt");
	ofstream fout2(filePath + "output_1_gra.txt");
	// Դ�ļ���ȡ
	string inbuffer;				// ���뻺����
	while (getline(fin, inbuffer) && inbuffer != "#") {
		inputstr += inbuffer;
	}
	// ���ζ��ַ������дʷ�����ʶ�������з���
	cout << "\n�ʷ�����ʶ������" << endl;
	lex.reSetLexTable();
	lex.lexicalAnalysis(inputstr);
	vector<word> result = lex.getLexicalTable();
	for (vector<word>::iterator iter = result.begin(); iter != result.end(); iter++) {
		fout1 << iter->sym << " " << iter->token << endl;
	}
	result.push_back(word{ 20, "#" });		// �ʷ���������м���һ����ֹ��
	gra.reSetStack();		// ��ʼ��
	gra.setLexicalTable(result);
	cout << "\n�м�������output_txt��" << endl;
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

	//�ر��ļ���
	fin.close();
	fout1.close();
	fout2.close();
	return 0;
}