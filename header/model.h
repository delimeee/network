#ifndef NETWORK_MODEL_H
#define NETWORK_MODEL_H
#include <ilcplex/ilocplex.h>
#include "environment.h"



class Model {
    public:
        Model(IloEnv env);
        ~Model();

    private:
        IloModel model;

};

#endif