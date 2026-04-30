#ifndef GRAPH_ANALYZER_H
#define GRAPH_ANALYZER_H
#include "../Graph/graph.h"
#include<unordered_set>

class GraphAnalyzer{
    std::pair<std::unordered_set<size_t>, double> validate_solution(Graph&);
};

#endif