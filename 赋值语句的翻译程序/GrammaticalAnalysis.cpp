#include<iostream>
#include<string>
#include<queue>
#include "GrammaticalAnalysis.h"
#include"stack.h"
using namespace std;

GrammaticalAnalysis::GrammaticalAnalysis() {
    grammar.initGrammer();
    termCount = 0;
    tempVarNum = 0;
    getTermSetAndAnalysisTable();
}


// 合并项目集：尝试寻找可合并的项目集（最后一个） 若有则返回合并后的下标
int GrammaticalAnalysis::findMergeSet()
{
    int index = -1; // 假定不可以进行合并
    for (int i = 0; i < termCount - 1; i++){
        if (termSet[i].size() == termSet[termCount - 1].size()) {
            index = i; // 判断是否可以合并
            for (int j = 0; j < termSet[termCount - 1].size(); j++){
                // 存在不等项目，当前项目集不可合并
                if (!(termSet[i][j] == termSet[termCount - 1][j])){
                    index = -1;
                }
            }
            if (index != -1) {
                return index;        // 找到可合并项目集，返回下标
            }
        }
    }
    return -1;
}

// 初始化第一项目集 I0
void GrammaticalAnalysis::initI0()
{
    term firstTerm;
    firstTerm.leftPart = grammar.productionTable[0].left;       // 读取文法的第一项产生式    
    firstTerm.rightPart = grammar.productionTable[0].right;
    if (firstTerm.pointPosition == -1)     // 如果没有活前缀"·"
        firstTerm.pointPosition = 0;       // 就添加活前缀
    firstTerm.searchForwardSymSet.push_back("#");
    termSet[0].push_back(firstTerm);              // 添加至I0
    termCount++;
}

// 求项目集的CLOSURE闭包函数（LR1）
void GrammaticalAnalysis::getClosureSet(int statusNum)
{
    queue<term> termQueue;           
    // 将I(index)下的项目依次加入队列
    for (int i = 0; i < termSet[statusNum].size(); i++) {
        termQueue.push(termSet[statusNum][i]);
    }
    // 当队列非空
    while (!termQueue.empty())
    { // 对项集I中每个项[A->a·B β,α]  E -> .F,# + -
        term curTerm = termQueue.front();       // 第一个项目
        // 如果是规约项，跳过并弹出该项
        if (curTerm.type == termType::R){
            termQueue.pop();
            continue;
        }
        string B = curTerm.rightPart[curTerm.pointPosition];       // 符号B F
        // 对文法中的每个产生式 B-> xxxx   F-> ..
        for (int j = 0; j < grammar.productionTable.size(); j++) { 
            // 左部吻合 B
            if (B != grammar.productionTable[j].left){
                continue;
            }
            // 将[B->·xxxx,b]加入集合I中，其中b是FIRST[ βα ]中的终结符
            term newTerm;
            newTerm.pointPosition = 0;
            newTerm.leftPart = grammar.productionTable[j].left;         // 左部
            newTerm.rightPart = grammar.productionTable[j].right;       // 右部
            // 判断右部第一个符号 是 Vt/Vn ?
            if (grammar.Vt.find(newTerm.rightPart[0]) != grammar.Vt.end()) {
                newTerm.type = termType::S;                     // B->·bA，移进项目
            }
            else if (grammar.Vn.find(newTerm.rightPart[0]) != grammar.Vn.end()) {
                newTerm.type = termType::UR;                    // B->b·A, 待约项目
            }
            else {
                cout << "符号 '" + newTerm.rightPart[0] + "' 在文法符号集中不存在\n";
            }
            // 找b(后继符号) from first集合中
            string beta;
            vector<string> b;
            // 如果 β 不存在，b即 α
            if (curTerm.pointPosition == curTerm.rightPart.size() - 1) {
                b = curTerm.searchForwardSymSet;
            }
            else if (grammar.Vt.find(curTerm.rightPart[curTerm.pointPosition + 1]) != grammar.Vt.end())
            { // 如果 β 存在，b为first( β )。特例： β 为终结符时，b就是 β
                b.push_back(curTerm.rightPart[curTerm.pointPosition + 1]);
            }
            else
            { // 非终结符Vn 遍历first( β )中的终结符b
                beta = curTerm.rightPart[curTerm.pointPosition + 1];
                for (auto it = grammar.firstMap[beta].begin(); it != grammar.firstMap[beta].end(); it++) {
                    b.push_back(*it);
                }
            }
            newTerm.searchForwardSymSet = b;
            // 只有闭包生成的新项B->xxxx,不在集合I中才加入；在集合I中的只要相应的增加后继符
            int newTermFlag = -1; // 先假设不在集合I中
            for (int k = 0; k < termSet[statusNum].size(); k++) {
                // 除搜索符集外项目一致
                if (newTerm.leftPart == termSet[statusNum][k].leftPart 
                    && newTerm.rightPart == termSet[statusNum][k].rightPart 
                    && newTerm.pointPosition == termSet[statusNum][k].pointPosition) {
                    newTermFlag = k;
                }
            }
            if (newTermFlag == -1) { // 不在当前集合I中就加入
                termQueue.push(newTerm);
                termSet[statusNum].push_back(newTerm);
            }
            else
            { // 如果新项B->xxxx在集合I中, 就在原先搜索符集上拓充
                map<string, int> searchForwardSymMap;
                for (int m = 0; m < termSet[statusNum][newTermFlag].searchForwardSymSet.size(); m++) {
                    searchForwardSymMap[termSet[statusNum][newTermFlag].searchForwardSymSet[m]]++;             // 存在则记录 +1
                }
                bool flag = false;
                for (int m = 0; m < newTerm.searchForwardSymSet.size(); m++)
                {
                    if (searchForwardSymMap[newTerm.searchForwardSymSet[m]] == 0) {
                        termSet[statusNum][newTermFlag].searchForwardSymSet.push_back(newTerm.searchForwardSymSet[m]);
                        flag = true;                
                    }
                }
                if (flag) {
                    termQueue.push(termSet[statusNum][newTermFlag]);        // 若在集合中且搜索符集合有更新 则重新进入队列（LR1）
                }
            }
        }
        termQueue.pop();            // 当前处理完毕 退栈
    }
}

