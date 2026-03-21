#include<iostream>
#include "OptimizationEngine/oengine.h"
#include "Graph/graph.h"

int main() {
    size_t nodes_amount;
    std::cin >> nodes_amount;

    Graph graph(nodes_amount);
    for(size_t i = 0; i != nodes_amount; ++i) {
        std::cin >> graph[i].x >> graph[i].y >> graph[i].demand;
        int type;
        if(std::cin >> type && type == 0) {
            graph[i].type = NodeType::PowerStation;
        } else {
            graph[i].type = NodeType::City;
        }
    }

    for(size_t i = 0; i != nodes_amount; ++i) {
        for(size_t j = 0; j != nodes_amount; ++j) {
            if(i != j) {
                std::cin >> graph(i, j).cost >> graph(i, j).max_flow;
            } else {
                graph(i, j).cost = 0;
                graph(i, j).max_flow = 0;
            } 
        }
    }

    OptimizationEngine eng(graph);
    eng.run();
}