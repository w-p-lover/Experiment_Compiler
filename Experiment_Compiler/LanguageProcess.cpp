#include "LanguageProcess.h"


LanguageProcess::LanguageProcess() {
	sym = nul;
	num = 0;
	id = "";
	index = 0;
}

LanguageProcess::~LanguageProcess() {

}

// �ж��Ƿ�Ϊ��ĸ
bool LanguageProcess::isLetter(string ch)
{
	if ((ch >= "a" && ch <= "z") || (ch >= "A" && ch <= "Z"))
	{
		return true;
	}
	return false;
}

// �Ƿ�Ϊ����
bool LanguageProcess::isNumber(string ch)
{
	if (ch >= "0" && ch <= "9")
	{
		return true;
	}
	return false;
}

// �ʷ���������
Word LanguageProcess::lexicalAnalysis(string inputstr)
{
	string now_str = "";			// �Ѷ����ַ���
	string ch;						// ��ȡ��ȡ���ַ�
	ch = inputstr[index++];			// ��ȡһ���ַ�
	
	// ���Կո񡢻��С��س���table
	while (ch == " " || ch == "\n" || ch == "\r" || ch == "\t")
		ch = inputstr[index++];

	if (isLetter(ch)) {				// ��ʶ��������������ĸ��ͷ

		while (isLetter(ch) || isNumber(ch)) {		// ��ʶ������ɹ���

			now_str += ch;
			ch = inputstr[index++];
		}

		ch = " ";
		index--;			// ��������λ��,��ֹ��λ
		// ʶ�𵥴��Ǳ����ֻ��Ǳ�ʶ��
		// ���Ƿ��Ǳ�����
		if (language.isReservedWord(now_str)) {
			return Word(3, now_str);				// �Ǳ�����
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
		index--;		   // ��������λ��

		return language.constantWord(now_str);

	}
	else if (language.isDelimiters(ch)) {
		now_str += ch;
		return Word(5, now_str);
	}
	else if (ch == ":") {
		now_str += ch;				// ����":"
		ch = inputstr[index++];
		now_str += ch;				// ��nextcharƴ�ӵ�":"��
		if (language.isOperator(now_str)) {					// �������
			return Word(4, now_str);
		}
		else {						// �ǿ�ʶ�𵥴�
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

//�ʷ���������
Word LanguageProcess::lexicalAnalysis2(string inputstr)
{
	string now_str = "";			// �Ѷ����ַ���
	string ch;						//��ȡ��ȡ���ַ�
	ch = inputstr[index++];			//��ȡһ���ַ�

	// ���Կո񡢻��С��س���table
	while (ch == " " || ch == "\n" || ch == "\r" || ch == "\t")
		ch = inputstr[index++];

	if (isLetter(ch)||ch == "_") {				// ��ʶ��������������ĸ���»��߿�ͷ

		while (isLetter(ch) || isNumber(ch)) {		// ��ʶ������ɹ���

			now_str += ch;
			ch = inputstr[index++];
		}

		ch = " ";
		index--;			// ��������λ��,��ֹ��λ
		// ʶ�𵥴��Ǳ����ֻ��Ǳ�ʶ��
		// ���Ƿ��Ǳ�����
		if (language.isReservedWord(now_str)) {
			return Word(3, now_str);				// �Ǳ�����
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
		index--;		   // ��������λ��
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
		now_str += ch;				// ����":"
		ch = inputstr[index++];
		now_str += ch;				// ��nextcharƴ�ӵ�":"��
		if (language.isOperator(now_str)) {					// �������
			return Word(4, now_str);
		}
		else {						// �ǿ�ʶ�𵥴�
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