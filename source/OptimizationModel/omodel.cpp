#include "omodel.h"

// class OptimizationModel {
//     private:
//         IloEnv env;
//         IloModel model;
//         IloCplex cplex;
//         Graph graph;

//     public:

//         OptimizationModel();
//         OptimizationModel(Graph&);
//         ~OptimizationModel();
//         OptimizationModel(const OptimizationModel&);
//         OptimizationModel& operator=(OptimizationModel);
//         OptimizationModel(OptimizationModel&&);
//         OptimizationModel& operator=(OptimizationModel&&);

//         Graph& get_solution();
//         void add_constraint(const Constraint&);
//         IloModel& get_model();
//         bool solve();
// }


OptimizationModel::OptimizationModel(): env{}, model(env), cplex{env}, graph() { }

OptimizationModel::OptimizationModel(Graph& g): env{}, model(env), cplex{env}, graph(g) {
    // Добавляем стоимости
    IloArray<IloNumArray> cplex_cost(env, n);
    for (int i = 0; i < graph.size(); ++i) {
        cplex_cost[i] = IloNumArray(env, n);
    }
    for(int i = 0; i != graph.size(); i++) {
        for(int j = 0; j != graph.size(); j++) {
            cplex_cost[i][j] = graph(i, j).cost;
        }
    }

    // Добавляем максимальные потоки
    IloArray<IloNumArray> cplex_max_flow(env, n);
    for (int i = 0; i < graph.size(); ++i) {
        cplex_max_flow[i] = IloNumArray(env, n);
    }
    for(int i = 0; i != graph.size(); i++) {
        for(int j = 0; j != graph.size(); j++) {
            cplex_max_flow[i][j] = graph(i, j).cost;
        }
    }

    // Добавляем переменные y
    IloArray<IloNumVarArray> y(env, graph.size());
    for(int i = 0; i != graph.size(); ++i) {
        y[i] = IloNumVarArray(env, graph.size(), 0.0, 1.0, ILOFLOAT);
        for(int j = 0; j != graph.size(); ++j) {
            std::string var_name = "y" + std::to_string(i+1) + '_' + std::to_string(j+1);
            y[i][j].setName(var_name.c_str());
        }
    }

    // Переменные потока
    IloArray<IloNumVarArray> f(env, graph.size());
    for(int i = 0; i != graph.size(); ++i) {
        f[i] = IloNumVarArray(env, graph.size(), 0.0, 1000.0, ILOFLOAT);
        for(int j = 0; j != graph.size(); ++j) {
            std::string var_name = "f" + std::to_string(i+1) + '_' + std::to_string(j+1);
            f[i][j].setName(var_name.c_str());
        }
    }

    // Добавляем ЦФ
    IloExpr obj(env);
    for(int i = 0; i != graph.size(); ++i) {
        for(int j = 0; j != graph.size(); ++j) {
            obj += cplex_cost[i][j] * y[i][j];
        }
    }
    model.add(IloMinimize(env, obj));
    obj.end();

    // Ограничения на максимальный поток
    for(size_t i = 0; i != graph.size(); ++i) {
        for(size_t j = 0; j != graph.size(); ++j) {
            IloExpr mflow(env);
            mflow = f[i][j];
            model.add(mflow <= cplex_max_flow[i][j] * y[i][j])
            mflow.end();
        }
    }

    // Ограничение на количество входов/выходов
    for(size_t i = 0; i != graph.size(); ++i) {
        IloExpr max_y(env);
        for(size_t j = 0; j != graph.size(); ++i) {
            max_y += y[i][j];
        }
        model.add(max_y >= 2.0 + ERROR)
        max_y.end()
    }



    // Закон Кхиргофа
    for(size_t i = 0; i != graph.size(); ++i) {
        IloExpr balance(env);

        for(auto& edge : graph.get_edges(i)) {
            balance += f[edge.from][edge.to]
        }

        balance.end();
    }
    
}

OptimizationModel::~OptimizationModel() {
    cplex.end()
    model.end()
    env.end()
}

OptimizationModel::OptimizationModel(const OptimizationModel& m) {
    cplex = m.cplex;

}