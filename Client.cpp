//
// Created by Aliez on 2018/10/9.
//

#include "Client.h"

void Client::initialize() {
    srand((unsigned) time(NULL) + rand());
    for(int j = 0; j < numOfHiddenLevel ; j++) {
        vector<vector<double >> tempMatrix;
        vector<double> tempBias;
        for (int i = 0; i < inputLevel; i++) {
            vector<double> temp;
            for (int k = 0; k < hiddenLevel[j]; k++) {
                double randomNumber = rand_back(0.0, 1.0);
                temp.push_back(randomNumber);
            }
            tempMatrix.push_back(temp);
        }
        parameterMatrixes.push_back(tempMatrix);
        for (int i = 0; i < hiddenLevel[j]; i++) {
//            tempBias.push_back(rand() % 1000 * 0.00001 );
            tempBias.push_back(0.0);
        }
        biases.push_back(tempBias);
    }
    vector<vector<double >> tempMatrix;
    vector<double> tempBias;
    for (int i = 0; i < hiddenLevel[numOfHiddenLevel-1]; i++) {
        vector<double> temp;
        for (int j = 0; j < outputLevel; j++) {
            double randomNumber = rand_back(0.0, 1.0);
            temp.push_back(randomNumber);
        }
        tempMatrix.push_back(temp);
    }
    parameterMatrixes.push_back(tempMatrix);
    for (int i = 0; i < outputLevel; i++) {
//        tempBias.push_back(rand() % 1000 * 0.001 - 0.5);
        tempBias.push_back(0.0);
    }
    biases.push_back(tempBias);
}

graphNode* Client::graphConstruct() {

    graphNode *root = new graphNode(0, OUTPUTNODE, empty, empty, emptyMatrix, "add-sigmoid");
    graphNode *position = root;

    int i = 0;
    for (; i < numOfHiddenLevel; i++) {
        position->left = new graphNode(2 * i + 1, EMPTYNODE, empty, empty, emptyMatrix, "plus",
                                       position);
        position->right = new graphNode(2 * i + 1, BIASNODE, empty, biases[biases.size() - i - 1],
                                        emptyMatrix, "none", position);
        position = root->left;

        position->left = new graphNode(2 * i + 2, LEVELNODE, empty, empty, emptyMatrix, "add-sigmoid",
                                       position);
        position->right = new graphNode(2 * i + 2, MATRIXNODE, empty, empty,
                                        parameterMatrixes[parameterMatrixes.size() - i - 1], "none", position);
        position = root->left->left;
    }
    position->left = new graphNode(2 * i + 1, EMPTYNODE, empty, empty, emptyMatrix, "plus", position);
    position->right = new graphNode(2 * i + 1, BIASNODE, empty, biases[biases.size() - i - 1], emptyMatrix,
                                    "none", position);

    position = root->left->left->left;
    position->left = new graphNode(2 * i + 2, INPUTNODE, empty, empty, emptyMatrix, "none", position);
    position->right = new graphNode(2 * i + 2, MATRIXNODE, empty, empty,
                                    parameterMatrixes[parameterMatrixes.size() - i - 1], "none", position);

    return root;
}

void Client::prepare() {
    initialize();
    dataProcessing(parametersIn,testData);
}

void Client::trainning() {
    prepare();
    root = graphConstruct();
    master->graphReady = true;
    master->root = root;
    master->parametersIn = parametersIn;
    master->cutGraph();
    parametersIn = master->parametersIn;
}

void Client::test() {
    testData = parametersIn;
    master = new Master();
    master->graphReady = true;
    master->root = root;
    master->parametersIn = testData;
    master->cutGraph();
    testData = master->parametersIn;
}

void Client::showResult() {
    test();
    int count = 0;
//    cout<<"预测结果为："<<endl;
    for (unsigned long i = testData.size() / 3 * 2; i < testData.size(); i++) {
        if (testData[i][4] > 0.5) {
//            cout << 1 << endl;
            if (testData[i][0] == 1) {
                count++;
            }
        } else {
//            cout << 0 << endl;
            if (testData[i][0] == 0) {
                count++;
            }
        }
    }
    cout <<"准确率为："<< count / 100.0 << endl;
}
