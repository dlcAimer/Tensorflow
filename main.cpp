#include "Client.h"

int main() {
    //设置Bp网结构为一层隐层，输入层节点3个，隐层节点10个，输出层节点1个
    vector<int> hidden;
    hidden.push_back(10);
    setBp(1, 3, hidden, 1, 0.08);
    //训练10个Bp网，看平均准确率
    for (int i = 0; i < 10; i++) {
        Client client;
        client.trainning();
        client.showResult();
    }
    return 0;
}