// GOTO函数-调用closure 由状态集statusNum读入一个符号symbol(vn或vt)，返回转移后的项集编号
int GrammaticalAnalysis::getGotoStatus(int statusNum, string symbol)
{  
    int size = termSet[statusNum].size();
    for (int i = 0; i < size; i++){
        vector<string> right = termSet[statusNum][i].rightPart;       // 右部
        int pointPosition = termSet[statusNum][i].pointPosition;                    // . 位置
        // symbol是vn或者vt都可
        if (pointPosition < right.size() && symbol == right[pointPosition])
        {   // 尚未归约且.后为symbol相同
            term tmpTerm = termSet[statusNum][i];
            tmpTerm.pointPosition = tmpTerm.pointPosition + 1; // 活前缀后移一位
            pointPosition = tmpTerm.pointPosition;
            // 如果后移一位之后发现成为了规约项目,则加入新项目集
            if (tmpTerm.pointPosition == tmpTerm.rightPart.size()){
                tmpTerm.type = termType::R;
                termSet[termCount].push_back(tmpTerm);     
            }      
            else if (grammar.Vt.find(tmpTerm.rightPart[pointPosition]) != grammar.Vt.end()) { 
                // 活前缀不在最后，且紧随着一个终结符(移进项目)，加入新项目
                tmpTerm.type = termType::S;
                termSet[termCount].push_back(tmpTerm);      
            }
            else if (grammar.Vn.find(tmpTerm.rightPart[pointPosition]) != grammar.Vn.end())
            { // 活前缀不在最后，且紧随着一个非终结符(待约项目)，将上一个状态集中的该非终结符产生式加入
                // 先加入 S->B·B
                tmpTerm.type = termType::UR;
                termSet[termCount].push_back(tmpTerm);
                // 再进行闭包计算
                getClosureSet(termCount);
            }
        }
    }
    termCount++;
    int index = findMergeSet();
    if (index != -1) {           // 可合并
        termSet[termCount - 1].clear(); 
        termCount--;
        return index;
    }
    else {
        return termCount - 1;
    }
}

