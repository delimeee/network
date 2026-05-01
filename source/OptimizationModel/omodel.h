#ifndef OPT_MODEL_H
#define OPT_MODEL_H
#include <ilcplex/ilocplex.h>
#include <unordered_set>
#include "../Graph/graph.h"

class OptimizationModel {
    private:
        Graph graph;
        IloEnv env;
        IloModel model;
        IloCplex cplex;
        IloArray<IloNumArray> cplex_cost;
        IloArray<IloNumArray> cplex_max_flow;
        IloArray<IloNumVarArray> y;
        IloArray<IloNumVarArray> f;
        IloNumArray demand;


    public:
        OptimizationModel(Graph&);
        ~OptimizationModel();
        OptimizationModel(const OptimizationModel&) = delete;
        OptimizationModel& operator=(OptimizationModel) = delete;

        Graph& get_solution();
        void add_survivable_constraint(const std::unordered_set<size_t>&, double);
        IloModel& get_model();
        bool solve();
};

#endif