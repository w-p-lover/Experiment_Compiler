#include<vector>
#include<map>
#include<string>
#include<set>
#include<vector>
#include<iostream>
#include "Grammar.h"
using namespace std;

void Grammar::initGrammer() {
    // 简单赋值语句文法产生式
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

    // 显示输入产生式
    for (int i = 0; i < productionTable.size(); i++) {
        cout << productionTable[i].left << "->";
        for (int j = 0; j < productionTable[i].right.size(); j++) {
            cout << productionTable[i].right[j];
        }
        cout << endl;
    }
    // 非终结符Vn的first集合插入（可改用算法自动计算）A B E F G S
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
    // 终结符Vt的first集合（自身）i = n ( ) + - * /
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

// 获取在firstMap中的索引位置
int Grammar::getVIndex(string V) {
    if (firstMap.find(V) != firstMap.end()) {
        return distance(firstMap.find(V), firstMap.end());      // 返回索引
    }
    return -1;          // 符号不存在
}
