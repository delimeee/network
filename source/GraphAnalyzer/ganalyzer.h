#ifndef GRAPH_ANALYZER_H
#define GRAPH_ANALYZER_H
#include "../Graph/graph.h"
#include<unordered_set>
#include<optional>

class GraphAnalyzer{
    public:
        std::optional<std::pair<std::unordered_set<size_t>, double>> validate_solution(Graph&);
        std::vector<std::pair<std::unordered_set<size_t>, double>> many_validate(Graph& g);
};

#endif