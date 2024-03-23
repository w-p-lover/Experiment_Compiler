#include "Language.h"

Language::Language() {
	initOperators();
	initDelimiters();
	initReservedWrods();
}

Language::~Language() {

}

void Language::initReservedWrods(){
	ReservedWord[0] = "const";
	ReservedWord[1] = "var";
	ReservedWord[2] = "procedure";
	ReservedWord[3] = "begin";
	ReservedWord[4] = "end";
	ReservedWord[5] = "odd";
	ReservedWord[6] = "if";
	ReservedWord[7] = "then";
	ReservedWord[8] = "call";
	ReservedWord[9] = "while";
	ReservedWord[10] = "do";
	ReservedWord[11] = "read";
	ReservedWord[12] = "write";
}

void Language::initDelimiters(){
	Delimiter[0] = "(";
	Delimiter[1] = ")";
	Delimiter[2] = ",";
	Delimiter[3] = ";";
	Delimiter[4] = ".";
}

void Language::initOperators() {
	Operator[0] = "+";
	Operator[1] = "-";
	Operator[2] = "*";
	Operator[3] = "/";
	Operator[4] = "=";
	Operator[5] = "#";
	Operator[6] = "<";
	Operator[7] = "<=";
	Operator[8] = ">";
	Operator[9] = ">=";
	Operator[10] = ":=";
}

// 判断是否为关键字
bool Language::isReservedWord(string str){

	for (int i = 0; i < RNUM; i++)
	{
		if (str == ReservedWord[i])
		{
			return true;
		}
	}
	return false;
}


// 判断是否为界符
bool Language::isDelimiters(string str){

	for (int i = 0; i < DNUM; i++)
	{
		if (str == Delimiter[i])
		{
			return true;
		}
	}
	return false;
}

// 判断是否为运算符
bool Language::isOperator(string str){

	for (int i = 0; i < ONUM; i++)
	{
		if (str == Operator[i])
		{
			return true;
		}
	}
	return false;
}

Word Language::identifierWord(string str) {

	ite = Identifier.find(str);				// 查找str
	if (ite != Identifier.end()){			// 已存在该元素
		return Word(1, str);
	}
	else{
		Identifier[str] = Identifier.size();	
		return Word(1, str);
	}
}

Word Language::constantWord(string str) {
	ite = Constant.find(str);				// 查找str
	if (ite != Constant.end())				// 已存在该元素
	{
		return Word(2, str);
	}
	else
	{
		Constant[str] = Constant.size();	// 不存在-->插入到常数表末尾
		return Word(2, str);
	}
}