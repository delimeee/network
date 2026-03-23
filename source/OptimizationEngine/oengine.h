#ifndef OENGINE_H
#define OENGINE_H
#include "../OptimizationModel/omodel.h"
#include "../GraphAnalyzer/ganalyzer.h"
#include "../Graph/graph.h"


class OptimizationEngine {
    private:
        OptimizationModel model;
        GraphAnalyzer graph_analyzer;
        Graph& g;

    public:
        OptimizationEngine(Graph&);
        void run();
};

#endif