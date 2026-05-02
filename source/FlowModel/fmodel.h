// This class needs efor flow calculating in subgraphs of origin graph

#ifndef FLOW_MODEL_H
#define FLOW_MODEL_H
#include "../Graph/graph.h"
#include <ilcplex/ilocplex.h>
#include <unordered_set>

class FlowModel {
    IloEnv env;
    IloEnv model;
    IloCplex cplex;
    IloArray<IloNumVarArray> f;
    Graph graph;

    public:
        FlowModel(Graph& g, std::unordered_set<size_t> nodes);
        ~FlowModel();
        FlowModel FlowModel(const FlowModel) = delete;
        FlowModel& operator=(const FlowModel&) = delete;
        Graph& get_solution();
        bool solve();

}

#endif