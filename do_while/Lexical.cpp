#include<iostream>
#include"Lexical.h"
using namespace std;

// 初始化词法分析所需要的关键字表
LexicalAnalysis::LexicalAnalysis() {
    CppTable[0] = "标识符(变量)";         // 标识符 0    常量 1~3
    CppTable[1] = "整数";
    CppTable[2] = "实数";
    CppTable[3] = "字符常量";
    CppTable[4] = "+";                   // 运算符 4~19 
    CppTable[5] = "-";
    CppTable[6] = "*";
    CppTable[7] = "/";
    CppTable[8] = "<";
    CppTable[9] = "<=";
    CppTable[10] = "==";
    CppTable[11] = "!=";
    CppTable[12] = ">";
    CppTable[13] = ">=";
    CppTable[14] = "&";
    CppTable[15] = "&&";
    CppTable[16] = "||";
    CppTable[17] = "=";
    CppTable[18] = "@";
    CppTable[19] = "!";
    CppTable[20] = "#";                  // 界符index 20~29
    CppTable[21] = "(";
    CppTable[22] = ")";
    CppTable[23] = "[";
    CppTable[24] = "]";
    CppTable[25] = "{";
    CppTable[26] = "}";
    CppTable[27] = ":";
    CppTable[28] = ";";
    CppTable[29] = ",";
    CppTable[30] = "void";               // 关键字 30~46
    CppTable[31] = "int";
    CppTable[32] = "float";
    CppTable[33] = "char";
    CppTable[34] = "if";
    CppTable[35] = "else";
    CppTable[36] = "while";
    CppTable[37] = "do";
    CppTable[38] = "for";
    CppTable[39] = "include";
    CppTable[40] = "iostream";
    CppTable[41] = "using";
    CppTable[42] = "namespace";
    CppTable[43] = "std";
    CppTable[44] = "main";
    CppTable[45] = "return";
    CppTable[46] = "null";
}

// 判断是否为字母
bool LexicalAnalysis::isLetter(char ch)
{
    if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
    {
        return true;
    }
    return false;

}
// 是否为数字
bool LexicalAnalysis::isNumber(char ch)
{
    if (ch >= '0' && ch <= '9')
    {
        return true;
    }
    return false;
}

// 判断是否为界符
bool LexicalAnalysis::isDelimiters(word word)
{
    if (word.sym >= 20 && word.sym <= 29) {
        return true;
    }
    return false;
};
// 判断是否为运算符
bool LexicalAnalysis::isOperator(word word)
{
    if (word.sym >= 4 && word.sym <= 19) {
        return true;
    }
    return false;
};
// 判断是否为关键字
bool LexicalAnalysis::isReservedWord(word word)
{
    if (word.sym >= 30 && word.sym <= 46) {
        return true;
    }
    return false;
};
// 判断是否为是常量
bool LexicalAnalysis::isconstantWord(word word)
{
    if (word.sym >= 1 && word.sym <= 3) {
        return true;
    }
    return false;
};
// 判断是否为标识符
bool LexicalAnalysis::isIdentifierWord(word word)
{
    if (word.sym == 0) {
        return true;
    }
    return false;
};

// 处理关键字和标识符
word LexicalAnalysis::keyWordAndIdAnalysis(const string& str)
{
    word item;
    bool isKeyword = false;
    // 在关键词表中寻找
    for (int i = 30; i <= 46; i++)
    {
        if (str.substr(0, CppTable[i].length()) == CppTable[i])
        {
            item.sym = i;
            item.token = CppTable[item.sym];
            isKeyword = true;
        }
    }
    if (!isKeyword)
    {
        // 如果不是上述关键词，则为标识符
        for (int i = 0; i <= str.length(); i++)
        { // 找到第一个不是数字、字母、下划线的位置
            if (!(isLetter(str[i]) || isNumber(str[i]) || str[i] == '_'))
            {
                item.sym = 0;       // Cpp关键词表中下标 0 表示标识符
                item.token = str.substr(0, i);
                break;
            }
        }
    }

    return item;
}

// 处理数字的函数
word LexicalAnalysis::numberAnalysis(const string& str)
{
    word item;
    for (int i = 0; i <= str.length(); ++i)
    {
        // 截取到第一个非数字和小数点字符
        if (!(isNumber(str[i]) || str[i] == '.'))
        {
            string curNum = str.substr(0, i);
            if (curNum.find('.') == string::npos) // 没读到'.'返回值为很大的数，若读到返回值是第一次出现的下标
                item.sym = 1;           // 1 整数
            else
                item.sym = 2;           // 2 实数位置
            item.token = curNum;

            break;
        }
    }

    return item;
}