// 输出状态集
void GrammaticalAnalysis::printStatus() 
{
    for (int i = 0; i < termCount; i++)
    {
        cout << "I" << i << ":" << endl;
        for (auto it = termSet[i].begin(); it != termSet[i].end(); it++){
            cout << *it;
            cout << endl;
        }
        cout << endl;
    }
}

// 输出分析表
void GrammaticalAnalysis::printTable()
{    
    cout << " \t";
    for (auto it = grammar.Vt.begin(); it != grammar.Vt.end(); it++) {
        cout << *it << "  \t";
    }
    for (auto it = grammar.Vn.begin(); it != grammar.Vn.end(); it++){ 
        cout << *it << "  \t";
    }
    cout << endl;
    for (int i = 0; i < termCount; i++){
        cout << i << "\t";
        // action部分
        for (auto it = grammar.Vt.begin(); it != grammar.Vt.end(); it++){
            int j = grammar.getVIndex(*it);
            cout << actionTable[i][j] << "\t";
        }
        // goto部分
        for (auto it = grammar.Vn.begin(); it != grammar.Vn.end(); it++) { 
            int y = grammar.getVIndex(*it);
            cout << gotoTable[i][y] << "  \t";
        }   
        cout << endl;
    }
}

// 同步构造项集和分析表
void GrammaticalAnalysis::getTermSetAndAnalysisTable()
{ 
    initI0();               // I0构造
    getClosureSet(0);

    queue<string> symbolQueue;          // 相对项目集即将读入的符号队列
    map<string, int> symbolMap;         // 索引
    int curStatus = 0;                  // 队列中当前项的状态

    for (int i = 0; i < termSet[0].size(); i++){
        string symbolStr = termSet[0][i].rightPart[termSet[0][i].pointPosition];       // I0项目集中 项目右部 .后的 符号
        if (symbolMap[symbolStr] == 0) {        
            // 不在map索引中则push到 队列中
            symbolQueue.push(symbolStr);
            symbolMap[symbolStr]++;
        }
    }
    symbolQueue.push("$$");    // 分割标志 表明 当前项集的移进项和goto都处理完毕

    while (!symbolQueue.empty()){
        if (symbolQueue.front() == "$$") { 
            // 开始处理规约项
            for (int i = 0; i < termSet[curStatus].size(); i++) {
                /* actionTable构造 */
                if (termSet[curStatus][i].type == termType::R) { 
                    // 可规约项 判断该规约项是用哪个产生式规约的
                    term tmpTerm = termSet[curStatus][i];
                    production tmpPro;
                    tmpPro.left = tmpTerm.leftPart;
                    tmpPro.right = tmpTerm.rightPart;
                    // 寻找规约所用的产生式索引
                    int productionIndex = -1;
                    for (int j = 0; j < grammar.productionTable.size(); j++) {
                        if (tmpPro == grammar.productionTable[j])
                            productionIndex = j;
                    }
                    // 接受状态
                    if (productionIndex == 0) {          // 首个产生式 接受
                        actionTable[curStatus][grammar.getVIndex("#")].type = termType::acc;
                    }        
                    else {
                        // LR(1)分析中，只有规约项的后继符才进行规约
                        for (int j = 0; j < tmpTerm.searchForwardSymSet.size(); j++) {
                            actionTable[curStatus][grammar.getVIndex(tmpTerm.searchForwardSymSet[j])] = { termType::R, productionIndex };
                        }
                    }
                }
            }
            curStatus++;
            symbolQueue.pop();
            continue;
        }
        // 获取读入当前符号的下一个状态
        int nextStatus = getGotoStatus(curStatus, symbolQueue.front());
        // 读入非终结符 移入项 
        if (grammar.Vt.find(symbolQueue.front()) != grammar.Vt.end()){
            if (actionTable[curStatus][grammar.getVIndex(symbolQueue.front())].num != -1) {
                cout << "(状态" << curStatus << "移进" << symbolQueue.front() << "存在冲突)";
            }
            actionTable[curStatus][grammar.getVIndex(symbolQueue.front())].type = termType::S;
            actionTable[curStatus][grammar.getVIndex(symbolQueue.front())].num = nextStatus;
        }
        else {
            // 读入非终结符 goto表
            gotoTable[curStatus][grammar.getVIndex(symbolQueue.front())] = nextStatus;
        } 
        // 新状态集的 . 后面的符号(包括vn和vt)入队列 , 若nextStatus是已有的状态集则不用处理
        if (nextStatus == termCount - 1){
            symbolMap.clear();
            for (int i = 0; i < termSet[nextStatus].size(); i++){
                // 规约项就跳过
                if (termSet[nextStatus][i].type != termType::R) {
                    string symbolStr = termSet[nextStatus][i].rightPart[termSet[nextStatus][i].pointPosition];
                    if (symbolMap[symbolStr] == 0) {            // 相同符号只添加一次
                        symbolQueue.push(symbolStr);
                        symbolMap[symbolStr]++;
                    }
                }
            }
            symbolQueue.push("$$"); // 引入分隔项 标记nextStatus的S和goto处理完
        }
        symbolQueue.pop();
    }
    printStatus(); //输出状态项集
    printTable();  //输出分析表
}

