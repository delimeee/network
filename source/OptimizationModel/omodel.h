#ifndef OPT_MODEL_H
#define OPT_MODEL_H
#include <ilcplex/ilocplex.h>
#include "../Graph/graph.h"

class OptimizationModel {
    private:
        IloEnv env;
        IloModel model;
        IloCplex cplex;
        Graph graph;

    public:

        OptimizationModel();
        OptimizationModel(Graph&);
        ~OptimizationModel();
        OptimizationModel(const OptimizationModel&);
        OptimizationModel& operator=(OptimizationModel);
        OptimizationModel(OptimizationModel&&);
        OptimizationModel& operator=(OptimizationModel&&);

        Graph& get_solution();
        void add_constraint(const Constraint&);
        IloModel& get_model();
        bool solve();
}

#endif