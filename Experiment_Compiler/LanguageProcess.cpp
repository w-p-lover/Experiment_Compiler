#include "LanguageProcess.h"


LanguageProcess::LanguageProcess() {
	sym = nul;
	num = 0;
	id = "";
	index = 0;
}

LanguageProcess::~LanguageProcess() {

}

// 判断是否为字母
bool LanguageProcess::isLetter(string ch)
{
	if ((ch >= "a" && ch <= "z") || (ch >= "A" && ch <= "Z"))
	{
		return true;
	}
	return false;
}

// 是否为数字
bool LanguageProcess::isNumber(string ch)
{
	if (ch >= "0" && ch <= "9")
	{
		return true;
	}
	return false;
}

// 词法分析函数
Word LanguageProcess::lexicalAnalysis(string inputstr)
{
	string now_str = "";			// 已读入字符串
	string ch;						// 存取读取的字符
	ch = inputstr[index++];			// 读取一个字符
	
	// 忽略空格、换行、回车和table
	while (ch == " " || ch == "\n" || ch == "\r" || ch == "\t")
		ch = inputstr[index++];

	if (isLetter(ch)) {				// 标识符、保留字以字母开头

		while (isLetter(ch) || isNumber(ch)) {		// 标识符的组成规则

			now_str += ch;
			ch = inputstr[index++];
		}

		ch = " ";
		index--;			// 读完后回退位置,防止错位
		// 识别单词是保留字还是标识符
		// 看是否是保留字
		if (language.isReservedWord(now_str)) {
			return Word(3, now_str);				// 是保留字
		}
		else {
			return language.identifierWord(now_str);
		}
	}
	else if (isNumber(ch)) {
		while (isNumber(ch)) {
			now_str += ch;
			ch = inputstr[index++];
		}

		ch = " ";
		index--;		   // 读完后回退位置

		return language.constantWord(now_str);

	}
	else if (language.isDelimiters(ch)) {
		now_str += ch;
		return Word(5, now_str);
	}
	else if (ch == ":") {
		now_str += ch;				// 读入":"
		ch = inputstr[index++];
		now_str += ch;				// 将nextchar拼接到":"后
		if (language.isOperator(now_str)) {					// 是运算符
			return Word(4, now_str);
		}
		else {						// 非可识别单词
			index--;
			return Word(0, now_str);
		}
	}
	else if(ch == "<"||ch == ">") {		// > ? < ? >=? <= ?
		now_str += ch;
		ch = inputstr[index++];
		now_str += ch;
		if (language.isOperator(now_str)) {
			return Word(4, now_str);
		}
		else {
			index--;
			return Word(4, now_str);
		}
	}
	else if (language.isOperator(ch)) {			// + - * ...
		return Word(4, now_str);
	}
	else
	{
		now_str += ch;
		return Word(0, now_str);
	}
}

bool LanguageProcess::isOver(string str) {
	if (index < str.size()) {
		return false;
	}
	else {
		return true;
	}
}

//词法分析函数
Word LanguageProcess::lexicalAnalysis2(string inputstr)
{
	string now_str = "";			// 已读入字符串
	string ch;						//存取读取的字符
	ch = inputstr[index++];			//读取一个字符

	// 忽略空格、换行、回车和table
	while (ch == " " || ch == "\n" || ch == "\r" || ch == "\t")
		ch = inputstr[index++];

	if (isLetter(ch)||ch == "_") {				// 标识符、保留字以字母或下划线开头

		while (isLetter(ch) || isNumber(ch)) {		// 标识符的组成规则

			now_str += ch;
			ch = inputstr[index++];
		}

		ch = " ";
		index--;			// 读完后回退位置,防止错位
		// 识别单词是保留字还是标识符
		// 看是否是保留字
		if (language.isReservedWord(now_str)) {
			return Word(3, now_str);				// 是保留字
		}
		else {
			return language.identifierWord(now_str);
		}
	}
	else if (isNumber(ch)) {
		int flag = 0;
		while (isNumber(ch)||ch == ".") {
			if (ch == ".") {
				flag ++;
			}
			now_str += ch;
			ch = inputstr[index++];
			if (flag >= 2) {
				break;
			}
		}

		ch = " ";
		index--;		   // 读完后回退位置
		if (flag >= 2) {
			return Word(0, now_str);
		}
		return language.constantWord(now_str);

	}
	else if (language.isDelimiters(ch)) {
		now_str += ch;
		return Word(5, now_str);
	}
	else if (ch == ":") {
		now_str += ch;				// 读入":"
		ch = inputstr[index++];
		now_str += ch;				// 将nextchar拼接到":"后
		if (language.isOperator(now_str)) {					// 是运算符
			return Word(4, now_str);
		}
		else {						// 非可识别单词
			index--;
			return Word(0, now_str);
		}
	}
	else if (ch == "<" || ch == ">") {		// > ? < ? >=? <= ?
		now_str += ch;
		ch = inputstr[index++];
		now_str += ch;
		if (language.isOperator(now_str)) {
			return Word(4, now_str);
		}
		else {
			index--;
			return Word(4, now_str);
		}
	}
	else if (language.isOperator(ch)) {			// + - * ...
		return Word(4, now_str);
	}
	else
	{
		now_str += ch;
		return Word(0, now_str);
	}
}