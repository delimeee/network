#include "ganalyzer.h"
#include "../Graph/graph.h"
#include <vector>
#include <cstdint>
#include <queue>
#include <cmath>
#include <unordered_set>

bool check_surv_constraint(const Graph& g) {
    
}

void bfs(const Graph& g, int cur) {
    std::queue<size_t> q;
    q.push(cur);

    std::vector<size_t> d(g.size(), -1);
    d[cur] = 0;

    std::unordered_set<size_t> sub_graph;
    sub_graph.insert(cur);

    while(!q.empty()) {
        size_t v = q.front();
        q.pop();
        for(size_t i = 0; i != g.size(); ++i) {
            if(!(g(v, i).value > ERROR && d[i] != -1)) {
                continue;
            }

            q.push(i);
            d[i] = d[v] + 1;
            sub_graph.insert(i);

            if(!check_surv_constraint(g, sub_graph)) {

            }
        }
    }
}


bool GraphAnalyzer::is_survivable(Graph& g) {
    
}
