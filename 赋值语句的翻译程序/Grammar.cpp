#include<vector>
#include<map>
#include<string>
#include<set>
#include<vector>
#include<iostream>
#include "Grammar.h"
using namespace std;

void Grammar::initGrammer() {
    // �򵥸�ֵ����ķ�����ʽ
    production item = { "S'", {"S"} };
    productionTable.push_back(item);
    item.left = "S";
    item.right = {"i", "=", "E"};
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

    // ��ʾ�������ʽ
    for (int i = 0; i < productionTable.size(); i++) {
        cout << productionTable[i].left << "->";
        for (int j = 0; j < productionTable[i].right.size(); j++) {
            cout << productionTable[i].right[j];
        }
        cout << endl;
    }
    // ���ս��Vn��first���ϲ��루�ɸ����㷨�Զ����㣩A B E F G S
    Vn.insert("E");
    Vn.insert("F");
    Vn.insert("G");
    Vn.insert("S");
    Vn.insert("S'");
    firstMap.insert(map<string, set<string>>::value_type("E", { "(","-","i","n"}));
    firstMap.insert(map<string, set<string>>::value_type("F", { "i", "n", "(" }));
    firstMap.insert(map<string, set<string>>::value_type("G", { "i", "n", "(" }));
    firstMap.insert(map<string, set<string>>::value_type("S", { "i" }));
    firstMap.insert(map<string, set<string>>::value_type("S'", { "i" }));
    // �ս��Vt��first���ϣ�����i = n ( ) + - * /
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

}

// ��ȡ��firstMap�е�����λ��
int Grammar::getVIndex(string V) {
    if (firstMap.find(V) != firstMap.end()) {
        return distance(firstMap.find(V), firstMap.end());      // ��������
    }
    return -1;          // ���Ų�����
}
