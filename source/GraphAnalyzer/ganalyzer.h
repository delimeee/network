#ifndef GRAPH_ANALYZER_H
#define GRAPH_ANALYZER_H
#include "../Graph/graph.h"
#include<unordered_set>
#include<optional>

class GraphAnalyzer{
    public:
        std::vector<std::pair<std::unordered_set<size_t>, double>> find_all_violations(Graph& g);
        bool validate_solution(const Graph& g);

    private:
        double lvalue_constraint(Graph& g, std::unordered_set<size_t>& sub_graph);
        double rvalue_constraint_station(Graph& g, std::unordered_set<size_t>& sub_graph);
        double rvalue_constraint_line(Graph& g, std::unordered_set<size_t>& sub_graph);
        std::optional<std::pair<std::unordered_set<size_t>, double>> heuristic(
            Graph& g,
            std::unordered_set<size_t> checked
        );
        double calc_connectivity(
            Graph& g,
            std::unordered_set<size_t> subset,
            size_t cur 
        );
        std::vector<std::pair<std::unordered_set<size_t>, double>> greedy_best_first_search(
            Graph& g,
            size_t start
        );
};

#endif