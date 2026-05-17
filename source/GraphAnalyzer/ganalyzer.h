#ifndef GRAPH_ANALYZER_H
#define GRAPH_ANALYZER_H
#include "../Graph/graph.h"
#include<unordered_set>
#include<optional>

class GraphAnalyzer{
    public:
        std::optional<std::pair<std::unordered_set<size_t>, double>> find_violation(Graph&);
        std::vector<std::pair<std::unordered_set<size_t>, double>> find_all_violations(Graph& g);
        bool validate_solution(const Graph& g);
};

#endif