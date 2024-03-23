#pragma once
#include<string>
#include<vector>
using namespace std;


// ������
class word{
public:
    int sym{};            // ͨ���䴫�ݵ������ e.g. "ident"
    string token;         // ���ʵ�ֵ
};

// �������
enum class type {
    del, opStack, res, con, iden, err
};

// �ʷ�������(C++)
class LexicalAnalysis{
private:
    // C++�ؼ��ֱ�(����)
    string CppTable[75];
    // �洢�ʷ��������
    vector<word> lexicalTable;
    
private:
    bool isLetter(char ch);		    // �ж��Ƿ�Ϊ��ĸ
    bool isNumber(char ch);		    // �ж��Ƿ�Ϊ����

    bool isDelimiters(word word);	    // �ж��Ƿ�Ϊ���
    bool isOperator(word word);		    // �ж��Ƿ�Ϊ�����
    bool isReservedWord(word word);	    // �ж��Ƿ�Ϊ�ؼ���
    bool isconstantWord(word word);     // �ж��Ƿ�Ϊ�ǳ���
    bool isIdentifierWord(word word);   // �ж��Ƿ�Ϊ��ʶ��

    word keyWordAndIdAnalysis(const string& str);         // ����ؼ��ֺͱ�ʶ���ķ�������
    word numberAnalysis(const string& str);               // �������ֵķ�������
    word strAnalysis(const string& str);                  // �����ַ������ĺ���
    word charAnalysis(string str);                        // �����ַ��ķ�������

public:
    LexicalAnalysis();
    type getWordType(word word);                                // ��ȡ��������
    void printWord(word item);                                  // ������ʶ�Ԫʽ

    void lexicalAnalysis(const string& inputStr);               // �ʷ�����������
    vector<word> getLexicalTable() { return lexicalTable; };    // ��ȡ�ʷ��������

    void reSetLexTable() { lexicalTable.clear(); };                                       // ��ս��

};

