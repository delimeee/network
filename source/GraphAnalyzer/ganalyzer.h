#ifndef GRAPH_ANALYZER_H
#define GRAPH_ANALYZER_H
#include "../Graph/graph.h"

class GraphAnalyzer{
    bool is_connected(Graph&);
    bool is_survivable(Graph&);
};

#endif