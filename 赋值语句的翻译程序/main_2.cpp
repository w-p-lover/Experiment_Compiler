#include<iostream>
#include<string>
#include<fstream>
#include"LexicalAnalysis.h"
#include"GrammaticalAnalysis.h"
using namespace std;

LexicalAnalysis lex;			// 词法分析
GrammaticalAnalysis gra;		// 语法分析
string filePath = "testData/";

int main_1() {

	string inputstr;				// 输入源程序字符串

	// 文件流读写
	ifstream fin(filePath + "input_1.txt");
	ofstream fout1(filePath + "output_1_lex.txt");
	ofstream fout2(filePath + "output_1_gra.txt");
	// 源文件读取
	string inbuffer;				// 输入缓冲区
	while (getline(fin, inbuffer) && inbuffer != "#"){
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
	cout << "\n四元式中间代码：" << endl;
	gra.grammaticalTranslation();

	vector<quaternion> midCodeResult = gra.getMidCode();
	for (auto iter = midCodeResult.begin(); iter != midCodeResult.end(); iter++) {
		fout2 << "(" << iter->op << "\t," << iter->arg1.symName << "\t," << iter->arg2.symName << "\t," << iter->result.symName << ")" << endl;
	}

	//关闭文件流
	fin.close();
	fout1.close();
	fout2.close();
	return 0;
}

int main_2() {
	string inputstr;				// 输入源程序字符串

	// 文件流读写
	ifstream fin(filePath + "input_2.txt");
	ofstream fout1(filePath + "output_2_lex.txt");
	ofstream fout2(filePath + "output_2_gra.txt");
	// 源文件读取
	string inbuffer;				// 输入缓冲区
	while (getline(fin, inbuffer) && inbuffer != "#") {
		inputstr += inbuffer;
	}

	// 依次对字符串进行词法分析识别完所有符号
	cout << "\n词法分析识别结果：" << endl;
	lex.reSetLexTable();
	lex.lexicalAnalysis(inputstr);
	vector<word> result1 = lex.getLexicalTable();
	for (vector<word>::iterator iter = result1.begin(); iter != result1.end(); iter++) {
		fout1 << iter->sym << " " << iter->token << endl;
	}
	result1.push_back(word{ 20, "#" });		// 词法分析结果中加入一个终止符
	gra.reSetStack();		// 初始化
	gra.setLexicalTable(result1);
	cout << "\n四元式中间代码：" << endl;
	gra.grammaticalTranslation();

	vector<quaternion> midCodeResult = gra.getMidCode();
	for (auto iter = midCodeResult.begin(); iter != midCodeResult.end(); iter++) {
		fout2 << "(" << iter->op << "\t," << iter->arg1.symName << "\t," << iter->arg2.symName << "\t," << iter->result.symName << ")" << endl;
	}

	//关闭文件流
	fin.close();
	fout1.close();
	fout2.close();
	return 0;
}

int main_3() {
	string inputstr;				// 输入源程序字符串

	// 文件流读写
	ifstream fin(filePath + "input_3.txt");
	ofstream fout1(filePath + "output_3_lex.txt");
	ofstream fout2(filePath + "output_3_gra.txt");
	// 源文件读取
	string inbuffer;				// 输入缓冲区
	while (getline(fin, inbuffer) && inbuffer != "#") {
		inputstr += inbuffer;
	}

	// 依次对字符串进行词法分析识别完所有符号
	cout << "\n词法分析识别结果：" << endl;
	lex.reSetLexTable();
	lex.lexicalAnalysis(inputstr);
	vector<word> result1 = lex.getLexicalTable();
	for (vector<word>::iterator iter = result1.begin(); iter != result1.end(); iter++) {
		fout1 << iter->sym << " " << iter->token << endl;
	}
	result1.push_back(word{ 20, "#" });		// 词法分析结果中加入一个终止符
	gra.reSetStack();		// 初始化
	gra.setLexicalTable(result1);
	cout << "\n四元式中间代码：" << endl;
	gra.grammaticalTranslation();

	vector<quaternion> midCodeResult = gra.getMidCode();
	for (auto iter = midCodeResult.begin(); iter != midCodeResult.end(); iter++) {
		fout2 << "(" << iter->op << "\t," << iter->arg1.symName << "\t," << iter->arg2.symName << "\t," << iter->result.symName << ")" << endl;
	}

	//关闭文件流
	fin.close();
	fout1.close();
	fout2.close();
	return 0;
}

int main_4() {
	string inputstr;				// 输入源程序字符串

	// 文件流读写
	ifstream fin(filePath + "input_4.txt");
	ofstream fout1(filePath + "output_4_lex.txt");
	ofstream fout2(filePath + "output_4_gra.txt");
	// 源文件读取
	string inbuffer;				// 输入缓冲区
	while (getline(fin, inbuffer) && inbuffer != "#") {
		inputstr += inbuffer;
	}

	// 依次对字符串进行词法分析识别完所有符号
	cout << "\n词法分析识别结果：" << endl;
	lex.reSetLexTable();
	lex.lexicalAnalysis(inputstr);
	vector<word> result1 = lex.getLexicalTable();
	for (vector<word>::iterator iter = result1.begin(); iter != result1.end(); iter++) {
		fout1 << iter->sym << " " << iter->token << endl;
	}
	result1.push_back(word{ 20, "#" });		// 词法分析结果中加入一个终止符
	gra.reSetStack();		// 初始化
	gra.setLexicalTable(result1);
	cout << "\n四元式中间代码：" << endl;
	gra.grammaticalTranslation();

	vector<quaternion> midCodeResult = gra.getMidCode();
	for (auto iter = midCodeResult.begin(); iter != midCodeResult.end(); iter++) {
		cout << "(" << iter->op << "\t," << iter->arg1.symName << "\t," << iter->arg2.symName << "\t," << iter->result.symName << ")" << endl;
	}

	//关闭文件流
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