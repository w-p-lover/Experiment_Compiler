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
	enum symbol sym;		// 通过其传递单词类别 e.g. "ident"
	string id;				// 传递标识符的名字 e.g. "position"
	int num;				// 传递无符号整数单词的值 e.g. 60
	int index;				// 字符索引
private:
	bool isLetter(string ch);		// 判断是否为字母
	bool isNumber(string ch);		// 判断是否为数字

public:
	LanguageProcess();
	virtual ~LanguageProcess();
	Word lexicalAnalysis(string inputstr);
	Word lexicalAnalysis2(string inputstr);
	bool isOver(string str);
};

