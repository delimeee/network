#include "ganalyzer.h"
#include <vector>
#include <cstdint>

uint64_t size_byte_mask(size_t i);
std::vector<size_t> byte_mask_vector(size_t n);
bool num_in_sub_array(size_t n, uint64_t mask);
double count_y(const Graph& g,  const std::vector<size_t>& nums, uint64_t byte_mask_ps, uint64_t byte_mask_cities);

bool GraphAnalyzer::is_survivable(Graph& g) {
    std::vector<size_t> cities;
    std::vector<size_t> ps;
    std::vector<size_t> nums(g.size());
    for(size_t i = 0; i != g.size(); ++i) {
        if(g[i].type == NodeType::City) {
            cities.push_back(i);
            nums[i] = cities.size() - 1;
        } else {
            ps.push_back(i);
            nums[i] = ps.size() - 1;
        }
    }

    uint64_t byte_mask_ps = size_byte_mask(ps.size());
    for(size_t i = byte_mask_ps; i >= 0; i--) {

        uint64_t byte_mask_cities = size_byte_mask(cities.size());
        for(size_t j = byte_mask_cities; j >= 0; j--) {

        }
    }
}


uint64_t size_byte_mask(size_t n) {
    uint64_t mask = 0;
    for(size_t i = 0; i != n; ++i) {
        mask <<= 1;
        mask += 1;
    }
    return mask;
}

bool num_in_sub_array(size_t n, uint64_t mask) {
    return (mask >> n) & 1 == 1;
}

double count_y(const Graph& g, const std::vector<size_t>& nums, uint64_t byte_mask_ps, uint64_t byte_mask_cities) {
    double count = 0;
    for(size_t i = 0; i != g.size(); ++i) {
        bool is_in_cities =  
        if() {
            continue;
        }

        for(size_t j = 0; j != g.size(); ++j) {
            if(num_in_sub_array(j, mask)) {
                continue;
            }

            count += g(i, j).value;
        }
    }

    return count;
}

double count_demands(Graph& g, uint64_t mask) {

}