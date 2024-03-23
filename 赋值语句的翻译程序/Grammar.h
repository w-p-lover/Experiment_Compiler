#pragma once
#include<set>
using namespace std;

// ����ʽ
typedef struct production {
    string left;                    // ��(һ�����ս��)
    vector<string> right;           // �Ҳ�(���Vn/Vt)

    // �ж���������ʽ�Ƿ����
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


// �ķ��� ������ʽ���ս�������ս����first��follow���ϣ�
class Grammar {
// Ϊ���ں����ڴʷ��������﷨������ʹ�� �˴���Ϊpublic
public:
    vector<production> productionTable;            // �洢����ʽ
    
    set<string> Vn;                                // ���ս����
    set<string> Vt;                                // �ս����

    map<string, set<string>> firstMap;             // first����   Vn/Vt : {first} 
    map<string, set<string>> followMap;            // follow����  Vn/Vt : {follow}

public:
    int getVIndex(string V);                        // ����ĳ�����ŵ�����λ�ã�in firstMap��
    void initGrammer();                             // �ķ���ʼ������
};
