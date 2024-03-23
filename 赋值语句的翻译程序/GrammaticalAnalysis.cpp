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


// �ϲ���Ŀ��������Ѱ�ҿɺϲ�����Ŀ�������һ���� �����򷵻غϲ�����±�
int GrammaticalAnalysis::findMergeSet()
{
    int index = -1; // �ٶ������Խ��кϲ�
    for (int i = 0; i < termCount - 1; i++){
        if (termSet[i].size() == termSet[termCount - 1].size()) {
            index = i; // �ж��Ƿ���Ժϲ�
            for (int j = 0; j < termSet[termCount - 1].size(); j++){
                // ���ڲ�����Ŀ����ǰ��Ŀ�����ɺϲ�
                if (!(termSet[i][j] == termSet[termCount - 1][j])){
                    index = -1;
                }
            }
            if (index != -1) {
                return index;        // �ҵ��ɺϲ���Ŀ���������±�
            }
        }
    }
    return -1;
}

// ��ʼ����һ��Ŀ�� I0
void GrammaticalAnalysis::initI0()
{
    term firstTerm;
    firstTerm.leftPart = grammar.productionTable[0].left;       // ��ȡ�ķ��ĵ�һ�����ʽ    
    firstTerm.rightPart = grammar.productionTable[0].right;
    if (firstTerm.pointPosition == -1)     // ���û�л�ǰ׺"��"
        firstTerm.pointPosition = 0;       // ����ӻ�ǰ׺
    firstTerm.searchForwardSymSet.push_back("#");
    termSet[0].push_back(firstTerm);              // �����I0
    termCount++;
}

// ����Ŀ����CLOSURE�հ�������LR1��
void GrammaticalAnalysis::getClosureSet(int statusNum)
{
    queue<term> termQueue;           
    // ��I(index)�µ���Ŀ���μ������
    for (int i = 0; i < termSet[statusNum].size(); i++) {
        termQueue.push(termSet[statusNum][i]);
    }
    // �����зǿ�
    while (!termQueue.empty())
    { // ���I��ÿ����[A->a��B ��,��]  E -> .F,# + -
        term curTerm = termQueue.front();       // ��һ����Ŀ
        // ����ǹ�Լ���������������
        if (curTerm.type == termType::R){
            termQueue.pop();
            continue;
        }
        string B = curTerm.rightPart[curTerm.pointPosition];       // ����B F
        // ���ķ��е�ÿ������ʽ B-> xxxx   F-> ..
        for (int j = 0; j < grammar.productionTable.size(); j++) { 
            // ���Ǻ� B
            if (B != grammar.productionTable[j].left){
                continue;
            }
            // ��[B->��xxxx,b]���뼯��I�У�����b��FIRST[ �¦� ]�е��ս��
            term newTerm;
            newTerm.pointPosition = 0;
            newTerm.leftPart = grammar.productionTable[j].left;         // ��
            newTerm.rightPart = grammar.productionTable[j].right;       // �Ҳ�
            // �ж��Ҳ���һ������ �� Vt/Vn ?
            if (grammar.Vt.find(newTerm.rightPart[0]) != grammar.Vt.end()) {
                newTerm.type = termType::S;                     // B->��bA���ƽ���Ŀ
            }
            else if (grammar.Vn.find(newTerm.rightPart[0]) != grammar.Vn.end()) {
                newTerm.type = termType::UR;                    // B->b��A, ��Լ��Ŀ
            }
            else {
                cout << "���� '" + newTerm.rightPart[0] + "' ���ķ����ż��в�����\n";
            }
            // ��b(��̷���) from first������
            string beta;
            vector<string> b;
            // ��� �� �����ڣ�b�� ��
            if (curTerm.pointPosition == curTerm.rightPart.size() - 1) {
                b = curTerm.searchForwardSymSet;
            }
            else if (grammar.Vt.find(curTerm.rightPart[curTerm.pointPosition + 1]) != grammar.Vt.end())
            { // ��� �� ���ڣ�bΪfirst( �� )�������� �� Ϊ�ս��ʱ��b���� ��
                b.push_back(curTerm.rightPart[curTerm.pointPosition + 1]);
            }
            else
            { // ���ս��Vn ����first( �� )�е��ս��b
                beta = curTerm.rightPart[curTerm.pointPosition + 1];
                for (auto it = grammar.firstMap[beta].begin(); it != grammar.firstMap[beta].end(); it++) {
                    b.push_back(*it);
                }
            }
            newTerm.searchForwardSymSet = b;
            // ֻ�бհ����ɵ�����B->xxxx,���ڼ���I�вż��룻�ڼ���I�е�ֻҪ��Ӧ�����Ӻ�̷�
            int newTermFlag = -1; // �ȼ��費�ڼ���I��
            for (int k = 0; k < termSet[statusNum].size(); k++) {
                // ��������������Ŀһ��
                if (newTerm.leftPart == termSet[statusNum][k].leftPart 
                    && newTerm.rightPart == termSet[statusNum][k].rightPart 
                    && newTerm.pointPosition == termSet[statusNum][k].pointPosition) {
                    newTermFlag = k;
                }
            }
            if (newTermFlag == -1) { // ���ڵ�ǰ����I�оͼ���
                termQueue.push(newTerm);
                termSet[statusNum].push_back(newTerm);
            }
            else
            { // �������B->xxxx�ڼ���I��, ����ԭ�������������س�
                map<string, int> searchForwardSymMap;
                for (int m = 0; m < termSet[statusNum][newTermFlag].searchForwardSymSet.size(); m++) {
                    searchForwardSymMap[termSet[statusNum][newTermFlag].searchForwardSymSet[m]]++;             // �������¼ +1
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
                    termQueue.push(termSet[statusNum][newTermFlag]);        // ���ڼ������������������и��� �����½�����У�LR1��
                }
            }
        }
        termQueue.pop();            // ��ǰ������� ��ջ
    }
}

