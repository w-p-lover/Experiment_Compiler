#pragma once
#include<set>
using namespace std;

// 产生式
typedef struct production {
    string left;                    // 左部(一个非终结符)
    vector<string> right;           // 右部(多个Vn/Vt)

    // 判断两个产生式是否相等
    bool operator==(const production& pro)
    {
        if (this->left == pro.left && this->right.size() == pro.right.size()) {
            for (int i = 0; i < right.size(); i++) {
                if (this->right[i] != pro.right[i]) {
                    return false;
                }
            }
            return true;
        }
        return false;
    }
}production;


// 文法类 （产生式、终结符、非终结符、first和follow集合）
class Grammar {
// 为便于后续在词法分析和语法分析中使用 此处均为public
public:
    vector<production> productionTable;            // 存储产生式
    
    set<string> Vn;                                // 非终结符集
    set<string> Vt;                                // 终结符集

    map<string, set<string>> firstMap;             // first集合   Vn/Vt : {first} 
    map<string, set<string>> followMap;            // follow集合  Vn/Vt : {follow}

public:
    int getVIndex(string V);                        // 返回某个符号的索引位置（in firstMap）
    void initGrammer();                             // 文法初始化函数
};
