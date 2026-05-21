#include<iostream>
#include "../GraphAnalyzer/ganalyzer.h"
#include "./oengine.h"

// OptimizationEngine::OptimizationEngine(Graph& g): model{g},  solve_status{false},
//         survivable_network{false}, iterations{0}{ }

        OptimizationEngine::OptimizationEngine(Graph& g): model{g} { }

// Packages
void OptimizationEngine::run() {
    if(!(solve_status = model.solve())) {
        return;
    }

    auto g = model.get_solution();
    auto constr_vars = graph_analyzer.find_all_violations(g);
    while(!constr_vars.empty()) {
        model.add_survivable_constraint(constr_vars);
        
        std::cout << "Количество ограничений: " << constr_vars.size() << '\n';

        iterations++;

        if(!(solve_status = model.solve())) {
            break;
        }
        g = model.get_solution();
        constr_vars = graph_analyzer.find_all_violations(g);
    }


    std::cout << "Мы вышли отсюдова" << '\n';
    // === ШАГ 2: Переход к точному целочисленному решению (MIP) ===
    std::cout << "--- LP фаза завершена. Переключение в MIP (Точное решение) ---\n";
    
    model.switch_model(); // Включаем целочисленность (добавляем conversions в CPLEX)

    // Пересчитываем модель уже в целых числах с учетом всех найденных LP-разрезов
    if (!(solve_status = model.solve())) {
        return;
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