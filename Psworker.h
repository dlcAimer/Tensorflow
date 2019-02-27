//
// Created by Aliez on 2018/10/9.
//

#ifndef DISTRIBUTEDTENSORFLOW_PSWORKER_H
#define DISTRIBUTEDTENSORFLOW_PSWORKER_H

#include "GraphNode.h"
#include <map>


class PsWorker {
public:
    map<int, graphNode*> biasNodes;
    map<int, graphNode*> matrixNodes;

    PsWorker() = default;

    void refresh(vector<vector<vector<double >>> &matrixes, vector<vector<double >> &biases);
};


#endif //DISTRIBUTEDTENSORFLOW_PSWORKER_H
