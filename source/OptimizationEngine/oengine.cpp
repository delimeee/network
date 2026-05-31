#include "./oengine.h"
#include <iostream>
#include "../GraphAnalyzer/ganalyzer.h"

OptimizationEngine::OptimizationEngine(Graph& g)
    : graph{g}, solve_status{false}, survivable_network{false}, iterations{0} {}

void OptimizationEngine::run() {

    OptimizationModel model(graph);

    if (!(solve_status = model.solve())) {
        return;
    }

    auto g = model.get_solution();
    GraphAnalyzer graph_analyzer;
    auto constr_vars = graph_analyzer.find_all_violations(g);
    auto constrains = constr_vars;
    while (!constr_vars.empty()) {
        model.add_survivable_constraint(constr_vars);
        constrains.insert(constrains.end(),
                          std::make_move_iterator(constr_vars.begin()),
                          std::make_move_iterator(constr_vars.end()));

        iterations++;

        if (!(solve_status = model.solve())) {
            break;
        }

        std::cout << "Количество ограничений: " << constr_vars.size() << '\n';

        g = model.get_solution();
        constr_vars = graph_analyzer.find_all_violations(g);
    }

    std::vector<decltype(constr_vars)::value_type>().swap(constr_vars);

    SolverMT exact_model(graph, false);
    exact_model.add_survivable_constraint(constrains);
    std::vector<decltype(constrains)::value_type>().swap(constrains);

    exact_model.add_mip_start(graph);

    if (!(solve_status = exact_model.solve())) {
        return;
    }

    graph = exact_model.get_solution();
    survivable_network = graph_analyzer.validate_solution(graph);
}

Graph OptimizationEngine::get_solution() {
    if (solve_status) {
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