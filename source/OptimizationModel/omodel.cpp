#include "omodel.h"
#include <cmath>
// #include <iostream> //DEBUG

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
    // Добавляем стоимости
    for (int i = 0; i < graph.size(); ++i) {
        cplex_cost[i] = IloNumArray(env, graph.size());
    }
    for(int i = 0; i != graph.size(); i++) {
        for(int j = 0; j != graph.size(); j++) {
            cplex_cost[i][j] = graph(i, j).cost;
        }
    }

    // Добавляем максимальные потоки
    for (int i = 0; i < graph.size(); ++i) {
        cplex_max_flow[i] = IloNumArray(env, graph.size());
    }
    for(int i = 0; i != graph.size(); i++) {
        for(int j = 0; j != graph.size(); j++) {
            cplex_max_flow[i][j] = graph(i, j).max_flow;
        }
    }

    // Добавляем переменные y
    for(int i = 0; i != graph.size(); ++i) {
        y[i] = IloNumVarArray(env, graph.size(), 0.0, 1.0, ILOFLOAT);
        for(int j = 0; j != graph.size(); ++j) {
            std::string var_name = "y" + std::to_string(i+1) + '_' + std::to_string(j+1);
            y[i][j].setName(var_name.c_str());
        }
    }

    // Переменные потока
    for(int i = 0; i != graph.size(); ++i) {
        f[i] = IloNumVarArray(env, graph.size(), 0.0, 1000.0, ILOFLOAT);
        for(int j = 0; j != graph.size(); ++j) {
            std::string var_name = "f" + std::to_string(i+1) + '_' + std::to_string(j+1);
            f[i][j].setName(var_name.c_str());
        }
    }

    // Потребление
    for(size_t i = 0; i != graph.size(); ++i) {
        demand[i] = graph[i].demand;
    }

    // Добавляем ЦФ
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

    // Ограничения на максимальный поток
    for(size_t i = 0; i != graph.size(); ++i) {
        for(size_t j = 0; j != graph.size(); ++j) {
            model.add(f[i][j] <= cplex_max_flow[i][j] * y[i][j]);

            // std::cout << f[i][j]; //DEBUG
            // std::cout << "<=" << cplex_max_flow[i][j] << '*' << y[i][j] << std::endl; //DEBUG
        }
    }

    // Ограничение на количество входов/выходов
    for(size_t k = 0; k != graph.size(); ++k) {
        IloExpr max_y(env);
        for(size_t j = 0; j != graph.size(); ++j) {
            if(k != j) max_y += y[k][j];
        }

        for(size_t i = 0; i != graph.size(); ++i) {
            if(k != i) max_y += y[i][k];
        }

        // std::cout << max_y << '=' << 2.0 << std::endl; // DEBUG
        model.add(max_y == 2.0);
        max_y.end();
    }

    // Закон Кхиргофа
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

        // std::cout << balance << '=' << demand[k] << std::endl; // DEBUG
        model.add(balance == demand[k]);
        balance.end();
    }


    // Исключаем дуги в самих себя 
    for(size_t i = 0; i != graph.size(); ++i) {
        model.add(y[i][i] == 0.0);
        model.add(f[i][i] == 0.0);
        // std::cout << y[i][i] << '=' << 0.0 << std::endl; //DEBUG
        // std::cout << f[i][i] << '=' << 0.0 << std::endl; //DEBUG
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

void OptimizationModel::add_constraint(Constraint& c) {
    model.add(c.load());
}