#pragma once
#include<string>
#include<iostream>
using namespace std;

// 单词属性字结构表示 二元组
class Word
{
public:
	int type;			// 单词种别
	string values;		// 单词自身的值

	Word(int s, string v)
	{
		this->type = s;
		this->values = v;
	}

	friend ostream& operator << (std::ostream& os, Word word) {
		switch (word.type) {
		case 0:
			os << "Wrong：";
			break;
		case 1:
			os << "标识符";
			break;
		case 2:
			os << "常数 ";
			break;
		case 3:
			os << "关键字";
			break;
		case 4:
			os << "运算符";
			break;
		case 5:
			os << "界符 ";
			break;
		default:
			break;
		}
		return os << "\t(" << word.type << ",\'" << word.values << "\')" << endl;
	}
};