// new 临时变量 T0、T1、、、
symItem GrammaticalAnalysis::newtemp(){
    tempVarNum++;
    symItem T;
    T.symName = "T" + to_string(tempVarNum);
    T.position = symbolTable.size();
    symbolTable.push_back(T);
    entryMap[T.symName] = T.position;           // 变量入口地址映射
    return T;
}
// 产生一个四元式，并填入四元式序列表 
void GrammaticalAnalysis::GEN(string op, int arg1Index, int arg2Index, int resultIndex)
{   // 操作符，参数1在符号表的索引，参数2在符号表的索引，结果在符号表中的索引
    // 判断操作符
    try {
        if (op == "neg") { // 取负
            symbolTable[resultIndex].symValue = "-" + symbolTable[arg1Index].symValue;
        }
        else if (op == "pos") {       // 取正     
            symbolTable[resultIndex].symValue = symbolTable[arg1Index].symValue;
        }
        else if (op == "+") {        // +
            symbolTable[resultIndex].symValue = to_string(stoi(symbolTable[arg1Index].symValue) + stoi(symbolTable[arg2Index].symValue));
        }
        else if (op == "-") {        // -
            symbolTable[resultIndex].symValue = to_string(stoi(symbolTable[arg1Index].symValue) - stoi(symbolTable[arg2Index].symValue));
        }
        else if (op == "*") {        // *
            symbolTable[resultIndex].symValue = to_string(stoi(symbolTable[arg1Index].symValue) * stoi(symbolTable[arg2Index].symValue));
        }
        else if (op == "/") {       // /
            if (stoi(symbolTable[arg2Index].symValue) == 0) {
                cout << "0xC0000094: Integer division by zero。" << endl;            // 除 0 err
                errFlag = true;
                return;
            }
            symbolTable[resultIndex].symValue = to_string(stoi(symbolTable[arg1Index].symValue) / stoi(symbolTable[arg2Index].symValue));
        }
        if (op == "=") {            // =
            symbolTable[resultIndex].symValue = symbolTable[arg1Index].symValue;   // 赋值
        }
    }
    catch (int e) {
        cout << "e" << "calcuate err" << endl;
    }
    // 输出四元式
    cout << "(" << op << ",";
    arg1Index != -1 ? cout << symbolTable[arg1Index].symName : cout << " ";
    cout << ",";
    arg2Index != -1 ? cout << symbolTable[arg2Index].symName : cout << " ";
    cout << "," << symbolTable[resultIndex].symName << ")" << endl;
    // 插入到四元式序列中
    quaSequence.push_back(
        quaternion{ op,
        arg1Index == -1 ? symItem{ " " } : symbolTable[arg1Index],
        arg2Index == -1 ? symItem{ " " } : symbolTable[arg2Index],
        symbolTable[resultIndex] });
 
}

void GrammaticalAnalysis::setLexicalTable(vector<word> table) {
    lexicalTable = table;
}

