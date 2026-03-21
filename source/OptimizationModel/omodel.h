#ifndef OPT_MODEL_H
#define OPT_MODEL_H
#include <ilcplex/ilocplex.h>
#include "../Constraint/constraint.h"
#include "../Graph/graph.h"

class OptimizationModel {
    private:
        IloEnv env;
        IloModel model;
        IloCplex cplex;
        Graph graph;

    public:
        OptimizationModel(Graph&);
        ~OptimizationModel();
        OptimizationModel(const OptimizationModel&) = delete;
        OptimizationModel& operator=(OptimizationModel) = delete;

        Graph& get_solution();
        void add_constraint(Constraint&);
        IloModel& get_model();
        bool solve();
};

#endif