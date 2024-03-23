#include<iostream>
#include<string>
#include<fstream>
#include"LanguageProcess.h"
#include"Word.h"
using namespace std;


void main_true() {
	LanguageProcess process;
	string inputstr;				// ����Դ�����ַ���
	Word word(0, "");

	// �ļ�����д
	ifstream fin("input_1.txt");
	ofstream fout("output_1.txt");
	// Դ�ļ���ȡ
	string inbuffer;				// ���뻺����
	while (getline(fin, inbuffer) && inbuffer != "#")
	{
		inputstr += inbuffer;
	}

	// ���ζ��ַ������дʷ�����ʶ�������з���
	fout << "�ʷ�����ʶ������" << endl;
	while (!process.isOver(inputstr)){
		word = process.lexicalAnalysis(inputstr);
		fout << word;
	}

	//�ر��ļ���
	fin.close();
	fout.close();
}

void main_false() {
	LanguageProcess process;
	string inputstr;				// ����Դ�����ַ���
	Word word(0, "");

	// �ļ�����д
	ifstream fin("input_2.txt");
	ofstream fout("output_2.txt");
	// Դ�ļ���ȡ
	string inbuffer;				// ���뻺����
	while (getline(fin, inbuffer) && inbuffer != "#"){
		inputstr += inbuffer;
	}

	// ���ζ��ַ������дʷ�����ʶ�������з���
	fout << "�ʷ�����ʶ������" << endl;
	while (!process.isOver(inputstr)){
		word = process.lexicalAnalysis(inputstr);
		fout << word;
	}

	// �ر��ļ���
	fin.close();
	fout.close();
}

int main() {
	main_true();
	main_false();
	cout << "�ʷ�������������鿴�ļ�" << endl;
	return 0;
}

