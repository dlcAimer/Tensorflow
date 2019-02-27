//
// Created by Aliez on 2018/10/9.
//

#ifndef DISTRIBUTEDTENSORFLOW_CALCULATEWORKER_H
#define DISTRIBUTEDTENSORFLOW_CALCULATEWORKER_H

#include "Psworker.h"

class CalculateWorker {
private:
    PsWorker *psWorker;
    vector<vector<double >> deltas;

    vector<double> matrixPlusLevel1(vector<double> &parameter1, vector<vector<double >> &parameter2);

    vector<double> matrixPlusLevel2(vector<double> &parameter1, vector<vector<double >> &parameter2);

    vector<double>
    matrixPlusHiddenLevel(vector<double> &parameter1, vector<vector<double >> &parameter2, int startlevel);

    vector<double> matrixAddSigmoid(vector<double> &parameter1, vector<double> &bias);

    vector<double>
    dInputHiddenLevel(vector<double> &delta, vector<vector<double >> &parameterMatrix2, vector<double> &intermediateIn);

    vector<double> dHiddenOutputLevel(vector<double> &tempResult, double target);

    vector<double> dHiddenHiddenLevel(vector<double> &delta, vector<vector<double >> &parameterMatrix2,
                                      vector<double> &intermediateIn);

    void refreshParameterMatrix1(vector<vector<double >> &parameterMatrix1, vector<double> parametersIn,
                                 vector<double> &delta1, vector<double> &bias1);

    void refreshParameterMatrix2(vector<vector<double >> &parameterMatrix2, vector<double> &intermediateIn,
                                 vector<double> &delta2, vector<double> &bias2);

    graphNode *requestFromPsWorker(string kind, int no);

    void refreshPsWorker(graphNode *refreshStartPosition, vector<double> &parametersIn);


public:
    graphNode *calculateStartPosition;
    graphNode *refreshStartPosition;
    bool cutReady = false;

    CalculateWorker(PsWorker *psWorker1) { psWorker = psWorker1; }

    void calculate(vector<double> &parametersIn);

    void refresh(vector<double> &parametersIn);
};


#endif //DISTRIBUTEDTENSORFLOW_CALCULATEWORKER_H
