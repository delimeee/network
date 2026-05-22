#include<iostream>
#include "../GraphAnalyzer/ganalyzer.h"
#include "./oengine.h"

OptimizationEngine::OptimizationEngine(Graph& g): graph{g},  solve_status{false},
        survivable_network{false}, iterations{0}{ }

// Packages
void OptimizationEngine::run() {

    OptimizationModel model(graph);

    if(!(solve_status = model.solve())) {
        return;
    }

    auto g = model.get_solution();
    GraphAnalyzer graph_analyzer;
    auto constr_vars = graph_analyzer.find_all_violations(g);
    auto constrains = constr_vars;
    while(!constr_vars.empty()) {
        model.add_survivable_constraint(constr_vars);
        constrains.insert(
            constrains.end(),
            std::make_move_iterator(constr_vars.begin()),
            std::make_move_iterator(constr_vars.end())
        );

        iterations++;

        if(!(solve_status = model.solve())) {
            break;
        }

        std::cout << "Float model size:\n";
        model.print_model_size();
        std::cout << "Количество ограничений: " << constr_vars.size() << '\n';
        size_t total_bytes = sizeof(constrains) + (constrains.capacity() * sizeof(constrains[0]));
        // Переводим в гигабайты (делим на 1024 в кубе)
        // Добавление .0 к числам превращает деление в вещественное
        double total_gb = static_cast<double>(total_bytes) / (1024.0 * 1024.0 * 1024.0);
        std::cout << "Память вектора: " << total_gb << " GB\n";

        g = model.get_solution();
        constr_vars = graph_analyzer.find_all_violations(g);
    }

    std::vector<decltype(constr_vars)::value_type>().swap(constr_vars);
    model.clear();

    // === ШАГ 2: Переход к точному целочисленному решению (MIP) ===
    std::cout << "--- LP фаза завершена. Переключение в MIP (Точное решение) ---\n";

    SolverMT exact_model(graph, false);
    exact_model.add_survivable_constraint(constrains);
    std::vector<decltype(constrains)::value_type>().swap(constrains);
    
    exact_model.add_mip_start(graph);
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