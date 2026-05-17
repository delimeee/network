#include<iostream>
#include <iomanip>
#include <sstream>
#include "OptimizationEngine/oengine.h"
#include "Graph/graph.h"
#include "constants.h"
#include <fstream>

void write_graph(const Graph& g);
void print_graph(const Graph& g);

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
                graph(i, j).max_flow = MAX_FLOW;
            } else {
                graph(i, j).cost = 1.e8;
                graph(i, j).max_flow = 0;
            } 
        }
    }

    OptimizationEngine eng(graph);
    eng.run();

    std::cout << "Пройдено итераций: " << eng.get_iterations() << '\n'; 

    Graph g = eng.get_solution();
    if(g.size() == 0) {
        std::cout << "Задача не решилась" << std::endl;
        return 0;
    }

    if(eng.get_survivable_network()) {
        std::cout << "Решение надёжное" << std::endl;
    } else {
        std::cout << "Решение НЕНАДЁЖНОЕ" << std::endl;
    }
    print_graph(g);
    write_graph(g);    
}

void print_graph(const Graph& g) {
    for (int i = 0; i < g.size(); ++i) {
        std::cout << i + 1 << ' ';
        for (int j = 0; j < g.size(); ++j) {
            std::cout << g(i, j).value << ';' << g(i, j).flow << ' ';
        }
        std::cout << "\n";
    }
}

void write_graph(const Graph& g) {
    std::ofstream file(std::string(OUTPUT_DIR) + "output.txt");
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