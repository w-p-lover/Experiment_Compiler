#include<iostream>
#include<string>
#include<fstream>
#include"LanguageProcess.h"
#include"Word.h"
using namespace std;


void main_true() {
	LanguageProcess process;
	string inputstr;				// 输入源程序字符串
	Word word(0, "");

	// 文件流读写
	ifstream fin("input_1.txt");
	ofstream fout("output_1.txt");
	// 源文件读取
	string inbuffer;				// 输入缓冲区
	while (getline(fin, inbuffer) && inbuffer != "#")
	{
		inputstr += inbuffer;
	}

	// 依次对字符串进行词法分析识别完所有符号
	fout << "词法分析识别结果：" << endl;
	while (!process.isOver(inputstr)){
		word = process.lexicalAnalysis(inputstr);
		fout << word;
	}

	//关闭文件流
	fin.close();
	fout.close();
}

void main_false() {
	LanguageProcess process;
	string inputstr;				// 输入源程序字符串
	Word word(0, "");

	// 文件流读写
	ifstream fin("input_2.txt");
	ofstream fout("output_2.txt");
	// 源文件读取
	string inbuffer;				// 输入缓冲区
	while (getline(fin, inbuffer) && inbuffer != "#"){
		inputstr += inbuffer;
	}

	// 依次对字符串进行词法分析识别完所有符号
	fout << "词法分析识别结果：" << endl;
	while (!process.isOver(inputstr)){
		word = process.lexicalAnalysis(inputstr);
		fout << word;
	}

	// 关闭文件流
	fin.close();
	fout.close();
}

int main() {
	main_true();
	main_false();
	cout << "词法分析结束，请查看文件" << endl;
	return 0;
}

