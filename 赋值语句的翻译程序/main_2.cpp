#include<iostream>
#include<string>
#include<fstream>
#include"LexicalAnalysis.h"
#include"GrammaticalAnalysis.h"
using namespace std;

LexicalAnalysis lex;			// �ʷ�����
GrammaticalAnalysis gra;		// �﷨����
string filePath = "testData/";

int main_1() {

	string inputstr;				// ����Դ�����ַ���

	// �ļ�����д
	ifstream fin(filePath + "input_1.txt");
	ofstream fout1(filePath + "output_1_lex.txt");
	ofstream fout2(filePath + "output_1_gra.txt");
	// Դ�ļ���ȡ
	string inbuffer;				// ���뻺����
	while (getline(fin, inbuffer) && inbuffer != "#"){
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
	cout << "\n��Ԫʽ�м���룺" << endl;
	gra.grammaticalTranslation();

	vector<quaternion> midCodeResult = gra.getMidCode();
	for (auto iter = midCodeResult.begin(); iter != midCodeResult.end(); iter++) {
		fout2 << "(" << iter->op << "\t," << iter->arg1.symName << "\t," << iter->arg2.symName << "\t," << iter->result.symName << ")" << endl;
	}

	//�ر��ļ���
	fin.close();
	fout1.close();
	fout2.close();
	return 0;
}

int main_2() {
	string inputstr;				// ����Դ�����ַ���

	// �ļ�����д
	ifstream fin(filePath + "input_2.txt");
	ofstream fout1(filePath + "output_2_lex.txt");
	ofstream fout2(filePath + "output_2_gra.txt");
	// Դ�ļ���ȡ
	string inbuffer;				// ���뻺����
	while (getline(fin, inbuffer) && inbuffer != "#") {
		inputstr += inbuffer;
	}

	// ���ζ��ַ������дʷ�����ʶ�������з���
	cout << "\n�ʷ�����ʶ������" << endl;
	lex.reSetLexTable();
	lex.lexicalAnalysis(inputstr);
	vector<word> result1 = lex.getLexicalTable();
	for (vector<word>::iterator iter = result1.begin(); iter != result1.end(); iter++) {
		fout1 << iter->sym << " " << iter->token << endl;
	}
	result1.push_back(word{ 20, "#" });		// �ʷ���������м���һ����ֹ��
	gra.reSetStack();		// ��ʼ��
	gra.setLexicalTable(result1);
	cout << "\n��Ԫʽ�м���룺" << endl;
	gra.grammaticalTranslation();

	vector<quaternion> midCodeResult = gra.getMidCode();
	for (auto iter = midCodeResult.begin(); iter != midCodeResult.end(); iter++) {
		fout2 << "(" << iter->op << "\t," << iter->arg1.symName << "\t," << iter->arg2.symName << "\t," << iter->result.symName << ")" << endl;
	}

	//�ر��ļ���
	fin.close();
	fout1.close();
	fout2.close();
	return 0;
}

int main_3() {
	string inputstr;				// ����Դ�����ַ���

	// �ļ�����д
	ifstream fin(filePath + "input_3.txt");
	ofstream fout1(filePath + "output_3_lex.txt");
	ofstream fout2(filePath + "output_3_gra.txt");
	// Դ�ļ���ȡ
	string inbuffer;				// ���뻺����
	while (getline(fin, inbuffer) && inbuffer != "#") {
		inputstr += inbuffer;
	}

	// ���ζ��ַ������дʷ�����ʶ�������з���
	cout << "\n�ʷ�����ʶ������" << endl;
	lex.reSetLexTable();
	lex.lexicalAnalysis(inputstr);
	vector<word> result1 = lex.getLexicalTable();
	for (vector<word>::iterator iter = result1.begin(); iter != result1.end(); iter++) {
		fout1 << iter->sym << " " << iter->token << endl;
	}
	result1.push_back(word{ 20, "#" });		// �ʷ���������м���һ����ֹ��
	gra.reSetStack();		// ��ʼ��
	gra.setLexicalTable(result1);
	cout << "\n��Ԫʽ�м���룺" << endl;
	gra.grammaticalTranslation();

	vector<quaternion> midCodeResult = gra.getMidCode();
	for (auto iter = midCodeResult.begin(); iter != midCodeResult.end(); iter++) {
		fout2 << "(" << iter->op << "\t," << iter->arg1.symName << "\t," << iter->arg2.symName << "\t," << iter->result.symName << ")" << endl;
	}

	//�ر��ļ���
	fin.close();
	fout1.close();
	fout2.close();
	return 0;
}

int main_4() {
	string inputstr;				// ����Դ�����ַ���

	// �ļ�����д
	ifstream fin(filePath + "input_4.txt");
	ofstream fout1(filePath + "output_4_lex.txt");
	ofstream fout2(filePath + "output_4_gra.txt");
	// Դ�ļ���ȡ
	string inbuffer;				// ���뻺����
	while (getline(fin, inbuffer) && inbuffer != "#") {
		inputstr += inbuffer;
	}

	// ���ζ��ַ������дʷ�����ʶ�������з���
	cout << "\n�ʷ�����ʶ������" << endl;
	lex.reSetLexTable();
	lex.lexicalAnalysis(inputstr);
	vector<word> result1 = lex.getLexicalTable();
	for (vector<word>::iterator iter = result1.begin(); iter != result1.end(); iter++) {
		fout1 << iter->sym << " " << iter->token << endl;
	}
	result1.push_back(word{ 20, "#" });		// �ʷ���������м���һ����ֹ��
	gra.reSetStack();		// ��ʼ��
	gra.setLexicalTable(result1);
	cout << "\n��Ԫʽ�м���룺" << endl;
	gra.grammaticalTranslation();

	vector<quaternion> midCodeResult = gra.getMidCode();
	for (auto iter = midCodeResult.begin(); iter != midCodeResult.end(); iter++) {
		cout << "(" << iter->op << "\t," << iter->arg1.symName << "\t," << iter->arg2.symName << "\t," << iter->result.symName << ")" << endl;
	}

	//�ر��ļ���
	fin.close();
	fout1.close();
	fout2.close();
	return 0;
}

int main() {
	main_1();
	main_2();
	main_3();
	main_4();
}