//
// Created by Aliez on 2018/10/21.
//

#ifndef DISTRIBUTEDTENSORFLOW_UTILS_H
#define DISTRIBUTEDTENSORFLOW_UTILS_H

#include <iostream>
#include "StaticParams.h"

using namespace std;

const int EMPTYNODE = 0;
const int LEVELNODE = 1;
const int INPUTNODE = 2;
const int OUTPUTNODE = 3;
const int BIASNODE = 4;
const int MATRIXNODE = 5;

struct graphNode {
    int no;
    int kind;
    vector<double> actionResult;
    vector<double> bias;
    vector<vector<double >> matrix;
    string action;
    graphNode *left;
    graphNode *right;
    graphNode *parent;

    graphNode(int No = -1, int Kind = -1, vector<double> &Operand = empty, vector<double> &Bias = empty,
              vector<vector<double >> &Matrix = emptyMatrix,
              string Action = NULL, graphNode *Parent = nullptr) :
            no(No), kind(Kind), action(std::move(Action)), left(nullptr), right(nullptr), parent(Parent) {
        for (double i : Operand) {
            actionResult.push_back(i);
        }
        for (double j : Bias) {
            bias.push_back(j);
        }
        for (int i = 0; i < Matrix.size(); i++) {
            vector<double> temp;
            for (int j = 0; j < Matrix[0].size(); j++) {
                temp.push_back(Matrix[i][j]);
            }
            matrix.push_back(temp);
            temp.clear();
        }
    }
};

#endif //DISTRIBUTEDTENSORFLOW_UTILS_H