#include "fmodel.h"
#include "../constants.h"

FlowModel::FlowModel(const Graph& g, const std::unordered_set<size_t> nodes):
    env{},
    model{env},
    cplex{env},
    graph{g},
    f(env, graph.size())
{
    // Objective function
    model.add(IloMinimize(env, 1));

    // Add flows vars
    for(int i = 0; i != graph.size(); ++i) {
        f[i] = IloNumVarArray(env, graph.size(), 0.0, MAX_FLOW, ILOFLOAT);
        for(int j = 0; j != graph.size(); ++j) {
            std::string var_name = "f" + std::to_string(i+1) + '_' + std::to_string(j+1);
            f[i][j].setName(var_name.c_str());
        }
    }

    // Max flow
    for(size_t i = 0; i != graph.size(); ++i) {
        for(size_t j = 0; j != graph.size(); ++j) {
            if(nodes.contains(i) || nodes.contains(j)) {
                model.add(f[i][j] <= g(i, j).max_flow * 0);
                continue;
            }

            model.add(f[i][j] <= g(i, j).max_flow * g(i, j).value);
        }
    }
    
    // Kirchhof
    for(size_t k = 0; k != graph.size(); ++k) {
        if(graph[k].type == NodeType::PowerStation) {
            continue;
        }

        IloExpr balance(env);
        for(size_t i = 0; i != graph.size(); ++i) {
            if(k == i) {
                continue;
            }
            balance += f[i][k];
        }

        for(size_t i = 0; i != graph.size(); ++i) {
            if(k == i) {
                continue;
            }
            balance -= f[k][i];
        }

        model.add(balance == g[k].demand);
        // std::cout << balance << '=' << demand[k] << std::endl; // DEBUG
        balance.end();
    }

    for(size_t i = 0; i != graph.size(); ++i) {
        model.add(f[i][i] == 0.0);
        // std::cout << y[i][i] << '=' << 0.0 << std::endl; //DEBUG
        // std::cout << f[i][i] << '=' << 0.0 << std::endl; //DEBUG
    }

    // Exclude a node
    // for(size_t i = 0; i != graph.size(); ++i) {
    //     if(!nodes.contains(i)) {
    //        continue; 
    //     }

    //     for(size_t j = 0; j != graph.size(); ++j) {
    //         model.add(f[i][j] == 0.0);
    //         model.add(f[j][i] == 0.0);
    //     }
    // }
}

bool FlowModel::solve() {
    cplex.extract(model);
    return cplex.solve();
}

Graph FlowModel::get_solution() {
    for(size_t i = 0; i != graph.size(); ++i) {
        for(size_t j = 0; j != graph.size(); ++j) {
            graph(i, j).flow = cplex.getValue(f[i][j]);
        }
    }
    return graph;
}

FlowModel::~FlowModel() {
    env.end();
}