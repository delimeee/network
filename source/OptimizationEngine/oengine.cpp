#include<iostream>
#include "../GraphAnalyzer/ganalyzer.h"
#include "./oengine.h"

OptimizationEngine::OptimizationEngine(Graph& g): graph{g}, model{g},  solve_status{false},
        survivable_network{false}, iterations{0}{ }

// Packages
void OptimizationEngine::run() {


    if(!(solve_status = model.solve())) {
        return;
    }

    auto g = model.get_solution();
    SolverMT exact_model(g, false);
    auto constr_vars = graph_analyzer.find_all_violations(g);
    while(!constr_vars.empty()) {
        model.add_survivable_constraint(constr_vars);
        exact_model.add_survivable_constraint(constr_vars);

        std::cout << "Количество ограничений: " << constr_vars.size() << '\n';

        iterations++;

        if(!(solve_status = model.solve())) {
            break;
        }
        g = model.get_solution();
        constr_vars = graph_analyzer.find_all_violations(g);
    }


    // === ШАГ 2: Переход к точному целочисленному решению (MIP) ===
    std::cout << "--- LP фаза завершена. Переключение в MIP (Точное решение) ---\n";

    
    exact_model.add_mip_start(g);
    // Пересчитываем модель уже в целых числах с учетом всех найденных LP-разрезов
    if (!(solve_status = exact_model.solve())) {
        return;
    }

    graph = exact_model.get_solution();
    survivable_network = graph_analyzer.validate_solution(graph);
}


Graph OptimizationEngine::get_solution() {
    if(solve_status) {
        return graph;
    } else {
        return Graph();
    }
}

size_t OptimizationEngine::get_iterations() {
    return iterations;
}

bool OptimizationEngine::get_survivable_network() {
    return survivable_network;
}