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

double rvalue_constraint_second(Graph& g, std::unordered_set<size_t>& sub_graph) {
    double production{0};
    double consumption{0};
    for(auto& v : sub_graph) {
        if(g[v].type == NodeType::PowerStation) {
            production += g[v].demand;
        } else {
            consumption += g[v].demand;
        }
    }
    double numerator = production - consumption;
    if(numerator < 0.0) {
        return std::ceil(-numerator / MAX_FLOW) + 1;
    }
    return -std::ceil(numerator / MAX_FLOW) + 1;
}

std::optional<std::pair<std::unordered_set<size_t>, double>> heuristic(
    Graph& g,
    std::unordered_set<size_t> checked
) {
    double lval = lvalue_constraint(g, checked);
    double rval = rvalue_constraint(g, checked);
    double rval_second = rvalue_constraint_second(g, checked);
    // if(std::abs(lval - rval) >= ERROR && lval < rval) {
    //     return std::make_optional(std::pair{checked, rval});
    // }
    if(
        std::abs(lval - std::max(rval, rval_second)) >= ERROR 
        && lval < std::max(rval, rval_second)
    ) {
        return std::make_optional(std::pair{checked, std::max(rval, rval_second)});
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

std::vector<std::pair<std::unordered_set<size_t>, double>> many_bfs(
    Graph& g,
    size_t start,
    bool many_ps=true
) {
    std::vector<std::pair<std::unordered_set<size_t>, double>> result;
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
                result.push_back(res.value());
            }
        }
    }

    return result;
}

std::vector<std::pair<std::unordered_set<size_t>, double>> many_dfs(
    Graph& g,
    size_t cur,
    std::unordered_set<size_t>& checked,
    bool many_ps=true
) {
    std::vector<std::pair<std::unordered_set<size_t>, double>> result;
    checked.insert(cur);

    auto res = heuristic(g, checked);
    if(res.has_value()) {
        result.push_back(res.value());
    }
    
    for(size_t i = 0; i != g.size(); ++i) {
        if(
            !checked.contains(i) &&
            g(cur, i).value > ERROR &&
            (many_ps || g[i].type == NodeType::City)
        ) {
            auto res = dfs(g, i, checked);
            if(res.has_value()) {
                result.push_back(res.value());
            }
        }
    }

    return result;
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
        auto res = dfs(g, station, checked);
        if (res.has_value()) {
            return res;
        }
    }
    for(auto& station: ps) {
        auto res = bfs(g, station, false);
        if (res.has_value()) {
            return res;
        }
    }
    for(auto& station: ps) {
        auto res = bfs(g, station, true);
        if (res.has_value()) {
            return res;
        }
    }
    for(auto& station: ps) {
        std::unordered_set<size_t> checked;
        auto res = dfs(g, station, checked);
        if (res.has_value()) {
            return res;
        }
    }
    return std::nullopt;
}

std::vector<std::pair<std::unordered_set<size_t>, double>> 
GraphAnalyzer::many_validate(Graph& g) {
    std::unordered_set<size_t> ps;
    for(size_t i = 0; i != g.size(); ++i) {
        if(g[i].type == NodeType::PowerStation) {
            ps.insert(i);
        }
    }
    std::vector<std::pair<std::unordered_set<size_t>, double>> constrains;
    for(auto& station: ps) {
        std::unordered_set<size_t> checked;
        constrains.append_range(many_dfs(g, station, checked));
    }
    for(auto& station: ps) {
        std::unordered_set<size_t> checked;
        constrains.append_range(many_dfs(g, station, checked, false));
    }
    for(auto& station: ps) {
        constrains.append_range(many_bfs(g, station));
    }
    for(auto& station: ps) {
        constrains.append_range(many_bfs(g, station, false));
    }

    return constrains;
}