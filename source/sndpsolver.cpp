#include<../header/sndpsolver.h>


SndpSolver::SndpSolver: env{}, model(env), cplex(env) { }

SndpSolver::~SndpSolver {
    env.end();
}

