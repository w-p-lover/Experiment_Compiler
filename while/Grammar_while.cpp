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
    // 简单赋值语句文法产生式
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
    // 显示输入产生式
    for (int i = 0; i < productionTable.size(); i++) {
        cout << productionTable[i].left << "->";
        for (int j = 0; j < productionTable[i].right.size(); j++) {
            cout << productionTable[i].right[j]<<" ";
        }
        cout << endl;
    }
    // 非终结符Vn的first集合插入（可改用算法自动计算）A B C S E F G ROP
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

    // 终结符Vt的first集合（自身）i = n ( ) + - * /  while { } ; > < >= <= ==
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

// 获取在firstMap中的索引位置
int Grammar::getVIndex(string V) {
    if (firstMap.find(V) != firstMap.end()) {
        return distance(firstMap.find(V), firstMap.end());      // 返回索引
    }
    return -1;          // 符号不存在
}
