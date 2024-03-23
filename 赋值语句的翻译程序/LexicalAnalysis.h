#pragma once
#include<string>
#include<vector>
using namespace std;


// 单词类
class word{
public:
    int sym{};            // 通过其传递单词类别 e.g. "ident"
    string token;         // 单词的值
};

// 单词类别
enum class type {
    del, opStack, res, con, iden, err
};

// 词法分析类(C++)
class LexicalAnalysis{
private:
    // C++关键字表(部分)
    string CppTable[75];
    // 存储词法分析结果
    vector<word> lexicalTable;
    
private:
    bool isLetter(char ch);		    // 判断是否为字母
    bool isNumber(char ch);		    // 判断是否为数字

    bool isDelimiters(word word);	    // 判断是否为界符
    bool isOperator(word word);		    // 判断是否为运算符
    bool isReservedWord(word word);	    // 判断是否为关键字
    bool isconstantWord(word word);     // 判断是否为是常量
    bool isIdentifierWord(word word);   // 判断是否为标识符

    word keyWordAndIdAnalysis(const string& str);         // 处理关键字和标识符的分析函数
    word numberAnalysis(const string& str);               // 处理数字的分析函数
    word strAnalysis(const string& str);                  // 处理字符常量的函数
    word charAnalysis(string str);                        // 处理字符的分析函数

public:
    LexicalAnalysis();
    type getWordType(word word);                                // 获取单词种类
    void printWord(word item);                                  // 输出单词二元式

    void lexicalAnalysis(const string& inputStr);               // 词法分析主程序
    vector<word> getLexicalTable() { return lexicalTable; };    // 获取词法分析结果

    void reSetLexTable() { lexicalTable.clear(); };                                       // 清空结果

};

