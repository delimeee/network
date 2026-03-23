#include "oengine.h"
#include<iostream>

OptimizationEngine::OptimizationEngine(Graph& g): model{g} { }

void OptimizationEngine::run() {
    if(!model.solve()) {
        std::cout << "Задача не решилась" << std::endl;
        throw std::runtime_error("ЗАДАЧА НЕ РЕШЕНА!!!!");
    }

    Graph g = model.get_solution();
    for(size_t i = 0; i != g.size(); ++i) {
        for(size_t j = 0; j != g.size(); ++j) {
            std::cout << g(i, j).value << ';' << g(i, j).flow << ' ';
        }
        std::cout << std::endl;
    }
}