void GrammaticalAnalysis::printAllStack() {
    
}

// S' -> S { print(S) }
void GrammaticalAnalysis::S() {
    cout <<"result: " << symbolTable[entryMap[opStack.top().symName]].symName << " = " << symbolTable[entryMap[opStack.top().symName]].symValue << endl;
}

// S -> i=E 
void GrammaticalAnalysis::S_i_E() {
    symItem E, i;
    // 符号栈、状态栈退栈（产生式右部规约长度）
    for (int popTime = 0; popTime < 3; popTime++)
    {
        if (popTime == 0)
            E = opStack.top();
        if (popTime == 2)
            i = opStack.top();
        opStack.pop();           // op
        statusStack.pop();       // status
    }
    // 左部进入操作栈
    opStack.push(symItem{ grammar.productionTable[1].left });

    int curStatus = statusStack.top();           // 当前状态
    statusStack.push(gotoTable[curStatus][grammar.getVIndex(opStack.top().symName)]);        // 执行goto

    // 执行语义动作
    semanticStack.pop();                 // 语义栈退栈（E)
    i.symName = semanticStack.top();
    semanticStack.pop();
    GEN("=", E.position, -1, entryMap[i.symName]);       // 产生四元式 因 变量 i 在push进opStack的时候未指定.position,故使用entryMap
}

// E -> -E   表达式 → +/- 表达式
void GrammaticalAnalysis::E_minE() {
    symItem E1 = opStack.top();
    int popNum = grammar.productionTable[2].right.size();
    while (popNum)
    {
        statusStack.pop();
        opStack.pop();
        popNum--;
    }
    // 把"E"push进符号栈，但还没和语义栈关联，下面进行
    opStack.push(symItem{ grammar.productionTable[2].left });
    int curStatus = statusStack.top();
    statusStack.push(gotoTable[curStatus][grammar.getVIndex(opStack.top().symName)]);

    symItem T = newtemp();
    GEN("neg", E1.position, -1, T.position);
    semanticStack.pop(); // 更新语义栈
    semanticStack.push(T.symName);
    opStack.top().position = T.position;
    opStack.top().symValue = T.symValue;
}

// E -> F    表达式 → 项 
void GrammaticalAnalysis::E_F() {
    symItem E1;
    E1 = opStack.top();
    opStack.pop();
    statusStack.pop();
    // 把"F"push进符号栈，但还没和语义栈关联，下面进行
    opStack.push(symItem{ grammar.productionTable[3].left });
    int curStatus = statusStack.top();
    statusStack.push(gotoTable[curStatus][grammar.getVIndex(opStack.top().symName)]);

    opStack.top().position = entryMap[semanticStack.top()];
}

// E -> E+-F  表达式 → 表达式 +/- 项
void GrammaticalAnalysis::E_EaddF() {
    symItem E1, E2;
    for (int popTime = 0; popTime < 3; popTime++)
    { // E->E + E 要出栈三次，进栈一次
        if (popTime == 0)
            E2 = opStack.top();
        if (popTime == 2)
            E1 = opStack.top();
        opStack.pop();
        statusStack.pop();
    }
    opStack.push(symItem{ grammar.productionTable[4].left }); // 把"E"push进符号栈，但还没和语义栈关联，下面进行
    int curStatus = statusStack.top();
    statusStack.push(gotoTable[curStatus][grammar.getVIndex(opStack.top().symName)]);

    symItem T = newtemp();
    GEN("+", E1.position, E2.position, T.position);
    semanticStack.pop();                  // 更新语义栈
    semanticStack.pop();
    semanticStack.push(T.symName);
    opStack.top().position = T.position;
    opStack.top().symValue = T.symValue;
}

