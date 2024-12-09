#pragma once
#include<string>
#include<vector>
#include<map>
#include <iomanip>
#include<stack>
#include"Lexical.h"
#include"Grammar_while.h"
using namespace std;

const int maxNum = 256;

// ��Ŀ����
enum class termType {
    S,              // �ƽ���Ŀ
    R,              // ��Լ��Ŀ
    UR,             // ��Լ��Ŀ
    acc             // ������Ŀ
};

// LR1
// LR1 ��Ŀ�ṹ��
typedef struct term
{
    termType type;                  // ��Ŀ����
    string leftPart;                // ��Ŀ��
    vector<string> rightPart;       // ��Ŀ�Ҳ�
    int pointPosition{ -1 };               // ǰ׺ . ֮ǰ��λ�ã���Ŀ�Ҳ��У�Ĭ�� -1

    vector<string> searchForwardSymSet;     // LR1�ķ�����Ҫ����ǰ��������

    bool operator==(const term& item) const           // ��д == 
    {
        if (this->leftPart == item.leftPart && this->rightPart == item.rightPart
            && this->pointPosition == item.pointPosition && this->searchForwardSymSet == item.searchForwardSymSet) {
            return true;
        }
        return false;
    }

    friend ostream& operator << (std::ostream& os, term termItem) {        // ��д <<
        os << termItem.leftPart << "->";
        for (int j = 0; j < termItem.rightPart.size(); j++) {
            if (j == termItem.pointPosition)
                os << "��";
            os << termItem.rightPart[j];
        }
        if (termItem.rightPart.size() == termItem.pointPosition) {
            os << "��";
        }
        for (int j = 0; j < termItem.searchForwardSymSet.size(); j++) {
            if (j == 0) {
                os << "," << termItem.searchForwardSymSet[j];
            }
            else {
                os << " " << termItem.searchForwardSymSet[j];
            }
        }
        return os;
    }

}term;

// action�ṹ�� ����actionTable
typedef struct action {
    termType type{ termType::UR };                  // �ƽ�S����ԼR������acc
    int num{ -1 };                  // ��� S��״̬ת��  R���� num �Ų���ʽ��Լ

    friend ostream& operator << (std::ostream& os, action action) {        // ��д <<
        if (action.type == termType::S) {            // �ƽ�
            os << "S" << action.num;
        }
        else if (action.type == termType::R) {       // ��Լ
            os << "r" << action.num;
        }
        else if (action.type == termType::acc) {     // ����
            os << "acc";
        }
        else {
            os << action.num;
        }
        return os;
    }

}action;

// ���Žṹ��
typedef struct symItem
{
    string symName;                 // ������
    string symValue{ "0" };         // �ַ�����ʽ ���ŵ�ֵ
    int position{ -1 };                // �÷����ڷ��ű��е�λ�� Ĭ�� -1
    friend ostream& operator << (std::ostream& os, symItem symbol) {        // ��д <<
        return os << "(\'" << symbol.symName << "\'," << symbol.symValue << "," << symbol.position << ")";
    }
    bool operator==(const symItem& item) const                              // ��д == 
    {
        if (this->symName == item.symName && this->symValue == item.symValue && this->position == item.position) {
            return true;
        }
        return false;
    }
}Symobl;

// ����ַ��ṹ��
typedef struct quaternion
{
    string op;                  // ������
    symItem arg1;               // Դ������1
    symItem arg2;               // Դ������2
    symItem result;             // Ŀ�Ĳ�����
}quaternion;



// �﷨������    
class GrammaticalAnalysis
{
private:
    bool errFlag;           // �쳣��ֹ�ж�
    Grammar grammar;                         // �ķ�(����ʽ��Vn/Vt��first���ϡ�follow����)
    vector<word> lexicalTable;               // �ʷ��������

    /* LR1 ��Ŀ��&& ������ */
    vector<term> termSet[maxNum];           // ��Ŀ������ I0��I1������ ������� maxNum ��I
    int termCount;                          // ����Ŀ������ 
    action actionTable[maxNum][maxNum];     // action��      �� Vt    �� status   
    int gotoTable[maxNum][maxNum];          // goto��        �� Vn    �� status

    /* ������� */
    vector<quaternion> quaSequence;          // ��Ԫʽ����
    vector<symItem> symbolTable;             // ���ű�����������裩
    map<string, int> entryMap;               // ��������ű��ӳ���ϵ  ��������ڵ�ַ
    stack<quaternion> Mstack;

    int tempVarNum;                          // ��ʱ�������� ���Ա�� T0��T1������

    /* ������������Ҫ����ջ */
    stack<int> statusStack;             // ״̬ջ
    stack<symItem> opStack;             // ����ջ
    stack<string> semanticStack;        // ����ջ

private:

    /* LR1 ��Ŀ��&& ������ ���� */
    void initI0();
    int findMergeSet();                 // Ѱ�ҿɺϲ���Ŀ��
    void getClosureSet(int statusNum);                          // ��Ŀ���հ�����
    int getGotoStatus(int statusNum, string symbol);            // ת������
    void printStatus();                         // ��ӡ״̬��
    void printTable();                          // ��ӡ������
    void printAllStack();                       // ������������е�ջ������
    void getTermSetAndAnalysisTable();                          // ������Ŀ���ͷ�����

    /* ������� */
    symItem newtemp();      // new ��ʱ���� T0��T1����
    void GEN(string op, int arg1Index, int arg2Index, int resultIndex);        // ����һ����Ԫʽ����������Ԫʽ���б�

    /* �ķ�����ʽ��Ӧ�����嶯�� */
    void S();                       // S'->S
    void AWhileCB();
    void C_E_R_E();                 // C->E ROP E
    void B_S_B();                   // B->S;B	
    void B_S();                     // B->S;
    void B_A();                     // B->A
    // ��ֵ���
    void S_i_E();                   // S -> i=E     
    void E_minE();                  // E -> -E   ���ʽ �� - ���ʽ
    void E_F();                     // E -> F    ���ʽ �� ��  
    void E_EaddF();                 // E -> E+F  ���ʽ �� ���ʽ - ��
    void E_EsubF();                 // E -> E-F  ���ʽ �� ���ʽ - ��
    void F_G();                     // F -> G    �� �� ����
    void F_FmulG();                 // F -> F*G  �� �� ��*����
    void F_FdivG();                 // F -> F/G  �� �� ��/����
    void G_i();                     // G -> i
    void G_n();                     // G -> n
    void G_E();                     // G -> (E)

    void R_greater();               // ROP-> >
    void R_less();                  // ROP-> <
    void R_eq();                    // ROP-> ==
    void R_not();
    void R_ge();                    // ROP-> >=
    void R_le();                    // ROP-> <=



public:
    GrammaticalAnalysis();
    void setLexicalTable(vector<word> table);                // �����﷨�������
    void grammaticalTranslation();
    void R_and();
    void R_or();
    // �������巭��
    void reSetStack();                  // ��ʼ��ջ
    vector<quaternion> getMidCode() { return quaSequence; };                         // ��ȡ��Ԫʽ�м����
};

