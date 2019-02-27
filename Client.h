//
// Created by Aliez on 2018/10/9.
//

#ifndef DISTRIBUTEDTENSORFLOW_CLIENT_H
#define DISTRIBUTEDTENSORFLOW_CLIENT_H

#include <utility>
#include "Master.h"

class Client {
private:
    graphNode *root;
    vector<vector<double >> parametersIn;
    vector<vector<double >> testData;
    vector<vector<vector<double >>> parameterMatrixes;
    vector<vector<double >> biases;
    Master *master = new Master();

    graphNode *graphConstruct();

    void initialize();

    void prepare();

    void test();

public:

    void trainning();

    void showResult();
};


#endif //DISTRIBUTEDTENSORFLOW_CLIENT_H
