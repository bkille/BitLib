#include <iostream>
#include "bit-iterator/bit.hpp"
#include "bit-algorithms/bit_algorithm.hpp"
#include "bit-containers/bitvector.hpp"


int main() {
    bit::bit_vector<unsigned char> bv1 {"01001010010"};
    std::cout << bv1 << std::endl; 
    return 0;
}
