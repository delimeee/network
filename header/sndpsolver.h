#ifndef SNDP_SOLVER_H
#define SNDP_SOLVER_H
#include<ilcplex/ilocplex.h>



class SndpSolver {
    private:
        IloEnv env;
        IloModel model;
        IloCplex cplex;

    public:
        SndpSolver();
        ~SndpSolver();
        SndpSolver(const LinearSolver&) = delete;
        LinearOperator& operator=(const LinearSolver&) = delete;
};  


#endif