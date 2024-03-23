#include<iostream>
#include"Lexical.h"
using namespace std;

// ��ʼ���ʷ���������Ҫ�Ĺؼ��ֱ�
LexicalAnalysis::LexicalAnalysis() {
    CppTable[0] = "��ʶ��(����)";         // ��ʶ�� 0    ���� 1~3
    CppTable[1] = "����";
    CppTable[2] = "ʵ��";
    CppTable[3] = "�ַ�����";
    CppTable[4] = "+";                   // ����� 4~19 
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
    CppTable[20] = "#";                  // ���index 20~29
    CppTable[21] = "(";
    CppTable[22] = ")";
    CppTable[23] = "[";
    CppTable[24] = "]";
    CppTable[25] = "{";
    CppTable[26] = "}";
    CppTable[27] = ":";
    CppTable[28] = ";";
    CppTable[29] = ",";
    CppTable[30] = "void";               // �ؼ��� 30~46
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

// �ж��Ƿ�Ϊ��ĸ
bool LexicalAnalysis::isLetter(char ch)
{
    if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
    {
        return true;
    }
    return false;

}
// �Ƿ�Ϊ����
bool LexicalAnalysis::isNumber(char ch)
{
    if (ch >= '0' && ch <= '9')
    {
        return true;
    }
    return false;
}

// �ж��Ƿ�Ϊ���
bool LexicalAnalysis::isDelimiters(word word)
{
    if (word.sym >= 20 && word.sym <= 29) {
        return true;
    }
    return false;
};
// �ж��Ƿ�Ϊ�����
bool LexicalAnalysis::isOperator(word word)
{
    if (word.sym >= 4 && word.sym <= 19) {
        return true;
    }
    return false;
};
// �ж��Ƿ�Ϊ�ؼ���
bool LexicalAnalysis::isReservedWord(word word)
{
    if (word.sym >= 30 && word.sym <= 46) {
        return true;
    }
    return false;
};
// �ж��Ƿ�Ϊ�ǳ���
bool LexicalAnalysis::isconstantWord(word word)
{
    if (word.sym >= 1 && word.sym <= 3) {
        return true;
    }
    return false;
};
// �ж��Ƿ�Ϊ��ʶ��
bool LexicalAnalysis::isIdentifierWord(word word)
{
    if (word.sym == 0) {
        return true;
    }
    return false;
};

// ����ؼ��ֺͱ�ʶ��
word LexicalAnalysis::keyWordAndIdAnalysis(const string& str)
{
    word item;
    bool isKeyword = false;
    // �ڹؼ��ʱ���Ѱ��
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
        // ������������ؼ��ʣ���Ϊ��ʶ��
        for (int i = 0; i <= str.length(); i++)
        { // �ҵ���һ���������֡���ĸ���»��ߵ�λ��
            if (!(isLetter(str[i]) || isNumber(str[i]) || str[i] == '_'))
            {
                item.sym = 0;       // Cpp�ؼ��ʱ����±� 0 ��ʾ��ʶ��
                item.token = str.substr(0, i);
                break;
            }
        }
    }

    return item;
}

// �������ֵĺ���
word LexicalAnalysis::numberAnalysis(const string& str)
{
    word item;
    for (int i = 0; i <= str.length(); ++i)
    {
        // ��ȡ����һ�������ֺ�С�����ַ�
        if (!(isNumber(str[i]) || str[i] == '.'))
        {
            string curNum = str.substr(0, i);
            if (curNum.find('.') == string::npos) // û����'.'����ֵΪ�ܴ����������������ֵ�ǵ�һ�γ��ֵ��±�
                item.sym = 1;           // 1 ����
            else
                item.sym = 2;           // 2 ʵ��λ��
            item.token = curNum;

            break;
        }
    }

    return item;
}

// �����ַ������ĺ���
word LexicalAnalysis::strAnalysis(const string& str)
{
    word item;
    int nextindex = str.find_first_of('"', 1);     // �ҵ��ڶ���"���ֵ�λ���±�
    item.sym = 3;                                  // 3 �ַ�������λ��
    item.token = str.substr(0, nextindex + 1);

    return item;
}

// �����ַ��ĺ���
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
        item.sym = -1;      // �Ƿ��ַ�
    }
    if (item.sym != -1) {
        item.token = CppTable[item.sym];
    }
    else {
        item.token = str[0];        // ��¼�Ƿ��ַ�
    }
    return item;
}

// ��ȡ��������
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

// ������ʶ�Ԫʽ
void LexicalAnalysis::printWord(word item) {
    switch (getWordType(item)) {
    case type::opStack:
        cout << "( ������, ";
        break;
    case type::del:
        cout << "( ���, ";
        break;
    case type::res:
        cout << "( �ؼ���, ";
        break;
    case type::con:
        cout << "( ����, ";
        break;
    case type::iden:
        cout << "( ��ʶ��, ";
        break;
    default:
        cout << "err input: ";
    }
    cout << item.sym << " , " << item.token << " )" << endl;
}

// �ʷ�����������
void LexicalAnalysis::lexicalAnalysis(const string& inputStr)
{
    for (int i = 0; i < inputStr.length(); ++i)
    {
        word item;
        if (inputStr[i] == ' '||inputStr[i] == '\t'|| inputStr[i] == '\n')
        {
            // ����ǿո� �س� ָ�����ֱ������
            continue;
        }
        else if (isLetter(inputStr[i]))
        {
            // ������
            item = keyWordAndIdAnalysis(inputStr.substr(i, inputStr.length() - i + 1));
        }
        else if (isNumber(inputStr[i]))
        {
            // ��������
            item = numberAnalysis(inputStr.substr(i, inputStr.length() - i + 1));
        }
        else if (inputStr[i] == '"') {
            // �����ַ�������
            item = strAnalysis(inputStr.substr(i, inputStr.length() - i + 1));
        }
        else
        {
            // �����������
            item = charAnalysis(inputStr.substr(i, inputStr.length() - i + 1));
        }
        i += item.token.length() - 1;
        lexicalTable.push_back(item);       // �ʷ�������Ľ������lexicalTable��

        printWord(item);        // ����ʷ�������� if need
    }
}