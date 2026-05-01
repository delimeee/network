#include "omodel.h"
#include "../constants.h"
#include <cmath>
#include <iostream> //DEBUG

OptimizationModel::OptimizationModel(Graph& g):
    env{},
    model(env),
    cplex{env},
    cplex_cost(env, graph.size()),
    cplex_max_flow(env, graph.size()),
    y(env, graph.size()),
    f(env, graph.size()),
    demand(env, graph.size()),
    graph(g) {
    // Create Costs
    for (int i = 0; i < graph.size(); ++i) {
        cplex_cost[i] = IloNumArray(env, graph.size());
    }
    for(int i = 0; i != graph.size(); i++) {
        for(int j = 0; j != graph.size(); j++) {
            cplex_cost[i][j] = graph(i, j).cost;
        }
    }

    // Create Maximum possible flow
    for (int i = 0; i < graph.size(); ++i) {
        cplex_max_flow[i] = IloNumArray(env, graph.size());
    }
    for(int i = 0; i != graph.size(); i++) {
        for(int j = 0; j != graph.size(); j++) {
            cplex_max_flow[i][j] = graph(i, j).max_flow;
        }
    }

    // Create Structure vars y
    for(int i = 0; i != graph.size(); ++i) {
        y[i] = IloNumVarArray(env, graph.size(), 0.0, 1.0, ILOFLOAT);
        for(int j = 0; j != graph.size(); ++j) {
            std::string var_name = "y" + std::to_string(i+1) + '_' + std::to_string(j+1);
            y[i][j].setName(var_name.c_str());
        }
    }

    // Create Flow vars f
    for(int i = 0; i != graph.size(); ++i) {
        f[i] = IloNumVarArray(env, graph.size(), 0.0, MAX_FLOW, ILOFLOAT);
        for(int j = 0; j != graph.size(); ++j) {
            std::string var_name = "f" + std::to_string(i+1) + '_' + std::to_string(j+1);
            f[i][j].setName(var_name.c_str());
        }
    }

    // Create Concumption/Production
    for(size_t i = 0; i != graph.size(); ++i) {
        demand[i] = graph[i].demand;
    }

    // Objective function
    IloExpr obj(env);
    for(int i = 0; i != graph.size(); ++i) {
        for(int j = 0; j != graph.size(); ++j) {
            obj += cplex_cost[i][j] * y[i][j] * [](const Node& n1, const Node& n2){
                return sqrt((n1.x - n2.x) * (n1.x - n2.x) + (n1.y - n2.y) * (n1.y - n2.y));
            }(graph[i], graph[j]);
        }
    }
    // std::cout << obj << std::endl; // DEBUG
    model.add(IloMinimize(env, obj));
    obj.end();

    // Maximum flow constraint
    for(size_t i = 0; i != graph.size(); ++i) {
        for(size_t j = 0; j != graph.size(); ++j) {
            model.add(f[i][j] <= cplex_max_flow[i][j] * y[i][j]);

            // std::cout << f[i][j]; //DEBUG
            // std::cout << "<=" << cplex_max_flow[i][j] << '*' << y[i][j] << std::endl; //DEBUG
        }
    }

    // Input/Output amount constraint
    for(size_t k = 0; k != graph.size(); ++k) {
        IloExpr max_y(env);
        for(size_t j = 0; j != graph.size(); ++j) {
            if(k != j) max_y += y[k][j];
        }

        // Add it only for directed structure graph(non-symetric adjency matrix)
        // for(size_t j = 0; j != graph.size(); ++j) {
        //     if(k != j) max_y += y[j][k];
        // }
        // std::cout << max_y << '=' << 2.0 << std::endl; // DEBUG
        
        //model.add(max_y == 2.0);

        if(graph[k].type == NodeType::City) {
            model.add(max_y == 2.0);
        } else {
            model.add(max_y >= 1.0);
        }
        max_y.end();
    }

    // Kirchhoff law constraint
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

        model.add(balance == demand[k]);
        // std::cout << balance << '=' << demand[k] << std::endl; // DEBUG
        balance.end();
    }


    // Exclude self-directed arc and edges constraint
    for(size_t i = 0; i != graph.size(); ++i) {
        model.add(y[i][i] == 0.0);
        model.add(f[i][i] == 0.0);
        // std::cout << y[i][i] << '=' << 0.0 << std::endl; //DEBUG
        // std::cout << f[i][i] << '=' << 0.0 << std::endl; //DEBUG
    }

    // Simmetry for not directed structure graph
    for(size_t i = 0; i < graph.size() - 1; ++i) {
        for(size_t j = i + 1; j != graph.size(); ++j) {
            model.add(y[i][j] - y[j][i] == 0);
        }
    }
}

OptimizationModel::~OptimizationModel() {
    cplex.end();
    model.end();
    env.end();
}

bool OptimizationModel::solve() {
    cplex.extract(model);
    return cplex.solve();
}

IloModel& OptimizationModel::get_model() {
    return model;
}

Graph& OptimizationModel::get_solution() {
    for(int i = 0; i < graph.size(); ++i) {
        for(int j = 0; j < graph.size(); ++j) {
            graph(i, j).value = cplex.getValue(y[i][j]);
            graph(i, j).flow = cplex.getValue(f[i][j]);
        }
    }
    return graph;
}

void OptimizationModel::add_survivable_constraint(const std::unordered_set<size_t>& nodes, double rvalue) {
    IloExpr surv_expr(env);
    for(auto& v: nodes) {
        for(size_t i = 0; i != graph.size(); ++i) {
            if(!nodes.contains(i)) {
                surv_expr += y[i][v];
                if(graph(i, v).value > ERROR) {
                    std::cout << "\t" << i + 1 << '\n';
                }
            }
        }
        std::cout << v + 1 << '\n';
    }
    std::cout << "ADDED CONSTRAINT\n";
    std::cout << surv_expr << rvalue << '\n';
    model.add(surv_expr >= rvalue);
    surv_expr.end();
}