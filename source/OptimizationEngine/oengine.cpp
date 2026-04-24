#include "oengine.h"
#include<iostream>

OptimizationEngine::OptimizationEngine(Graph& g): model{g}, status{false}{ }

void OptimizationEngine::run() {
    if(!(status = model.solve())) {
        return;
    }

    Graph g = model.get_solution();



}

Graph OptimizationEngine::get_solution() {
    if(status) {
        return model.get_solution();
    } else {
        return Graph();
    }
}