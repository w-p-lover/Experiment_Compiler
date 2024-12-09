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

// 项目类型
enum class termType {
    S,              // 移进项目
    R,              // 规约项目
    UR,             // 待约项目
    acc             // 接收项目
};

// LR1
// LR1 项目结构体
typedef struct term
{
    termType type;                  // 项目类型
    string leftPart;                // 项目左部
    vector<string> rightPart;       // 项目右部
    int pointPosition{ -1 };               // 前缀 . 之前的位置（项目右部中）默认 -1

    vector<string> searchForwardSymSet;     // LR1文法中需要的向前搜索符集

    bool operator==(const term& item) const           // 重写 == 
    {
        if (this->leftPart == item.leftPart && this->rightPart == item.rightPart
            && this->pointPosition == item.pointPosition && this->searchForwardSymSet == item.searchForwardSymSet) {
            return true;
        }
        return false;
    }

    friend ostream& operator << (std::ostream& os, term termItem) {        // 重写 <<
        os << termItem.leftPart << "->";
        for (int j = 0; j < termItem.rightPart.size(); j++) {
            if (j == termItem.pointPosition)
                os << "·";
            os << termItem.rightPart[j];
        }
        if (termItem.rightPart.size() == termItem.pointPosition) {
            os << "·";
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

// action结构体 用于actionTable
typedef struct action {
    termType type{ termType::UR };                  // 移进S、规约R、接收acc
    int num{ -1 };                  // 编号 S：状态转跳  R：用 num 号产生式规约

    friend ostream& operator << (std::ostream& os, action action) {        // 重写 <<
        if (action.type == termType::S) {            // 移进
            os << "S" << action.num;
        }
        else if (action.type == termType::R) {       // 规约
            os << "r" << action.num;
        }
        else if (action.type == termType::acc) {     // 接受
            os << "acc";
        }
        else {
            os << action.num;
        }
        return os;
    }

}action;

// 符号结构体
typedef struct symItem
{
    string symName;                 // 符号名
    string symValue{ "0" };         // 字符串形式 符号的值
    int position{ -1 };                // 该符号在符号表中的位置 默认 -1
    friend ostream& operator << (std::ostream& os, symItem symbol) {        // 重写 <<
        return os << "(\'" << symbol.symName << "\'," << symbol.symValue << "," << symbol.position << ")";
    }
    bool operator==(const symItem& item) const                              // 重写 == 
    {
        if (this->symName == item.symName && this->symValue == item.symValue && this->position == item.position) {
            return true;
        }
        return false;
    }
}Symobl;

// 三地址码结构体
typedef struct quaternion
{
    string op;                  // 操作符
    symItem arg1;               // 源操作数1
    symItem arg2;               // 源操作数2
    symItem result;             // 目的操作数
}quaternion;



// 语法分析类    
class GrammaticalAnalysis
{
private:
    bool errFlag;           // 异常中止判断
    Grammar grammar;                         // 文法(产生式、Vn/Vt、first集合、follow集合)
    vector<word> lexicalTable;               // 词法分析结果

    /* LR1 项目集&& 分析表 */
    vector<term> termSet[maxNum];           // 项目集数组 I0、I1、、、 最多容纳 maxNum 个I
    int termCount;                          // 总项目集数量 
    action actionTable[maxNum][maxNum];     // action表      列 Vt    行 status   
    int gotoTable[maxNum][maxNum];          // goto表        列 Vn    行 status

    /* 语义计算 */
    vector<quaternion> quaSequence;          // 四元式序列
    vector<symItem> symbolTable;             // 符号表（语义计算所需）
    map<string, int> entryMap;               // 符号与符号表的映射关系  即符号入口地址
    stack<quaternion> Mstack;

    int tempVarNum;                          // 临时变量个数 用以标记 T0、T1、、、

    /* 分析过程所需要三个栈 */
    stack<int> statusStack;             // 状态栈
    stack<symItem> opStack;             // 符号栈
    stack<string> semanticStack;        // 语义栈

private:

    /* LR1 项目集&& 分析表 构造 */
    void initI0();
    int findMergeSet();                 // 寻找可合并项目集
    void getClosureSet(int statusNum);                          // 项目集闭包函数
    int getGotoStatus(int statusNum, string symbol);            // 转换函数
    void printStatus();                         // 打印状态集
    void printTable();                          // 打印分析表
    void printAllStack();                       // 输出分析过程中的栈中内容
    void getTermSetAndAnalysisTable();                          // 构造项目集和分析表

    /* 语义计算 */
    symItem newtemp();      // new 临时变量 T0、T1、、
    void GEN(string op, int arg1Index, int arg2Index, int resultIndex);        // 产生一个四元式，并填入四元式序列表

    /* 文法产生式对应的语义动作 */
    void S();                       // S'->S
    void AWhileCB();
    void C_E_R_E();                 // C->E ROP E
    void B_S_B();                   // B->S;B	
    void B_S();                     // B->S;
    void B_A();                     // B->A
    // 赋值语句
    void S_i_E();                   // S -> i=E     
    void E_minE();                  // E -> -E   表达式 → - 表达式
    void E_F();                     // E -> F    表达式 → 项  
    void E_EaddF();                 // E -> E+F  表达式 → 表达式 - 项
    void E_EsubF();                 // E -> E-F  表达式 → 表达式 - 项
    void F_G();                     // F -> G    项 → 因子
    void F_FmulG();                 // F -> F*G  项 → 项*因子
    void F_FdivG();                 // F -> F/G  项 → 项/因子
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
    void setLexicalTable(vector<word> table);                // 设置语法分析结果
    void grammaticalTranslation();
    void R_and();
    void R_or();
    // 进行语义翻译
    void reSetStack();                  // 初始化栈
    vector<quaternion> getMidCode() { return quaSequence; };                         // 获取四元式中间代码
};

