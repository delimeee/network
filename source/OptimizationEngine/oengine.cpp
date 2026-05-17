#include<iostream>
#include "../GraphAnalyzer/ganalyzer.h"
#include "../OptimizationEngine/oengine.h"

OptimizationEngine::OptimizationEngine(Graph& g): model{g},  solve_status{false},
        survivable_network{false}, iterations{0}{ }

// void OptimizationEngine::run() {
//     if(!(solve_status = model.solve())) {
//         return;
//     }

//     auto g = model.get_solution();
//     auto constr_vars = graph_analyzer.find_violation(g);
//     while(constr_vars.has_value()) {
//         auto [sub_graph, value] = constr_vars.value();
//         model.add_survivable_constraint(sub_graph, value);

//         iterations++;

//         if(!(solve_status = model.solve())) {
//             return;
//         }
//         g = model.get_solution();
//         constr_vars = graph_analyzer.find_violation(g);
//     }
// }

// Packages
void OptimizationEngine::run() {
    if(!(solve_status = model.solve())) {
        return;
    }

    auto g = model.get_solution();
    auto constr_vars = graph_analyzer.find_all_violations(g);
    while(!constr_vars.empty()) {
        model.add_survivable_constraint(constr_vars);

        iterations++;

        if(!(solve_status = model.solve())) {
            break;
        }
        g = model.get_solution();
        constr_vars = graph_analyzer.find_all_violations(g);
    }

    survivable_network = graph_analyzer.validate_solution(g); 
}


Graph OptimizationEngine::get_solution() {
    if(solve_status) {
        return model.get_solution();
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