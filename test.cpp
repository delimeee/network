#include<iostream>
#include<cstdint>
#include<bitset>
#include <climits>

uint64_t size_byte_mask(size_t n) {
    uint64_t mask = 0;
    for(size_t i = 0; i != n; ++i) {
        mask <<= 1;
        mask += 1;
    }
    return mask;
}

int main() {
    size_t i;
    std::cin >> i;
    std::cout << std::bitset<sizeof(uint64_t) * CHAR_BIT>(size_byte_mask(i));
}