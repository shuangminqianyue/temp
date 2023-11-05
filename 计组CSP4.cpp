#include <bits/stdc++.h>
using namespace std;
typedef struct node {
    int id;         // 编号
    int block;      // 区
    int nums;       // 当前的任务数
    set<int> task;  // 所运行任务的编号

    bool operator<(const node& a) const {
        if (this->nums != a.nums) {
            return this->nums < a.nums;
        } else {
            return this->id < a.id;
        }
    }
} node;

unordered_map<int, node> Node;

// 编号为n的可用区内的计算节点
void fax1(int n, vector<int>& temp) {
    for (auto it = temp.begin(); it != temp.end();) {
        if (Node[*it].block != n) {
            temp.erase(it);
        } else
            it++;
    }
    return;
}

// 和编号为n的应用的计算任务在同一个可用区
void fax2(int n, vector<int>& temp) {
    set<int> qu;
    for (auto it = Node.begin(); it != Node.end(); it++) {
        if (it->second.task.count(n) == 1) {
            qu.insert(it->second.block);
        }
    }
    for (auto it = temp.begin(); it != temp.end();) {
        if (qu.count(Node[*it].block) == 0) {
            temp.erase(it);
        } else
            it++;
    }
    return;
}

// 表示不能和编号为n的应用的计算任务在同一个计算节点上运行
vector<int> fax3(int n, vector<int> temp) {
    for (auto it = temp.begin(); it != temp.end();) {
        if (Node[*it].task.count(n) == 1) {
            temp.erase(it);
        } else
            it++;
    }
    return temp;
}

int main() {
#ifdef inline
    freopen("stdin.txt", "r", stdin);
#endif
    ios::sync_with_stdio(false);
    int n, m;
    cin >> n >> m;  // n个节点，m个区
    int i = 1;

    vector<int> idsum;
    while (i <= n) {
        int temp;
        cin >> temp;
        Node.insert(pair<int, node>(i, node{i, temp, 0, set<int>()}));
        idsum.push_back(i);
        i++;
    }
    int T;
    cin >> T;
    while (T--) {
        int a1, a2, a3, a4, a5, a6;
        cin >> a1 >> a2 >> a3 >> a4 >> a5 >> a6;

        // #ifdef inline
        // 	for(auto &i:temp){
        // 		cout<<i<<" ";
        // 	}
        // 	cout<<"*************"<<endl;
        // #endif

        while (a1--) {
            vector<int> temp = idsum, temp2;
            // 计算节点亲和性
            if (a3 != 0) {
                fax1(a3, temp);
            }
            // 计算任务亲和性
            if (a4 != 0) {
                fax2(a4, temp);
            }
            // 计算任务反亲和性
            if (a5 != 0) {
                temp2 = fax3(a5, temp);
            }
            // 计算任务反亲和性要求是必须满足的且无可用节点   或 不考虑
            // 计算任务反亲和性也无可用节点
            if ((temp2.empty() && a6 == 1) || temp.empty()) {
                cout << 0 << " ";
                continue;
            }
            // 计算任务反亲和性要求是尽量满足的 且 无可用节点   则 不考虑
            // 计算任务反亲和性
            else if (temp2.empty() && a6 == 0) {
                temp2 = temp;
            }
            // 排序 按照节点的已有的作业数、序号
            sort(temp2.begin(), temp2.end(),
                 [](const auto& a, const auto& b) -> bool {
                     return Node[a] < Node[b];
                 });
            // 更新节点信息
            Node[temp2[0]].nums++;
            Node[temp2[0]].task.insert(a2);
            cout << Node[temp2[0]].id << " ";
        }
        cout << endl;
    }
    return 0;
}