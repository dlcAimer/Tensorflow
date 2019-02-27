//
// Created by Aliez on 2018/10/9.
//

#ifndef DISTRIBUTEDTENSORFLOW_MASTER_H
#define DISTRIBUTEDTENSORFLOW_MASTER_H

#include "CalculateWorker.h"

class Master {
private:
    PsWorker* psWorker;
    CalculateWorker* calculateWorker;
    vector<graphNode *> cutPosition;
    void preOrderTraverse(graphNode* root);

public:
    graphNode* root;
    vector<vector<double >> parametersIn;
    bool graphReady = false;

    Master(){psWorker = new PsWorker();calculateWorker = new CalculateWorker(psWorker);}
    void cutGraph();
};


#endif //DISTRIBUTEDTENSORFLOW_MASTER_H
