//
// Created by Aliez on 2018/10/26.
//

#include "Psworker.h"

void PsWorker::refresh(vector<vector<vector<double>>> &matrixes, vector<vector<double>> &biases) {
    matrixNodes[4]->matrix = matrixes[0];
    matrixNodes[2]->matrix = matrixes[1];

    biasNodes[3]->bias = biases[0];
    biasNodes[1]->bias = biases[1];
}
