#include<iostream>
#include "../GraphAnalyzer/ganalyzer.h"
#include "../OptimizationEngine/oengine.h"

OptimizationEngine::OptimizationEngine(Graph& g): model{g}, status{false}, iterations{0}{ }

void OptimizationEngine::run() {
    if(!(status = model.solve())) {
        return;
    }

    auto g = model.get_solution();
    auto constr_vars = graph_analyzer.validate_solution(g);
    while(constr_vars.has_value()) {
        auto [sub_graph, value] = constr_vars.value();
        model.add_survivable_constraint(sub_graph, value);

        iterations++;

        if(!(status = model.solve())) {
            return;
        }
        g = model.get_solution();
        constr_vars = graph_analyzer.validate_solution(g);
    }
}

Graph OptimizationEngine::get_solution() {
    if(status) {
        return model.get_solution();
    } else {
        return Graph();
    }
}

size_t OptimizationEngine::get_iterations() {
    return iterations;
}