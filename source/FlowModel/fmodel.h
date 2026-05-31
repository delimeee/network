// This class needs efor flow calculating in subgraphs of origin graph

#ifndef FLOW_MODEL_H
#define FLOW_MODEL_H
#include <ilcplex/ilocplex.h>
#include <unordered_set>
#include "../Graph/graph.h"

class FlowModel {
    IloEnv env;
    IloModel model;
    IloCplex cplex;
    Graph graph;
    IloArray<IloNumVarArray> f;

   public:
    FlowModel(const Graph& g, const std::unordered_set<size_t> nodes);
    ~FlowModel();
    FlowModel(const FlowModel&) = delete;
    FlowModel& operator=(FlowModel) = delete;

    Graph get_solution();
    bool solve();
};

#endif