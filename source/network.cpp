#include<iostream>
#include "OptimizationEngine/oengine.h"
#include "Graph/graph.h"
#include <fstream>
#define OUTPUT_DIR "output/"

void write_graph(Graph& g);

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
                graph(i, j).cost = 1;
                graph(i, j).max_flow = 1000;
            } else {
                graph(i, j).cost = 1.e8;
                graph(i, j).max_flow = 0;
            } 
        }
    }

    OptimizationEngine eng(graph);
    eng.run();

    Graph g = eng.get_solution();
    for(size_t i = 0; i != g.size(); ++i) {
        for(size_t j = 0; j != g.size(); ++j) {
            std::cout << g(i, j).value << ';' << g(i, j).flow << ' ';
        }
        std::cout << std::endl;
    }

    write_graph(g);    
}

void write_graph(Graph& g) {
    std::ofstream file(std::string`(OUTPUT_DIR) + "output.txt");
    for(size_t i = 0; i != g.size(); ++i) {
        file << g[i].x << ';' << g[i].y << ';' << g[i].demand << ';';
        if(g[i].type == NodeType::City) {
            file << "City";
        } else {
            file << "Station";
        }

        if(i != g.size() - 1) {
            file << ' ';
        }
    }
    file << std::endl;
    for(size_t i = 0; i != g.size(); ++i) {
        bool writed = false;
        for(size_t j = 0; j != g.size(); ++j) {
            if(g(i, j).value != 0) {
                if(writed) {
                    file << ' ';
                }
                writed = true;
                file << j << ';' << g(i, j).value << ';' << g(i, j).flow;
            }
        }
        file << std::endl;
    }
}