#pragma once
#include<string>
#include"Language.h"
#include"global.h"
#include"Word.h"
using namespace std;


class LanguageProcess
{
private:
	Language language;
	enum symbol sym;		// ͨ���䴫�ݵ������ e.g. "ident"
	string id;				// ���ݱ�ʶ�������� e.g. "position"
	int num;				// �����޷����������ʵ�ֵ e.g. 60
	int index;				// �ַ�����
private:
	bool isLetter(string ch);		// �ж��Ƿ�Ϊ��ĸ
	bool isNumber(string ch);		// �ж��Ƿ�Ϊ����

public:
	LanguageProcess();
	virtual ~LanguageProcess();
	Word lexicalAnalysis(string inputstr);
	Word lexicalAnalysis2(string inputstr);
	bool isOver(string str);
};

