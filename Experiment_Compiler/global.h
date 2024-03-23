#pragma once
#include<map>
using namespace std;
#define RNUM 13
#define DNUM 5
#define ONUM 11

// 单词种别 nul——表示“不能识别的字符”
enum symbol {
	nul, ident, number, plus, minus, times, slash, oddsym, eql, neq,
	lss, leq, gtr, geq, lparen, rparen, comma, semicolon, period, becomes,
	beginsym, endsym, ifsym, thensym, whilesym, writesym, dosym, callsym,
	constsym, varsym, procsym,
};
