//
// Created by Aliez on 2018/10/9.
//

#include "Master.h"

bool compareGraphNode1(graphNode *graphNode1, graphNode *graphNode2) {
    return graphNode1->no > graphNode2->no;
}

bool compareGraphNode2(graphNode *graphNode1, graphNode *graphNode2) {
    return graphNode1->no < graphNode2->no;
}

void sortCutNodes(vector<graphNode *> &input, const string order) {
    if (strcmp(order.c_str(), "positive") == 0) {
        sort(input.begin(), input.end(), compareGraphNode1);
    } else {
        sort(input.begin(), input.end(), compareGraphNode2);
    }
}

void Master::preOrderTraverse(graphNode *root) {
    if (root != nullptr) {
        if (root->kind == BIASNODE) {
            psWorker->biasNodes[root->no] = root;
        } else if (root->kind == MATRIXNODE) {
            psWorker->matrixNodes[root->no] = root;
        } else if (root->kind == OUTPUTNODE || root->kind == LEVELNODE) {
            cutPosition.push_back(root);
        }
        preOrderTraverse(root->left);
        preOrderTraverse(root->right);
    }
}

void Master::cutGraph() {
    if (graphReady) {
        preOrderTraverse(root);
        for (int i = 0; i < 200; i++) {
            unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
            shuffle(parametersIn.begin(), parametersIn.end(), default_random_engine(seed));
            for (int i = 0; i < parametersIn.size(); i++) {
                sortCutNodes(cutPosition, "positive");
                for (int j = 0; j < cutPosition.size(); j++) {
                    calculateWorker->cutReady = true;
                    calculateWorker->calculateStartPosition = cutPosition[j];
                    calculateWorker->calculate(parametersIn[i]);
                }
                sortCutNodes(cutPosition, "reversed");
                for (int j = 0; j < cutPosition.size(); j++) {
                    calculateWorker->cutReady = true;
                    calculateWorker->refreshStartPosition = cutPosition[j];
                    calculateWorker->refresh(parametersIn[i]);
                }
            }
        }
    }
}