// 处理字符常量的函数
word LexicalAnalysis::strAnalysis(const string& str)
{
    word item;
    int nextindex = str.find_first_of('"', 1);     // 找到第二个"出现的位置下标
    item.sym = 3;                                  // 3 字符串常量位置
    item.token = str.substr(0, nextindex + 1);

    return item;
}

// 处理字符的函数
word LexicalAnalysis::charAnalysis(string str)
{
    word item;
    switch (str[0])
    {
    case '#':
        item.sym = 20;
        break;
    case '+':
        item.sym = 4;
        break;
    case '-':
        item.sym = 5;
        break;
    case '*':
        item.sym = 6;
        break;
    case '/':
        item.sym = 7;
        break;
    case '<':
        if (str[1] == '=') {
            item.sym = 9;
        }
        else {
            item.sym = 8;
        }
        break;
    case '=':
        if (str[1] == '=') {
            item.sym = 10;
        }
        else {
            item.sym = 17;
        }
        break;
    case '!':
        if (str[1] == '=') {
            item.sym = 11;
        }
        else {
            item.sym = 19;
        }
        break;
    case '>':
        if (str[1] == '=') {
            item.sym = 13;
        }
        else {
            item.sym = 12;
        }
        break;
    case '&':
        if (str[1] == '&') {
            item.sym = 15;
        }
        else {
            item.sym = 14;
        }
        break;
    case '|':
        if (str[1] == '|') {
            item.sym = 16;
        }
        break;
    case '(':
        item.sym = 21;
        break;
    case ')':
        item.sym = 22;
        break;
    case '[':
        item.sym = 23;
        break;
    case ']':
        item.sym = 24;
        break;
    case '{':
        item.sym = 25;
        break;
    case '}':
        item.sym = 26;
        break;
    case ':':
        item.sym = 27;
        break;
    case ';':
        item.sym = 28;
        break;
    case ',':
        item.sym = 29;
        break;
    case '@':
        item.sym = 18;
        break;
    default:
        item.sym = -1;      // 非法字符
    }
    if (item.sym != -1) {
        item.token = CppTable[item.sym];
    }
    else {
        item.token = str[0];        // 记录非法字符
    }
    return item;
}

// 获取单词种类
type LexicalAnalysis::getWordType(word word) {
    if (isOperator(word)) {
        return type::opStack;
    }
    if (isDelimiters(word)) {
        return type::del;
    }
    if (isconstantWord(word)) {
        return type::con;
    }
    if (isReservedWord(word)) {
        return type::res;
    }
    if (isIdentifierWord(word)) {
        return type::iden;
    }
    return type::err;
}

// 输出单词二元式
void LexicalAnalysis::printWord(word item) {
    switch (getWordType(item)) {
    case type::opStack:
        cout << "( 操作符, ";
        break;
    case type::del:
        cout << "( 界符, ";
        break;
    case type::res:
        cout << "( 关键字, ";
        break;
    case type::con:
        cout << "( 常量, ";
        break;
    case type::iden:
        cout << "( 标识符, ";
        break;
    default:
        cout << "err input: ";
    }
    cout << item.sym << " , " << item.token << " )" << endl;
}

// 词法分析主函数
void LexicalAnalysis::lexicalAnalysis(const string& inputStr)
{
    for (int i = 0; i < inputStr.length(); ++i)
    {
        word item;
        if (inputStr[i] == ' '||inputStr[i] == '\t'|| inputStr[i] == '\n')
        {
            // 如果是空格 回车 指表符，直接跳过
            continue;
        }
        else if (isLetter(inputStr[i]))
        {
            // 处理单词
            item = keyWordAndIdAnalysis(inputStr.substr(i, inputStr.length() - i + 1));
        }
        else if (isNumber(inputStr[i]))
        {
            // 处理数字
            item = numberAnalysis(inputStr.substr(i, inputStr.length() - i + 1));
        }
        else if (inputStr[i] == '"') {
            // 处理字符串常量
            item = strAnalysis(inputStr.substr(i, inputStr.length() - i + 1));
        }
        else
        {
            // 处理特殊符号
            item = charAnalysis(inputStr.substr(i, inputStr.length() - i + 1));
        }
        i += item.token.length() - 1;
        lexicalTable.push_back(item);       // 词法处理完的结果存入lexicalTable中

        printWord(item);        // 输出词法分析结果 if need
    }
}