// E -> E-F  表达式 → 表达式 - 项
void GrammaticalAnalysis::E_EsubF() {
    // 分析栈操作
    symItem E1, E2;
    // 出栈三次 E - F
    for (int popTime = 0; popTime < 3; popTime++)
    {
        if (popTime == 0)
            E2 = opStack.top();
        if (popTime == 2)
            E1 = opStack.top();
        opStack.pop();
        statusStack.pop();
    }
    opStack.push(symItem{ grammar.productionTable[5].left }); // 把"E"push进符号栈，但还没和语义栈关联，下面进行
    int curStatus = statusStack.top();
    statusStack.push(gotoTable[curStatus][grammar.getVIndex(opStack.top().symName)]);

    // 同步执行相应的语义动作
    symItem T = newtemp();
    GEN("-", E1.position, E2.position, T.position);
    semanticStack.pop();                  // 更新语义栈
    semanticStack.pop();
    semanticStack.push(T.symName);
    opStack.top().position = T.position;
    opStack.top().symValue = T.symValue;
}


// F -> G    项 → 因子
void GrammaticalAnalysis::F_G() {
    symItem E1;
    E1 = opStack.top();
    opStack.pop();
    statusStack.pop();
    // 把"F"push进符号栈，但还没和语义栈关联，下面进行
    opStack.push(symItem{ grammar.productionTable[6].left });
    int curStatus = statusStack.top();
    statusStack.push(gotoTable[curStatus][grammar.getVIndex(opStack.top().symName)]);

    opStack.top().position = entryMap[semanticStack.top()];
}

// F -> F*/G  项 → 项*/因子
void GrammaticalAnalysis::F_FmulG() {
    symItem E1, E2;
    for (int popTime = 0; popTime < 3; popTime++)
    { // E->E + E 要出栈三次，进栈一次
        if (popTime == 0)
            E2 = opStack.top();
        if (popTime == 2)
            E1 = opStack.top();
        opStack.pop();
        statusStack.pop();
    }
    opStack.push(symItem{ grammar.productionTable[7].left }); // 把"E"push进符号栈，但还没和语义栈关联，下面进行
    int curStatus = statusStack.top();
    statusStack.push(gotoTable[curStatus][grammar.getVIndex(opStack.top().symName)]);

    symItem T = newtemp();
    GEN("*", E1.position, E2.position, T.position);
    semanticStack.pop();                  // 更新语义栈
    semanticStack.pop();
    semanticStack.push(T.symName);
    opStack.top().position = T.position;
    opStack.top().symValue = T.symValue;
}

// F -> F*/G  项 → 项*/因子
void GrammaticalAnalysis::F_FdivG() {
    symItem E1, E2;
    for (int popTime = 0; popTime < 3; popTime++)
    { // E->E + E 要出栈三次，进栈一次
        if (popTime == 0)
            E2 = opStack.top();
        if (popTime == 2)
            E1 = opStack.top();
        opStack.pop();
        statusStack.pop();
    }
    opStack.push(symItem{ grammar.productionTable[8].left }); // 把"E"push进符号栈，但还没和语义栈关联，下面进行
    int curStatus = statusStack.top();
    statusStack.push(gotoTable[curStatus][grammar.getVIndex(opStack.top().symName)]);

    symItem T = newtemp();
    try {
        GEN("/", E1.position, E2.position, T.position);
    }
    catch (exception e) {
        cout << e.what();
    }
    semanticStack.pop();                  // 更新语义栈
    semanticStack.pop();
    semanticStack.push(T.symName);
    opStack.top().position = T.position;
    opStack.top().symValue = T.symValue;
}
// G -> i
void GrammaticalAnalysis::G_i() {
    int popNum = grammar.productionTable[9].right.size();
    while (popNum) {
        statusStack.pop();
        opStack.pop();
        popNum--;
    }
    // 把"G"push进符号栈，但还没和语义栈关联，下面进行
    opStack.push(symItem{ grammar.productionTable[9].left });
    int curStatus = statusStack.top();
    statusStack.push(gotoTable[curStatus][grammar.getVIndex(opStack.top().symName)]);
    // G->i规约,将E与语义栈栈顶值关联之后
    opStack.top().position = entryMap[semanticStack.top()];
}

