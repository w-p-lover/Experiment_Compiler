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

// �ж��Ƿ�Ϊ�ؼ���
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


// �ж��Ƿ�Ϊ���
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

// �ж��Ƿ�Ϊ�����
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

	ite = Identifier.find(str);				// ����str
	if (ite != Identifier.end()){			// �Ѵ��ڸ�Ԫ��
		return Word(1, str);
	}
	else{
		Identifier[str] = Identifier.size();	
		return Word(1, str);
	}
}

Word Language::constantWord(string str) {
	ite = Constant.find(str);				// ����str
	if (ite != Constant.end())				// �Ѵ��ڸ�Ԫ��
	{
		return Word(2, str);
	}
	else
	{
		Constant[str] = Constant.size();	// ������-->���뵽������ĩβ
		return Word(2, str);
	}
}