#ifndef SOLVER_ENV_H
#define SOLVER_ENV_H
#include <ilcplex/ilocplex.h>
#include "model.h"
#include "cplex.h"

class Environment {
    public:
        Environment();
        ~Environment();
        IloEnv get_environment();

    private:
        IloEnv env;
        Model model;
        Cplex solver;

};

#endif  