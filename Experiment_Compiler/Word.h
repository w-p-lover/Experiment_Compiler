#pragma once
#include<string>
#include<iostream>
using namespace std;

// ���������ֽṹ��ʾ ��Ԫ��
class Word
{
public:
	int type;			// �����ֱ�
	string values;		// ���������ֵ

	Word(int s, string v)
	{
		this->type = s;
		this->values = v;
	}

	friend ostream& operator << (std::ostream& os, Word word) {
		switch (word.type) {
		case 0:
			os << "Wrong��";
			break;
		case 1:
			os << "��ʶ��";
			break;
		case 2:
			os << "���� ";
			break;
		case 3:
			os << "�ؼ���";
			break;
		case 4:
			os << "�����";
			break;
		case 5:
			os << "��� ";
			break;
		default:
			break;
		}
		return os << "\t(" << word.type << ",\'" << word.values << "\')" << endl;
	}
};
