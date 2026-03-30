#include "ganalyzer.h"
#include <vector>

std::vector<size_t> byte_mask_vector(size_t n);

bool GraphAnalyzer::is_survivable(Graph& g) {
    std::vector<size_t> cities;
    std::vector<size_t> ps;
    for(size_t i = 0; i != g.size(); ++i) {
        if(g[i].type == NodeType::City) {
            cities.push_back(i);
        } else {
            ps.push_back(i);
        }
    }

    for(size_t i = 0; i != ps.size(); i++) {

        std::vector<size_t> sub_ps;
        size_t byte_mask = i;

        for(size_t j = 0; j != cities.size(); ++i) {

        }
    }
}

