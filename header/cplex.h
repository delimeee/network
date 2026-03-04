#ifndef SOLVER_CPLEX_H
#define SOLVER_CPLEX_H
#include <ilcplex/ilocplex.h>

class Cplex {
    public:
        Cplex();
        ~Cplex();
    private:
        IloCplex cplex;
};

#endif