// GOTO����-����closure ��״̬��statusNum����һ������symbol(vn��vt)������ת�ƺ������
int GrammaticalAnalysis::getGotoStatus(int statusNum, string symbol)
{  
    int size = termSet[statusNum].size();
    for (int i = 0; i < size; i++){
        vector<string> right = termSet[statusNum][i].rightPart;       // �Ҳ�
        int pointPosition = termSet[statusNum][i].pointPosition;                    // . λ��
        // symbol��vn����vt����
        if (pointPosition < right.size() && symbol == right[pointPosition])
        {   // ��δ��Լ��.��Ϊsymbol��ͬ
            term tmpTerm = termSet[statusNum][i];
            tmpTerm.pointPosition = tmpTerm.pointPosition + 1; // ��ǰ׺����һλ
            pointPosition = tmpTerm.pointPosition;
            // �������һλ֮���ֳ�Ϊ�˹�Լ��Ŀ,���������Ŀ��
            if (tmpTerm.pointPosition == tmpTerm.rightPart.size()){
                tmpTerm.type = termType::R;
                termSet[termCount].push_back(tmpTerm);     
            }      
            else if (grammar.Vt.find(tmpTerm.rightPart[pointPosition]) != grammar.Vt.end()) { 
                // ��ǰ׺��������ҽ�����һ���ս��(�ƽ���Ŀ)����������Ŀ
                tmpTerm.type = termType::S;
                termSet[termCount].push_back(tmpTerm);      
            }
            else if (grammar.Vn.find(tmpTerm.rightPart[pointPosition]) != grammar.Vn.end())
            { // ��ǰ׺��������ҽ�����һ�����ս��(��Լ��Ŀ)������һ��״̬���еĸ÷��ս������ʽ����
                // �ȼ��� S->B��B
                tmpTerm.type = termType::UR;
                termSet[termCount].push_back(tmpTerm);
                // �ٽ��бհ�����
                getClosureSet(termCount);
            }
        }
    }
    termCount++;
    int index = findMergeSet();
    if (index != -1) {           // �ɺϲ�
        termSet[termCount - 1].clear(); 
        termCount--;
        return index;
    }
    else {
        return termCount - 1;
    }
}

// ���״̬��
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

// ���������
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
        // action����
        for (auto it = grammar.Vt.begin(); it != grammar.Vt.end(); it++){
            int j = grammar.getVIndex(*it);
            cout << actionTable[i][j] << "\t";
        }
        // goto����
        for (auto it = grammar.Vn.begin(); it != grammar.Vn.end(); it++) { 
            int y = grammar.getVIndex(*it);
            cout << gotoTable[i][y] << "  \t";
        }   
        cout << endl;
    }
}

