//
// Created by Aliez on 2018/10/24.
//

#ifndef DISTRIBUTEDTENSORFLOW_STATICPARAMS_H
#define DISTRIBUTEDTENSORFLOW_STATICPARAMS_H

#include <vector>
#include <cstdlib>
#include <cmath>
#include <random>
#include <chrono>
#include <fstream>

using namespace std;

#define pi 3.1415926

static int numOfHiddenLevel = 1;
static int inputLevel = 3;
static vector<int > hiddenLevel = {10};
static int outputLevel = 1;
static double alpha = 0.01;

static vector<double> empty;
static vector<vector<double >> emptyMatrix;

static void setBp(int NumOfHiddenLevel, int Inputlevel, vector<int > &Hiddenlevel, int Outputlevel, double Alpha){
    numOfHiddenLevel = NumOfHiddenLevel;
    inputLevel = Inputlevel;
    for(int i = 0 ; i < Hiddenlevel.size() ; i++) {
        hiddenLevel.push_back(Hiddenlevel[i]);
    }
    outputLevel = Outputlevel;
    alpha = Alpha;
}

static double rand_back(double i,double j) {

    double u1=double(rand()%1000)/1000,u2=double(rand()%1000)/1000,r;
    static unsigned int seed=0;
    r=i+sqrt(j)*sqrt(-2.0*(log(u1)/log(exp(1.0))))*cos(2*pi*u2);
    return 0.01 * r;
}

static void dataProcessing(vector<vector<double >> &parametersIn, vector<vector<double >> &testData) {
    ifstream readIn("/Users/Aliez/CLionProjects/DistributedTensorflow/student_data.csv");
    string tempLine;
    unsigned long cursor;
    vector<vector<double >> parameters;
    while (getline(readIn, tempLine)) {
        if (tempLine[0] > '9') {
            continue;
        }
        vector<double> temp;
        while ((cursor = tempLine.find(',')) != string::npos) {
            double parameter = atof(tempLine.substr(0, cursor).c_str());
            tempLine = tempLine.substr(cursor + 1, tempLine.length() - cursor - 1);
            temp.push_back(parameter);
        }
        temp.push_back(atof(tempLine.c_str()));
        parameters.push_back(temp);
    }
    readIn.close();
    for (int i = 0; i < parameters.size() /4 * 3 + 30 ; i++) {
        parameters[i][1] = parameters[i][1] / 800;
        parameters[i][2] = parameters[i][2] / 4.0;
        parameters[i][3] = parameters[i][3] / 4.0;
        parameters[i].push_back(0.0);
        parametersIn.push_back(parameters[i]);
    }
    for (int i = parameters.size() / 4 * 3 ; i < parameters.size(); i++) {
        parameters[i][1] = parameters[i][1] / 800;
        parameters[i][2] = parameters[i][2] / 4.0;
        parameters[i][3] = parameters[i][3] / 4.0;
        parameters[i].push_back(0.0);
        testData.push_back(parameters[i]);
    }
//    for(int i = parametersIn.size() / 4; i < parametersIn.size() / 4; i++){
//        testData.push_back(parametersIn[i]);
//    }
}

static double sigmoid(double x) {
    return (1 / (1 + exp(-x)));
}

#endif //DISTRIBUTEDTENSORFLOW_STATICPARAMS_H
