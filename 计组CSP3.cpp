#include <string.h>
#include <algorithm>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>
using namespace std;
// 结构体---逻辑器件
struct device {
    int id;                  // 序号
    int level;               // 层级
    int output;              // 该器件输出
    string type;             // 器件类型
    bool ct;                 // 是否构造好该器件
    vector<int> input;       // 器件输入
    vector<int> id_flag;     // 标识
    vector<string> input_s;  // 输入字符串
    // 初始化
    device() {
        level = -1;
        ct = false;
    }
    // 每个器件的运算
    compute() {
        int result = input[0];
        if (type == "NOT")
            result = !input[0];
        else if (type == "AND") {
            for (int i = 1; i < input.size(); i++) {
                result &= input[i];
            }
        } else if (type == "OR") {
            for (int i = 1; i < input.size(); i++) {
                result |= input[i];
            }
        } else if (type == "XOR") {
            for (int i = 1; i < input.size(); i++) {
                result ^= input[i];
            }
        } else if (type == "NAND") {
            for (int i = 1; i < input.size(); i++) {
                result &= input[i];
            }
            result = !result;
        } else {
            for (int i = 1; i < input.size(); i++) {
                result |= input[i];
            }
            result = !result;
        }
        output = result;
    }
};
// string to int
int str_to_int(string s) {
    int result;
    string str = s.substr(1);  // 截取I/O后面的数字字符串
    stringstream ss;
    ss << str;
    ss >> result;
    return result;
}
// sort函数的cmp，定义器件层级由小到大排列
bool cmp(device d1, device d2) {
    return d1.level < d2.level;
}

int main() {
    int qt_num;  // 问题数量
    cin >> qt_num;
    for (int i = 0; i < qt_num; i++) {
        // 存储输入的第一部分---器件构造
        int in_num, d_num;  // 输入数量、输出/器件数量
        cin >> in_num >> d_num;
        device dce[d_num];
        for (int j = 0; j < d_num; j++) {
            int in_size;
            dce[j].id = j + 1;
            cin >> dce[j].type >> in_size;  // 器件类型及其输入的个数
            for (int k = 0; k < in_size; k++) {
                string i_o;
                cin >> i_o;
                dce[j].id_flag.push_back(0);
                dce[j].input_s.push_back(i_o);
            }
        }
        // 存储输入的第二部分---测试器件
        int test_num;
        cin >> test_num;
        int test_input[test_num][in_num];
        for (int i = 0; i < test_num; i++) {
            for (int j = 0; j < in_num; j++) {
                cin >> test_input[i][j];
            }
        }
        vector<int> test_output[test_num];  // 题目要的器件输出
        for (int i = 0; i < test_num; i++) {
            int test_output_num;
            cin >> test_output_num;
            for (int j = 0; j < test_output_num; j++) {
                int op_num_id;
                cin >> op_num_id;
                test_output[i].push_back(op_num_id);
            }
        }
        // 开始构建电路
        bool loop = 0;    // 判断是否成环
        int run_num = 0;  // 记录已经搭建好的器件数量
        int pre_level[d_num];
        while (1) {
            run_num = 0;
            for (int i = 0; i < d_num; i++) {
                pre_level[i] = dce[i].level;
            }
            for (int i = 0; i < d_num; i++) {
                if (!dce[i].ct)  // 未被建造
                {
                    run_num++;
                    for (int j = 0; j < dce[i].input_s.size(); j++) {
                        int run_level;
                        if (!dce[i].id_flag[j])  // 未访问I/O
                        {
                            if (dce[i].input_s[j][0] == 'I') {
                                run_level = 0;  // 输入端为0级
                                dce[i].id_flag[j] = 1;
                            } else if (dce[i].input_s[j][0] ==
                                       'O') {  // O必须之前建造过才能进行访问
                                if (dce[str_to_int(dce[i].input_s[j]) - 1].ct) {
                                    dce[i].id_flag[j] = 1;
                                    run_level =
                                        dce[str_to_int(dce[i].input_s[j]) - 1]
                                            .level +
                                        1;
                                }
                            }
                            if (run_level >
                                dce[i].level)  // level大进行层级更新
                            {
                                dce[i].level = run_level;
                            }
                        }
                    }
                    for (int j = 0; j < dce[i].id_flag.size(); j++) {
                        // 查找有无未访问I/O
                        if (!dce[i].id_flag[j])
                            break;
                        // 标识该器件已被建造
                        if (j == dce[i].id_flag.size() - 1)
                            dce[i].ct = true;
                    }
                }
            }
            // 判断电路是否成环
            if (run_num == 0)
                break;
            int same_num = 0;
            for (int i = 0; i < d_num; i++) {
                if (pre_level[i] == dce[i].level)
                    same_num++;  // 每执行一次各器件层级应该会发生变化，
            }                    // 没有变化说明电路存在环
            if (same_num == d_num) {
                loop = true;
                break;
            }
        }
        // 开始进行测试
        if (loop)
            cout << "LOOP" << endl;
        else {
            sort(dce, dce + d_num, cmp);  // 按层级大小排序
            for (int i = 0; i < test_num; i++) {
                map<int, int> opt;
                for (int j = 0; j < d_num; j++) {
                    for (int k = 0; k < dce[j].input_s.size(); k++) {
                        if (dce[j].input_s[k][0] ==
                            'I')  // 去test_input里面找对应的输入
                            dce[j].input.push_back(
                                test_input[i]
                                          [str_to_int(dce[j].input_s[k]) - 1]);
                        else if (dce[j].input_s[k][0] ==
                                 'O')  // 去output里面找对应的输出
                            dce[j].input.push_back(
                                opt[str_to_int(dce[j].input_s[k])]);
                    }
                    // 开始计算
                    dce[j].compute();
                    opt[dce[j].id] = dce[j].output;
                }
                for (int j = 0; j < test_output[i].size(); j++) {
                    cout << opt[test_output[i][j]] << " ";
                }
                cout << endl;
                // 释放资源
                opt.clear();
                for (int j = 0; j < d_num; j++) {
                    dce[j].input.clear();
                }
            }
        }
    }
    return 0;
}