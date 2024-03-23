#pragma once
#include<string>
#include<map>
#include"global.h"
#include"Word.h"
using namespace std;


// �߼������� PL/0
class Language
{

private:
	string ReservedWord[RNUM];		// �ؼ���
	string Delimiter[DNUM];			// ���
	string Operator[ONUM];			// �����
	map<string, int>Identifier;		// ��ʶ����
	map<string, int>Constant;		// ������
	map<string, int>::iterator ite; // ������

private:
	void initReservedWrods();
	void initDelimiters();
	void initOperators();

public:
	Language();
	~Language();
	bool isDelimiters(string str);		// �ж��Ƿ�Ϊ���
	bool isOperator(string str);		// �ж��Ƿ�Ϊ�����
	bool isReservedWord(string str);	// �ж��Ƿ�Ϊ�ؼ���

	Word identifierWord(string str);	// strΪ��ʶ������������ʶ����󷵻ض�Ӧword�����Ѵ�����ֱ�ӷ��ض�Ӧword
	Word constantWord(string str);		// strΪ������������볣���󷵻ض�Ӧword�����Ѵ�����ֱ�ӷ��ض�Ӧword
};

