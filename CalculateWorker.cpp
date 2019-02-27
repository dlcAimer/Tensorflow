//
// Created by Aliez on 2018/10/9.
//

#include "CalculateWorker.h"

vector<double> CalculateWorker::matrixPlusLevel1(vector<double> &parameter1, vector<vector<double >> &parameter2) {
    vector<double> result;
    for (int i = 0; i < hiddenLevel[0]; i++) {
        double temp = 0.0;
        for (int j = 0; j < inputLevel; j++) {
            temp += parameter1[j] * parameter2[j][i];
        }
        result.push_back(temp);
    }
    return result;
}

vector<double> CalculateWorker::matrixPlusLevel2(vector<double> &parameter1, vector<vector<double >> &parameter2) {
    vector<double> result;
    for (int i = 0; i < outputLevel; i++) {
        double temp = 0.0;
        for (int j = 0; j < hiddenLevel[0]; j++) {
            temp += parameter1[j] * parameter2[j][i];
        }
        result.push_back(temp);
    }
    return result;
}

vector<double> CalculateWorker::matrixPlusHiddenLevel(vector<double> &parameter1, vector<vector<double >> &parameter2, int startlevel) {
    vector<double> result;
    for (int i = 0; i < hiddenLevel[startlevel + 1]; i++) {
        double temp = 0.0;
        for (int j = 0; j < hiddenLevel[startlevel]; j++) {
            temp += parameter1[j] * parameter2[j][i];
        }
        result.push_back(temp);
    }
    return result;
}

vector<double> CalculateWorker::matrixAddSigmoid(vector<double> &parameter1, vector<double> &bias) {
    for (int i = 0; i < parameter1.size(); i++) {
        double temp = sigmoid(parameter1[i] + bias[i]);
        parameter1[i] = temp;
    }
    return parameter1;
}

vector<double > CalculateWorker::dInputHiddenLevel(vector<double > &delta,vector<vector<double >> &parameterMatrix2, vector<double > &intermediateIn) {
    vector<double> result;
    for (int i = 0; i < hiddenLevel[0]; i++) {
        double temp = 0.0;
        for (int j = 0; j < outputLevel; j++) {
            temp += parameterMatrix2[i][j] * delta[j];
        }
        result.push_back(intermediateIn[i] * (1.0 - intermediateIn[i]) * temp);
    }
    return result;
}

vector<double> CalculateWorker::dHiddenHiddenLevel(vector<double > &delta,vector<vector<double >> &parameterMatrix2, vector<double > &intermediateIn) {
    vector<double> result;
    for (int i = 0; i < hiddenLevel[0]; i++) {
        double temp = 0.0;
        for (int j = 0; j < outputLevel; j++) {
            temp += parameterMatrix2[i][j] * delta[j];
        }
        result.push_back(intermediateIn[i] * (1.0 - intermediateIn[i]) * temp);
    }
    return result;
}

vector<double > CalculateWorker::dHiddenOutputLevel(vector<double > &tempResult,double target){
    vector<double> result;
    for (int i = 0; i < outputLevel; i++) {
        result.push_back((tempResult[i] - target) * tempResult[i] * (1 - tempResult[i]));
    }
    return result;
}

void CalculateWorker::refreshParameterMatrix1(vector<vector<double >> &parameterMatrix1, vector<double > parametersIn, vector<double > &delta1
        , vector<double > &bias1) {
    for (int j = 0; j < parameterMatrix1[0].size(); j++) {
        bias1[j] -= alpha * delta1[j];
        for (int k = 0; k < parameterMatrix1.size(); k++) {
            parameterMatrix1[k][j] -= alpha * parametersIn[k] * delta1[j];
        }
    }
}

void CalculateWorker::refreshParameterMatrix2(vector<vector<double >> &parameterMatrix2, vector<double > &intermediateIn,vector<double > &delta2
        ,vector<double > &bias2) {
    for (int i = 0; i < outputLevel; i++) {
        bias2[i] -= alpha * delta2[i];
        for (int j = 0; j < hiddenLevel[0]; j++) {
            parameterMatrix2[j][i] -= alpha * intermediateIn[j] * delta2[i];
        }
    }
}

graphNode* CalculateWorker::requestFromPsWorker(string kind, int no) {
    if(kind == "plus"){
        return psWorker->matrixNodes[no+1];
    } else if( kind == "add-sigmoid"){
        return psWorker->biasNodes[no+1];
    }
    return nullptr;
}

