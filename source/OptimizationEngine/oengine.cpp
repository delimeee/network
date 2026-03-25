#include "oengine.h"
#include<iostream>

OptimizationEngine::OptimizationEngine(Graph& g): model{g} { }

void OptimizationEngine::run() {
    if(!model.solve()) {
        std::cout << "Задача не решилась" << std::endl;
        return;
    }
}

Graph OptimizationEngine::get_solution() {
    return model.get_solution();
}