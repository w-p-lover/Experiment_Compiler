#include "Grammar_while.h"
using namespace std;

void Grammar::initGrammer() {
    // while
    production item = { "S'", {"A"} };
    productionTable.push_back(item);
    item.left = "A";
    item.right = { "while", "(", "C", ")","{", "B", "}"};
    productionTable.push_back(item);
    item.left = "C";
    item.right = { "E" ,"ROP","E" };
    productionTable.push_back(item);
    item.left = "B";
    item.right = { "S" ,";","B" };
    productionTable.push_back(item);
    item.left = "B";
    item.right = { "S" ,";" };
    productionTable.push_back(item);
    item.left = "B";
    item.right = { "A" };
    productionTable.push_back(item);
    // �򵥸�ֵ����ķ�����ʽ
    item.left = "S";
    item.right = { "i", "=", "E" };
    productionTable.push_back(item);
    item.left = "E";
    item.right = { "-","E" };
    productionTable.push_back(item);
    item.left = "E";
    item.right = { "F" };
    productionTable.push_back(item);
    item.left = "E";
    item.right = { "E","+","F" };
    productionTable.push_back(item);
    item.left = "E";
    item.right = { "E","-","F" };
    productionTable.push_back(item);
    item.left = "F";
    item.right = { "G" };
    productionTable.push_back(item);
    item.left = "F";
    item.right = { "F","*","G" };
    productionTable.push_back(item);
    item.left = "F";
    item.right = { "F","/","G" };
    productionTable.push_back(item);
    item.left = "G";
    item.right = { "i" };
    productionTable.push_back(item);
    item.left = "G";
    item.right = { "n" };
    productionTable.push_back(item);
    item.left = "G";
    item.right = { "(","E",")" };
    productionTable.push_back(item);
    item.left = "ROP";
    item.right = { ">" };
    productionTable.push_back(item);
    item.left = "ROP";
    item.right = { "<" };
    productionTable.push_back(item);
    item.left = "ROP";
    item.right = { "==" };
    productionTable.push_back(item);
    item.left = "ROP";
    item.right = { ">=" };
    productionTable.push_back(item);
    item.left = "ROP";
    item.right = { "<=" };
    productionTable.push_back(item);
    // ��ʾ�������ʽ
    for (int i = 0; i < productionTable.size(); i++) {
        cout << productionTable[i].left << "->";
        for (int j = 0; j < productionTable[i].right.size(); j++) {
            cout << productionTable[i].right[j]<<" ";
        }
        cout << endl;
    }
    // ���ս��Vn��first���ϲ��루�ɸ����㷨�Զ����㣩A B C S E F G ROP
    Vn.insert("S'");
    Vn.insert("A");
    Vn.insert("B");
    Vn.insert("C");
    Vn.insert("S");
    Vn.insert("E");
    Vn.insert("F");
    Vn.insert("G");
    Vn.insert("ROP");

    firstMap.insert(map<string, set<string>>::value_type("S'", { "while" }));
    firstMap.insert(map<string, set<string>>::value_type("A", { "while" }));
    firstMap.insert(map<string, set<string>>::value_type("B", { "while", "i"}));
    firstMap.insert(map<string, set<string>>::value_type("C", { "(","-","i","n" }));
    firstMap.insert(map<string, set<string>>::value_type("S", { "i" }));
    firstMap.insert(map<string, set<string>>::value_type("E", { "(","-","i","n" }));
    firstMap.insert(map<string, set<string>>::value_type("F", { "i", "n", "(" }));
    firstMap.insert(map<string, set<string>>::value_type("G", { "i", "n", "(" }));
    firstMap.insert(map<string, set<string>>::value_type("ROP", { ">","<","==",">=","<=" }));

    // �ս��Vt��first���ϣ�����i = n ( ) + - * /  while { } ; > < >= <= ==
    Vt.insert("i");
    Vt.insert("=");
    Vt.insert("n");
    Vt.insert("(");
    Vt.insert(")");
    Vt.insert("+");
    Vt.insert("-");
    Vt.insert("*");
    Vt.insert("/");
    Vt.insert("#");

    Vt.insert("while");
    Vt.insert("{");
    Vt.insert("}");
    Vt.insert(";");
    Vt.insert(">");
    Vt.insert(">=");
    Vt.insert("<");
    Vt.insert("<=");
    Vt.insert("==");
    firstMap.insert(map<string, set<string>>::value_type("i", { "i" }));
    firstMap.insert(map<string, set<string>>::value_type("=", { "=" }));
    firstMap.insert(map<string, set<string>>::value_type("n", { "n" }));
    firstMap.insert(map<string, set<string>>::value_type("(", { "(" }));
    firstMap.insert(map<string, set<string>>::value_type(")", { ")" }));
    firstMap.insert(map<string, set<string>>::value_type("+", { "+" }));
    firstMap.insert(map<string, set<string>>::value_type("-", { "-" }));
    firstMap.insert(map<string, set<string>>::value_type("*", { "*" }));
    firstMap.insert(map<string, set<string>>::value_type("/", { "/" }));
    firstMap.insert(map<string, set<string>>::value_type("#", { "#" }));

    firstMap.insert(map<string, set<string>>::value_type("while", { "while" }));
    firstMap.insert(map<string, set<string>>::value_type("{", { "{" }));
    firstMap.insert(map<string, set<string>>::value_type("}", { "}" }));
    firstMap.insert(map<string, set<string>>::value_type(";", { ";" }));
    firstMap.insert(map<string, set<string>>::value_type(">", { ">" }));
    firstMap.insert(map<string, set<string>>::value_type(">=", { ">=" }));
    firstMap.insert(map<string, set<string>>::value_type("<", { "<=" }));
    firstMap.insert(map<string, set<string>>::value_type("<=", { "<=" }));
    firstMap.insert(map<string, set<string>>::value_type("==", { "==" }));

}

// ��ȡ��firstMap�е�����λ��
int Grammar::getVIndex(string V) {
    if (firstMap.find(V) != firstMap.end()) {
        return distance(firstMap.find(V), firstMap.end());      // ��������
    }
    return -1;          // ���Ų�����
}
