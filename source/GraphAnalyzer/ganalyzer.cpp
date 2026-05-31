#include "ganalyzer.h"
#include "../Graph/graph.h"
#include "../constants.h"
#include "../FlowModel/fmodel.h"
#include<vector>
#include<cmath>
#include<iostream>
#include<algorithm>

double GraphAnalyzer::lvalue_constraint(Graph& g, std::unordered_set<size_t>& sub_graph) {
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

double GraphAnalyzer::rvalue_constraint_station(Graph& g, std::unordered_set<size_t>& sub_graph) {
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

double GraphAnalyzer::rvalue_constraint_line(Graph& g, std::unordered_set<size_t>& sub_graph) {
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

std::optional<std::pair<std::unordered_set<size_t>, double>> GraphAnalyzer::heuristic(
    Graph& g,
    std::unordered_set<size_t> checked
) {
    double lval = lvalue_constraint(g, checked);
    double rval = rvalue_constraint_station(g, checked);
    double rval_second = rvalue_constraint_line(g, checked);
    if(
        std::abs(lval - std::max(rval, rval_second)) >= ERROR 
        && lval < std::max(rval, rval_second)
    ) {
        return std::make_optional(std::pair{checked, std::max(rval, rval_second)});
    }
    return std::nullopt;
}

double GraphAnalyzer::calc_connectivity(
    Graph& g,
    std::unordered_set<size_t> subset,
    size_t cur 
) {
    if(subset.contains(cur)) {
        return 0.0;
    }

    double result{0.0};
    for(size_t i = 0; i != g.size(); ++i) {
        if(subset.contains(i) && g(cur, i).value >= ERROR) {
            result += g(cur, i).value;
        }
    }

    return result;
}

std::vector<std::pair<std::unordered_set<size_t>, double>> GraphAnalyzer::greedy_best_first_search(
    Graph& g,
    size_t start
) {
    std::vector<std::pair<std::unordered_set<size_t>, double>> result;

    std::unordered_set<size_t> subset{start};
    std::vector<std::pair<size_t, double>> connectivity;
    connectivity.reserve(g.size() - 1);
    for(size_t i = 0; i != g.size(); ++i) {
        if(i != start) {
            connectivity.emplace_back(i, calc_connectivity(g, subset, i));
        }  
    }

    auto res = heuristic(g, subset);
        if(res.has_value()) {
            result.push_back(res.value());
        }
    
    while (subset.size() < g.size()) {
        for(auto& [num, conn]: connectivity) {
            conn = calc_connectivity(g, subset, num);
        }
        std::sort(connectivity.begin(), connectivity.end(), 
        [](const auto& a, const auto& b) {
            if (std::abs(a.second - b.second) > ERROR) { 
                return a.second < b.second;
            }
            return a.first < b.first; 
        });

        auto [best, best_val] = connectivity.back();
        subset.insert(best);
        connectivity.pop_back();

        auto res = heuristic(g, subset);
        if(res.has_value()) {
            result.push_back(res.value());
        }

    }

    return result;
}

std::vector<std::pair<std::unordered_set<size_t>, double>> 
GraphAnalyzer::find_all_violations(Graph& g) {
    std::vector<std::pair<std::unordered_set<size_t>, double>> constrains;
    //New search
    for(size_t i = 0; i != g.size(); ++i) {
        constrains.append_range(greedy_best_first_search(g, i));
    }


    return constrains;
}

bool GraphAnalyzer::validate_solution(const Graph& g) {
    std::cout << "Валидируем решение" << '\n';
    for(size_t i = 0; i != g.size(); ++i) {
        if(g[i].type == NodeType::PowerStation) {
            std::cout << "Удаляем вершину: " << i + 1<< '\n';
            FlowModel model(g, {i});
            if(!model.solve()) {
                return false;
            }
        }
    }

    return true;
}