// G -> n
void GrammaticalAnalysis::G_n() {
    int popNum = grammar.productionTable[10].right.size();
    while (popNum){
        statusStack.pop();
        opStack.pop();
        popNum--;
    }
    // 把"G"push进符号栈，但还没和语义栈关联，下面进行
    opStack.push(symItem{ grammar.productionTable[10].left }); 
    int curStatus = statusStack.top();
    statusStack.push(gotoTable[curStatus][grammar.getVIndex(opStack.top().symName)]);
    // G->n规约,将E与语义栈栈顶值关联之后
    opStack.top().position = entryMap[semanticStack.top()];
}

// G -> (E)
void GrammaticalAnalysis::G_E() {
    symItem E1;
    for (int popTime = 0; popTime < 3; popTime++) {
        if (popTime == 1)
            E1 = opStack.top();
        opStack.pop();
        statusStack.pop();
    }
    // 把"G"push进符号栈，但还没和语义栈关联，下面进行
    opStack.push(symItem{ grammar.productionTable[11].left });
    int curStatus = statusStack.top();
    statusStack.push(gotoTable[curStatus][grammar.getVIndex(opStack.top().symName)]);

    opStack.top().position = entryMap[semanticStack.top()];
}

//// A -> +
//void GrammaticalAnalysis::A_addOp() {
//    int popNum = grammar.productionTable[10].right.size();
//    while (popNum) {
//        statusStack.pop();
//        opStack.pop();
//        popNum--;
//    }
//    // 把"A"push进符号栈，但还没和语义栈关联，下面进行
//    opStack.push(symItem{ grammar.productionTable[10].left });
//    int curStatus = statusStack.top();
//    statusStack.push(gotoTable[curStatus][grammar.getVIndex(opStack.top().symName)]);
//    // A-> +规约, 将A与语义栈栈顶值关联之后
//    opStack.top().position = ENTRY[semanticStack.top()];
//}
//
//// A -> -
//void GrammaticalAnalysis::A_subOp() {
//    int popNum = grammar.productionTable[11].right.size();
//    while (popNum){
//        statusStack.pop();
//        opStack.pop();
//        popNum--;
//    }
//    // 把"A"push进符号栈，但还没和语义栈关联，下面进行
//    opStack.push(symItem{ grammar.productionTable[11].left });
//    int curStatus = statusStack.top();
//    statusStack.push(gotoTable[curStatus][grammar.getVIndex(opStack.top().symName)]);
//    // A-> -规约, 将A与语义栈栈顶值关联之后
//    opStack.top().position = ENTRY[semanticStack.top()];
//}
//
//// B -> *
//void GrammaticalAnalysis::B_mulOp() {
//    int popNum = grammar.productionTable[12].right.size();
//    while (popNum){
//        statusStack.pop();
//        opStack.pop();
//        popNum--;
//    }
//    // 把"B"push进符号栈，但还没和语义栈关联，下面进行
//    opStack.push(symItem{ grammar.productionTable[12].left }); 
//    int curStatus = statusStack.top();
//    statusStack.push(gotoTable[curStatus][grammar.getVIndex(opStack.top().symName)]);
//    // B->*规约, 将B与语义栈栈顶值关联之后
//    opStack.top().position = ENTRY[semanticStack.top()];
//}
//
//// B -> /
//void GrammaticalAnalysis::B_divOp() {
//    int popNum = grammar.productionTable[13].right.size();
//    while (popNum){
//        statusStack.pop();
//        opStack.pop();
//        popNum--;
//    }
//    // 把"B"push进符号栈，但还没和语义栈关联，下面进行
//    opStack.push(symItem{ grammar.productionTable[13].left }); 
//    int curStatus = statusStack.top();
//    statusStack.push(gotoTable[curStatus][grammar.getVIndex(opStack.top().symName)]);
//    // B->/规约, 将B与语义栈栈顶值关联之后
//    opStack.top().position = ENTRY[semanticStack.top()];
//}

// 初始化栈
void GrammaticalAnalysis::reSetStack() {
    while (!opStack.empty()) {
        opStack.pop();
    }
    while (!statusStack.empty()) {
        statusStack.pop();
    }
    while (!semanticStack.empty()) {
        semanticStack.pop();
    }
    lexicalTable.clear();
    quaSequence.clear();
    symbolTable.clear();
    entryMap.clear();
    tempVarNum = 0;
    errFlag = false;
}