// ͬ��������ͷ�����
void GrammaticalAnalysis::getTermSetAndAnalysisTable()
{ 
    initI0();               // I0����
    getClosureSet(0);

    queue<string> symbolQueue;          // �����Ŀ����������ķ��Ŷ���
    map<string, int> symbolMap;         // ����
    int curStatus = 0;                  // �����е�ǰ���״̬

    for (int i = 0; i < termSet[0].size(); i++){
        string symbolStr = termSet[0][i].rightPart[termSet[0][i].pointPosition];       // I0��Ŀ���� ��Ŀ�Ҳ� .��� ����
        if (symbolMap[symbolStr] == 0) {        
            // ����map��������push�� ������
            symbolQueue.push(symbolStr);
            symbolMap[symbolStr]++;
        }
    }
    symbolQueue.push("$$");    // �ָ��־ ���� ��ǰ����ƽ����goto���������

    while (!symbolQueue.empty()){
        if (symbolQueue.front() == "$$") { 
            // ��ʼ�����Լ��
            for (int i = 0; i < termSet[curStatus].size(); i++) {
                /* actionTable���� */
                if (termSet[curStatus][i].type == termType::R) { 
                    // �ɹ�Լ�� �жϸù�Լ�������ĸ�����ʽ��Լ��
                    term tmpTerm = termSet[curStatus][i];
                    production tmpPro;
                    tmpPro.left = tmpTerm.leftPart;
                    tmpPro.right = tmpTerm.rightPart;
                    // Ѱ�ҹ�Լ���õĲ���ʽ����
                    int productionIndex = -1;
                    for (int j = 0; j < grammar.productionTable.size(); j++) {
                        if (tmpPro == grammar.productionTable[j])
                            productionIndex = j;
                    }
                    // ����״̬
                    if (productionIndex == 0) {          // �׸�����ʽ ����
                        actionTable[curStatus][grammar.getVIndex("#")].type = termType::acc;
                    }        
                    else {
                        // LR(1)�����У�ֻ�й�Լ��ĺ�̷��Ž��й�Լ
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
        // ��ȡ���뵱ǰ���ŵ���һ��״̬
        int nextStatus = getGotoStatus(curStatus, symbolQueue.front());
        // ������ս�� ������ 
        if (grammar.Vt.find(symbolQueue.front()) != grammar.Vt.end()){
            if (actionTable[curStatus][grammar.getVIndex(symbolQueue.front())].num != -1) {
                cout << "(״̬" << curStatus << "�ƽ�" << symbolQueue.front() << "���ڳ�ͻ)";
            }
            actionTable[curStatus][grammar.getVIndex(symbolQueue.front())].type = termType::S;
            actionTable[curStatus][grammar.getVIndex(symbolQueue.front())].num = nextStatus;
        }
        else {
            // ������ս�� goto��
            gotoTable[curStatus][grammar.getVIndex(symbolQueue.front())] = nextStatus;
        } 
        // ��״̬���� . ����ķ���(����vn��vt)����� , ��nextStatus�����е�״̬�����ô���
        if (nextStatus == termCount - 1){
            symbolMap.clear();
            for (int i = 0; i < termSet[nextStatus].size(); i++){
                // ��Լ�������
                if (termSet[nextStatus][i].type != termType::R) {
                    string symbolStr = termSet[nextStatus][i].rightPart[termSet[nextStatus][i].pointPosition];
                    if (symbolMap[symbolStr] == 0) {            // ��ͬ����ֻ���һ��
                        symbolQueue.push(symbolStr);
                        symbolMap[symbolStr]++;
                    }
                }
            }
            symbolQueue.push("$$"); // ����ָ��� ���nextStatus��S��goto������
        }
        symbolQueue.pop();
    }
    printStatus(); //���״̬�
    printTable();  //���������
}

// new ��ʱ���� T0��T1������
symItem GrammaticalAnalysis::newtemp(){
    tempVarNum++;
    symItem T;
    T.symName = "T" + to_string(tempVarNum);
    T.position = symbolTable.size();
    symbolTable.push_back(T);
    entryMap[T.symName] = T.position;           // ������ڵ�ַӳ��
    return T;
}
// ����һ����Ԫʽ����������Ԫʽ���б� 
void GrammaticalAnalysis::GEN(string op, int arg1Index, int arg2Index, int resultIndex)
{   // ������������1�ڷ��ű������������2�ڷ��ű������������ڷ��ű��е�����
    // �жϲ�����
    try {
        if (op == "neg") { // ȡ��
            symbolTable[resultIndex].symValue = "-" + symbolTable[arg1Index].symValue;
        }
        else if (op == "pos") {       // ȡ��     
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
                cout << "0xC0000094: Integer division by zero��" << endl;            // �� 0 err
                errFlag = true;
                return;
            }
            symbolTable[resultIndex].symValue = to_string(stoi(symbolTable[arg1Index].symValue) / stoi(symbolTable[arg2Index].symValue));
        }
        if (op == "=") {            // =
            symbolTable[resultIndex].symValue = symbolTable[arg1Index].symValue;   // ��ֵ
        }
    }
    catch (int e) {
        cout << "e" << "calcuate err" << endl;
    }
    // �����Ԫʽ
    cout << "(" << op << ",";
    arg1Index != -1 ? cout << symbolTable[arg1Index].symName : cout << " ";
    cout << ",";
    arg2Index != -1 ? cout << symbolTable[arg2Index].symName : cout << " ";
    cout << "," << symbolTable[resultIndex].symName << ")" << endl;
    // ���뵽��Ԫʽ������
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
    // ����ջ��״̬ջ��ջ������ʽ�Ҳ���Լ���ȣ�
    for (int popTime = 0; popTime < 3; popTime++)
    {
        if (popTime == 0)
            E = opStack.top();
        if (popTime == 2)
            i = opStack.top();
        opStack.pop();           // op
        statusStack.pop();       // status
    }
    // �󲿽������ջ
    opStack.push(symItem{ grammar.productionTable[1].left });

    int curStatus = statusStack.top();           // ��ǰ״̬
    statusStack.push(gotoTable[curStatus][grammar.getVIndex(opStack.top().symName)]);        // ִ��goto

    // ִ�����嶯��
    semanticStack.pop();                 // ����ջ��ջ��E)
    i.symName = semanticStack.top();
    semanticStack.pop();
    GEN("=", E.position, -1, entryMap[i.symName]);       // ������Ԫʽ �� ���� i ��push��opStack��ʱ��δָ��.position,��ʹ��entryMap
}

// E -> -E   ���ʽ �� +/- ���ʽ
void GrammaticalAnalysis::E_minE() {
    symItem E1 = opStack.top();
    int popNum = grammar.productionTable[2].right.size();
    while (popNum)
    {
        statusStack.pop();
        opStack.pop();
        popNum--;
    }
    // ��"E"push������ջ������û������ջ�������������
    opStack.push(symItem{ grammar.productionTable[2].left });
    int curStatus = statusStack.top();
    statusStack.push(gotoTable[curStatus][grammar.getVIndex(opStack.top().symName)]);

    symItem T = newtemp();
    GEN("neg", E1.position, -1, T.position);
    semanticStack.pop(); // ��������ջ
    semanticStack.push(T.symName);
    opStack.top().position = T.position;
    opStack.top().symValue = T.symValue;
}

// E -> F    ���ʽ �� �� 
void GrammaticalAnalysis::E_F() {
    symItem E1;
    E1 = opStack.top();
    opStack.pop();
    statusStack.pop();
    // ��"F"push������ջ������û������ջ�������������
    opStack.push(symItem{ grammar.productionTable[3].left });
    int curStatus = statusStack.top();
    statusStack.push(gotoTable[curStatus][grammar.getVIndex(opStack.top().symName)]);

    opStack.top().position = entryMap[semanticStack.top()];
}

// E -> E+-F  ���ʽ �� ���ʽ +/- ��
void GrammaticalAnalysis::E_EaddF() {
    symItem E1, E2;
    for (int popTime = 0; popTime < 3; popTime++)
    { // E->E + E Ҫ��ջ���Σ���ջһ��
        if (popTime == 0)
            E2 = opStack.top();
        if (popTime == 2)
            E1 = opStack.top();
        opStack.pop();
        statusStack.pop();
    }
    opStack.push(symItem{ grammar.productionTable[4].left }); // ��"E"push������ջ������û������ջ�������������
    int curStatus = statusStack.top();
    statusStack.push(gotoTable[curStatus][grammar.getVIndex(opStack.top().symName)]);

    symItem T = newtemp();
    GEN("+", E1.position, E2.position, T.position);
    semanticStack.pop();                  // ��������ջ
    semanticStack.pop();
    semanticStack.push(T.symName);
    opStack.top().position = T.position;
    opStack.top().symValue = T.symValue;
}

// E -> E-F  ���ʽ �� ���ʽ - ��
void GrammaticalAnalysis::E_EsubF() {
    // ����ջ����
    symItem E1, E2;
    // ��ջ���� E - F
    for (int popTime = 0; popTime < 3; popTime++)
    {
        if (popTime == 0)
            E2 = opStack.top();
        if (popTime == 2)
            E1 = opStack.top();
        opStack.pop();
        statusStack.pop();
    }
    opStack.push(symItem{ grammar.productionTable[5].left }); // ��"E"push������ջ������û������ջ�������������
    int curStatus = statusStack.top();
    statusStack.push(gotoTable[curStatus][grammar.getVIndex(opStack.top().symName)]);

    // ͬ��ִ����Ӧ�����嶯��
    symItem T = newtemp();
    GEN("-", E1.position, E2.position, T.position);
    semanticStack.pop();                  // ��������ջ
    semanticStack.pop();
    semanticStack.push(T.symName);
    opStack.top().position = T.position;
    opStack.top().symValue = T.symValue;
}


// F -> G    �� �� ����
void GrammaticalAnalysis::F_G() {
    symItem E1;
    E1 = opStack.top();
    opStack.pop();
    statusStack.pop();
    // ��"F"push������ջ������û������ջ�������������
    opStack.push(symItem{ grammar.productionTable[6].left });
    int curStatus = statusStack.top();
    statusStack.push(gotoTable[curStatus][grammar.getVIndex(opStack.top().symName)]);

    opStack.top().position = entryMap[semanticStack.top()];
}

// F -> F*/G  �� �� ��*/����
void GrammaticalAnalysis::F_FmulG() {
    symItem E1, E2;
    for (int popTime = 0; popTime < 3; popTime++)
    { // E->E + E Ҫ��ջ���Σ���ջһ��
        if (popTime == 0)
            E2 = opStack.top();
        if (popTime == 2)
            E1 = opStack.top();
        opStack.pop();
        statusStack.pop();
    }
    opStack.push(symItem{ grammar.productionTable[7].left }); // ��"E"push������ջ������û������ջ�������������
    int curStatus = statusStack.top();
    statusStack.push(gotoTable[curStatus][grammar.getVIndex(opStack.top().symName)]);

    symItem T = newtemp();
    GEN("*", E1.position, E2.position, T.position);
    semanticStack.pop();                  // ��������ջ
    semanticStack.pop();
    semanticStack.push(T.symName);
    opStack.top().position = T.position;
    opStack.top().symValue = T.symValue;
}

// F -> F*/G  �� �� ��*/����
void GrammaticalAnalysis::F_FdivG() {
    symItem E1, E2;
    for (int popTime = 0; popTime < 3; popTime++)
    { // E->E + E Ҫ��ջ���Σ���ջһ��
        if (popTime == 0)
            E2 = opStack.top();
        if (popTime == 2)
            E1 = opStack.top();
        opStack.pop();
        statusStack.pop();
    }
    opStack.push(symItem{ grammar.productionTable[8].left }); // ��"E"push������ջ������û������ջ�������������
    int curStatus = statusStack.top();
    statusStack.push(gotoTable[curStatus][grammar.getVIndex(opStack.top().symName)]);

    symItem T = newtemp();
    try {
        GEN("/", E1.position, E2.position, T.position);
    }
    catch (exception e) {
        cout << e.what();
    }
    semanticStack.pop();                  // ��������ջ
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
    // ��"G"push������ջ������û������ջ�������������
    opStack.push(symItem{ grammar.productionTable[9].left });
    int curStatus = statusStack.top();
    statusStack.push(gotoTable[curStatus][grammar.getVIndex(opStack.top().symName)]);
    // G->i��Լ,��E������ջջ��ֵ����֮��
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
    // ��"G"push������ջ������û������ջ�������������
    opStack.push(symItem{ grammar.productionTable[10].left }); 
    int curStatus = statusStack.top();
    statusStack.push(gotoTable[curStatus][grammar.getVIndex(opStack.top().symName)]);
    // G->n��Լ,��E������ջջ��ֵ����֮��
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
    // ��"G"push������ջ������û������ջ�������������
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
//    // ��"A"push������ջ������û������ջ�������������
//    opStack.push(symItem{ grammar.productionTable[10].left });
//    int curStatus = statusStack.top();
//    statusStack.push(gotoTable[curStatus][grammar.getVIndex(opStack.top().symName)]);
//    // A-> +��Լ, ��A������ջջ��ֵ����֮��
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
//    // ��"A"push������ջ������û������ջ�������������
//    opStack.push(symItem{ grammar.productionTable[11].left });
//    int curStatus = statusStack.top();
//    statusStack.push(gotoTable[curStatus][grammar.getVIndex(opStack.top().symName)]);
//    // A-> -��Լ, ��A������ջջ��ֵ����֮��
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
//    // ��"B"push������ջ������û������ջ�������������
//    opStack.push(symItem{ grammar.productionTable[12].left }); 
//    int curStatus = statusStack.top();
//    statusStack.push(gotoTable[curStatus][grammar.getVIndex(opStack.top().symName)]);
//    // B->*��Լ, ��B������ջջ��ֵ����֮��
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
//    // ��"B"push������ջ������û������ջ�������������
//    opStack.push(symItem{ grammar.productionTable[13].left }); 
//    int curStatus = statusStack.top();
//    statusStack.push(gotoTable[curStatus][grammar.getVIndex(opStack.top().symName)]);
//    // B->/��Լ, ��B������ջջ��ֵ����֮��
//    opStack.top().position = ENTRY[semanticStack.top()];
//}

// ��ʼ��ջ
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

// ����LR1����
void GrammaticalAnalysis::grammaticalTranslation()
{
    int pointer = 0;                  // ���봮ָ��
    statusStack.push(0);              // ״̬ջ��ʼ��
    opStack.push(symItem{ "#" });     // ����ջ��ʼ��,{name��value��place}
    // ֱ��opStackΪ��
    while (!opStack.empty()&&!errFlag){
        int curStatus = statusStack.top();                      // ��ǰ״̬
        word curWord = lexicalTable[pointer];                // ָ��ָ��ĵ���
        string curSymbol = curWord.token;      // ��ͷ����
       
        // ����(��ʶ��)Ϊi����ֵΪn
        if (curWord.sym == 0)  {          // 0�Ǳ���
            curSymbol = "i";
            semanticStack.push(curWord.token); // ����ջ
            // ��������ż�����ű��������ڵ�ַӳ��
            symItem tempSym; 
            tempSym.symName = curWord.token;
            tempSym.symValue = "0";             // ���ڲ�δ���б��ʽ�Ҳ������ĳ�ʼ��ֵ �ڴ�Ĭ����������ʱ��ֵΪ 0;
            tempSym.position = symbolTable.size();
            symbolTable.push_back(tempSym);
            entryMap[tempSym.symName] = tempSym.position;
        }
        else if (curWord.sym == 1 || curWord.sym == 2) {        // ��ֵ ��1��������2��ʵ��
            curSymbol = "n";
            semanticStack.push(curWord.token);    // ����ջ
            // �����ż�����ű��������ڵ�ַӳ��
            symItem tempSym; 
            tempSym.symName = curWord.token;
            tempSym.symValue = curWord.token;     // ��ֵ���� ��ֵΪ����
            tempSym.position = symbolTable.size();
            symbolTable.push_back(tempSym);
            entryMap[tempSym.symName] = tempSym.position;
        }
        if (grammar.getVIndex(curSymbol) == -1) {           // �����Ų��������ķ����򱨴�
            cout << "err of:"<<curSymbol << ", �ķ����Ų�����" << endl;
            return;
        }
        // ��ȡ������ 
        action curAction = actionTable[curStatus][grammar.getVIndex(curSymbol)];
        if (curAction.type == termType::S) { 
            // �ƽ� ����һ��״̬����һ���ս��ת����״̬ 
            statusStack.push(curAction.num);
            opStack.push(symItem{ curSymbol });
            pointer++;
        }
        else if (curAction.type == termType::R) {
            // ��Լ
            int reduceGrammaIndex = curAction.num;
            try {
                switch (reduceGrammaIndex) {
                case 1:             // S -> i=E (ִ�����嶯��)
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
                    throw("err: out of production Index��");
                }
            }
            catch (exception e) {
                cout << e.what() << endl;
            }
        }
        else if (curAction.type == termType::acc){ 
            // ����״̬
            S();
            statusStack.pop();
            opStack.pop();
            statusStack.pop();
            opStack.pop();
        }
        else {
            cout << "�������󣬸�ֵ���ʽ����" << endl;
            return;
        }
    }
}
