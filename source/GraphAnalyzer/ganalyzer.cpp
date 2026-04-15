#include "ganalyzer.h"
#include "../Graph/graph.h"
#include <vector>
#include <cstdint>
#include <queue>
#include <cmath>
#include <unordered_set>
#define MAX_FLOW 1000.0
#define ERROR 1.e-8

bool check_surv_constraint(const Graph& g, const std::unordered_set<size_t>& sub_g) {
    double sum_y = 0;
    double sum_p = 0;
    double sum_d = 0;
    double max_p = -1;
    for(auto& cur: sub_g) {
        for(size_t i = 0; i != g.size(); ++i) {
            if(g(cur, i).value > ERROR && !sub_g.contains(i)) {
                sum_y += g(cur, i).value;
                if(g[i].type == NodeType::PowerStation) {
                    sum_p += g[i].demand;
                    max_p = std::max(max_p, sum_p);
                }
                else if(g[i].type == NodeType::City) sum_d += g[i].demand;
            }
        }
    }

    return sum_y >= - ceil((sum_p - sum_d - max_p)) / MAX_FLOW;
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
