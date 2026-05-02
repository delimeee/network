#include "ganalyzer.h"
#include "../Graph/graph.h"
#include "../constants.h"
#include<queue>
#include<vector>
#include<cmath>
#include<iostream>

double lvalue_constraint(Graph& g, std::unordered_set<size_t>& sub_graph) {
    double result = 0;
    for(auto& v : sub_graph) {
        for(size_t i = 0; i != g.size(); ++i) {
            if(g(i, v).value > ERROR && !sub_graph.contains(i)) {
                result += g(i, v).value;
            }
        }
    }
    return result;
}

double rvalue_constraint(Graph& g, std::unordered_set<size_t>& sub_graph) {
    double production{0};
    double consumption{0};
    double max_production{0};
    for(auto& v : sub_graph) {
        if(g[v].type == NodeType::PowerStation) {
            max_production = std::max(max_production, g[v].demand);
            production += g[v].demand;
        } else {
            consumption += g[v].demand;
        }
    }
    double numerator = production - consumption - max_production;
    if(numerator < 0.0) {
        return std::ceil(-numerator / MAX_FLOW);
    }
    return -std::ceil(numerator / MAX_FLOW);
}

std::optional<std::pair<std::unordered_set<size_t>, double>> heuristic(
    Graph& g,
    std::unordered_set<size_t> checked
) {
    double lval = lvalue_constraint(g, checked);
    double rval = rvalue_constraint(g, checked);
    if(std::abs(lval - rval) >= ERROR && lval < rval) {
        return std::make_optional(std::pair{checked, rval});
    }
    return std::nullopt;
}

std::optional<std::pair<std::unordered_set<size_t>, double>> bfs(
    Graph& g,
    size_t start,
    bool many_ps=true
) {
    std::queue<size_t> q;
    std::unordered_set<size_t> checked;
    checked.insert(start);
    q.push(start);

    while (!q.empty()) {
        size_t v = q.front();
        q.pop();
        for (size_t i = 0; i != g.size(); ++i) {
            if(
                g(v, i).value <= ERROR ||
                (!many_ps && g[i].type == NodeType::PowerStation)
            ) {
                continue;
            } if(!checked.contains(i)) {
                checked.insert(i);
                q.push(i);
            }

            auto res = heuristic(g, checked);
            if(res.has_value()) {
                return res;
            }
        }
    }

    return std::nullopt;
}

std::optional<std::pair<std::unordered_set<size_t>, double>> dfs(
    Graph& g,
    size_t cur,
    std::unordered_set<size_t>& checked,
    bool many_ps=true
) {

    checked.insert(cur);

    auto res = heuristic(g, checked);
    if(res.has_value()) {
        return res;
    }
    
    for(size_t i = 0; i != g.size(); ++i) {
        if(
            !checked.contains(i) &&
            g(cur, i).value > ERROR &&
            (many_ps || g[i].type == NodeType::City)
        ) {
            auto res = dfs(g, i, checked);
            if(res.has_value()) {
                return res;
            }
        }
    }

    return std::nullopt;
}

std::optional<std::pair<std::unordered_set<size_t>, double>> GraphAnalyzer::validate_solution(Graph& g) {
    std::unordered_set<size_t> ps;
    for(size_t i = 0; i != g.size(); ++i) {
        if(g[i].type == NodeType::PowerStation) {
            ps.insert(i);
        }
    }
    for(auto& station: ps) {
        std::unordered_set<size_t> checked;
        auto res = dfs(g, station, checked, false);
        if (res.has_value()) {
            return res;
        }
        res = bfs(g, station, false);
        if (res.has_value()) {
            return res;
        }
        res = bfs(g, station, true);
        if (res.has_value()) {
            return res;
        }
        res = dfs(g, station, checked);
        if (res.has_value()) {
            return res;
        }
    }
    
    return std::nullopt;
}