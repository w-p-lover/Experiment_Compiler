#pragma once
#include<string>
#include<map>
#include"global.h"
#include"Word.h"
using namespace std;


// 高级语言类 PL/0
class Language
{

private:
	string ReservedWord[RNUM];		// 关键字
	string Delimiter[DNUM];			// 界符
	string Operator[ONUM];			// 运算符
	map<string, int>Identifier;		// 标识符表
	map<string, int>Constant;		// 常数表
	map<string, int>::iterator ite; // 迭代器

private:
	void initReservedWrods();
	void initDelimiters();
	void initOperators();

public:
	Language();
	~Language();
	bool isDelimiters(string str);		// 判断是否为界符
	bool isOperator(string str);		// 判断是否为运算符
	bool isReservedWord(string str);	// 判断是否为关键字

	Word identifierWord(string str);	// str为标识符，将其加入标识符表后返回对应word；若已存在则直接返回对应word
	Word constantWord(string str);		// str为常数，将其加入常熟表后返回对应word；若已存在则直接返回对应word
};

