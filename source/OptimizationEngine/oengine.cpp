#include "oengine.h"
#include<iostream>

OptimizationEngine::OptimizationEngine(Graph& g): model{g} { }

void OptimizationEngine::run() {
    if(!model.solve()) {
        std::cout << "Почему она не решается то" << std::endl;
        return;
    }
}

Graph OptimizationEngine::get_solution() {
    return model.get_solution();
}