void CalculateWorker::refreshPsWorker(graphNode *refreshStartPosition, vector<double> &parametersIn) {
    vector<double > In;
    for(int i = 1; i < 4 ; i++){
        In.push_back(parametersIn[i]);
    }
    refreshParameterMatrix2(requestFromPsWorker("plus", 1)->matrix,
                            refreshStartPosition->actionResult, deltas[0],
                            requestFromPsWorker("add-sigmoid", 0)->bias);
    refreshParameterMatrix1(requestFromPsWorker("plus", 3)->matrix,
                            In, deltas[deltas.size() - 1],
                            requestFromPsWorker("add-sigmoid", 2)->bias);
    vector<vector<vector<double>>> matrixes;
    vector<vector<double >> biases;
    matrixes.push_back(requestFromPsWorker("plus", 3)->matrix);
    matrixes.push_back(requestFromPsWorker("plus", 1)->matrix);
    biases.push_back(requestFromPsWorker("add-sigmoid", 2)->bias);
    biases.push_back(requestFromPsWorker("add-sigmoid", 0)->bias);
    psWorker->refresh(matrixes,biases);
}

void CalculateWorker::calculate(vector<double> &parametersIn) {
    if (cutReady) {
        vector<double > In;
        for(int i = 1; i < 4 ; i++){
            In.push_back(parametersIn[i]);
        }
        if (calculateStartPosition->no == numOfHiddenLevel * 2) {
            while (calculateStartPosition->left != nullptr || calculateStartPosition->right != nullptr) {
                calculateStartPosition = calculateStartPosition->left;
            }
            while (calculateStartPosition->kind != LEVELNODE) {
                calculateStartPosition = calculateStartPosition->parent;
                if (calculateStartPosition->action == "plus") {
                    graphNode *temp = requestFromPsWorker("plus", calculateStartPosition->no);
                    calculateStartPosition->actionResult = matrixPlusLevel1(
                            In, temp->matrix);
                }
            }
        } else if(calculateStartPosition->no < numOfHiddenLevel * 2 && calculateStartPosition->no > 0) {
            int rightLevel = calculateStartPosition->no % 10 / 2;
            while (calculateStartPosition->no != (rightLevel + 1) * 2) {
                calculateStartPosition = calculateStartPosition->left;
            }
            rightLevel = rightLevel + 1 ;
            while (calculateStartPosition->no % 10 != rightLevel * 2) {
                calculateStartPosition = calculateStartPosition->parent;
                if (calculateStartPosition->action == "plus") {
                    graphNode *temp = requestFromPsWorker("plus", calculateStartPosition->no);
                    calculateStartPosition->actionResult = matrixPlusHiddenLevel(
                            calculateStartPosition->left->actionResult,
                            temp->matrix, 2 - rightLevel);
                }
            }
        } else if (calculateStartPosition->no == 0) {
            while (calculateStartPosition->no != 2) {
                calculateStartPosition = calculateStartPosition->left;
            }
            while (calculateStartPosition->kind != OUTPUTNODE) {
                calculateStartPosition = calculateStartPosition->parent;
                if (calculateStartPosition->action == "plus") {
                    graphNode *temp = requestFromPsWorker("plus", calculateStartPosition->no);
                    calculateStartPosition->actionResult = matrixPlusLevel2(
                            calculateStartPosition->left->actionResult, temp->matrix);
                }
            }
        }
        if (calculateStartPosition->action == "add-sigmoid") {
            graphNode *temp = requestFromPsWorker("add-sigmoid", calculateStartPosition->no);
            calculateStartPosition->actionResult = matrixAddSigmoid(
                    calculateStartPosition->left->actionResult, temp->bias);
            if(calculateStartPosition->no == 0) {
                parametersIn[4] = calculateStartPosition->actionResult[0];
            }
        }
        cutReady = false;
    }
}

void CalculateWorker::refresh(vector<double> &parametersIn) {
    if (cutReady) {
        vector<double> temp;
        vector<double> delta;
        if (refreshStartPosition->no == 0) {
            temp = refreshStartPosition->actionResult;
            delta = dHiddenOutputLevel(temp, parametersIn[0]);
            deltas.push_back(delta);
        } else if (refreshStartPosition->no > 0 && refreshStartPosition->no < numOfHiddenLevel * 2){
            temp = refreshStartPosition->actionResult;
        }
        else if (refreshStartPosition->no == 2) {
            temp = refreshStartPosition->actionResult;
            delta = dInputHiddenLevel(deltas[refreshStartPosition->no / 2 - 1],
                                      requestFromPsWorker("plus", refreshStartPosition->no - 1)->matrix,
                                      refreshStartPosition->actionResult);
            deltas.push_back(delta);
            refreshPsWorker(refreshStartPosition, parametersIn);
            deltas.clear();
        }
        cutReady = false;
    }
}