// 进行LR1翻译
void GrammaticalAnalysis::grammaticalTranslation()
{
    int pointer = 0;                  // 输入串指针
    statusStack.push(0);              // 状态栈初始化
    opStack.push(symItem{ "#" });     // 符号栈初始化,{name、value、place}
    // 直到opStack为空
    while (!opStack.empty()&&!errFlag){
        int curStatus = statusStack.top();                      // 当前状态
        word curWord = lexicalTable[pointer];                // 指针指向的单词
        string curSymbol = curWord.token;      // 读头符号
       
        // 变量(标识符)为i，数值为n
        if (curWord.sym == 0)  {          // 0是变量
            curSymbol = "i";
            semanticStack.push(curWord.token); // 语义栈
            // 将语义符号加入符号表，并添加入口地址映射
            symItem tempSym; 
            tempSym.symName = curWord.token;
            tempSym.symValue = "0";             // 由于并未进行表达式右部变量的初始赋值 在此默认声明变量时赋值为 0;
            tempSym.position = symbolTable.size();
            symbolTable.push_back(tempSym);
            entryMap[tempSym.symName] = tempSym.position;
        }
        else if (curWord.sym == 1 || curWord.sym == 2) {        // 数值 ，1是整数，2是实数
            curSymbol = "n";
            semanticStack.push(curWord.token);    // 语义栈
            // 将符号加入符号表，并添加入口地址映射
            symItem tempSym; 
            tempSym.symName = curWord.token;
            tempSym.symValue = curWord.token;     // 数值常量 其值为本身
            tempSym.position = symbolTable.size();
            symbolTable.push_back(tempSym);
            entryMap[tempSym.symName] = tempSym.position;
        }
        if (grammar.getVIndex(curSymbol) == -1) {           // 若符号不存在于文法，则报错
            cout << "err of:"<<curSymbol << ", 文法符号不存在" << endl;
            return;
        }
        // 读取分析表 
        action curAction = actionTable[curStatus][grammar.getVIndex(curSymbol)];
        if (curAction.type == termType::S) { 
            // 移进 由上一个状态读入一个终结符转入新状态 
            statusStack.push(curAction.num);
            opStack.push(symItem{ curSymbol });
            pointer++;
        }
        else if (curAction.type == termType::R) {
            // 规约
            int reduceGrammaIndex = curAction.num;
            try {
                switch (reduceGrammaIndex) {
                case 1:             // S -> i=E (执行语义动作)
                    // E_i_E();
                    S_i_E();
                    break;
                case 2:            
                    // E_EaddE();     // E -> E+E
                    E_minE();           // E-> -E
                    break;
                case 3:             
                    // E_EsubE();      // E -> E-E
                    E_F();              // E-> F
                    break;
                case 4:             
                    // E_EmulE();      // E -> E*E
                    E_EaddF();          // E->E+F
                    break;
                case 5:            
                    // E_EdivE();       // E -> E/E
                    E_EsubF();          // E->E-F
                    break;
                case 6:             
                    // E_minusE();     // E-> -E
                    F_G();              // E->G
                    break;
                case 7:             
                    // E_E();          // E-> (E)
                    F_FmulG();          // E->F*G
                    break;
                case 8:             
                    // E_i();          // E-> i
                    F_FdivG();          // E->F/G
                    break;
                case 9:            
                    // E_n();           // E-> n
                    G_i();              // G->i
                    break;
                case 10:
                    G_n();              // G->n
                    break;
                case 11:
                    G_E();              // G->(E)
                    break;
                //case 12:
                //    B_mulOp();
                //    break;
                //case 13:
                //    B_divOp();
                //    break;
                default:
                    throw("err: out of production Index！");
                }
            }
            catch (exception e) {
                cout << e.what() << endl;
            }
        }
        else if (curAction.type == termType::acc){ 
            // 接受状态
            S();
            statusStack.pop();
            opStack.pop();
            statusStack.pop();
            opStack.pop();
        }
        else {
            cout << "分析错误，赋值表达式有误" << endl;
            return;
        }
    }
}
