#include <iostream>
#include "bit-iterator/bit.hpp"
#include "bit-algorithms/bit_algorithm.hpp"
#include "bit-containers/bitvector.hpp"


int main() {
    bit::bit_vector<unsigned char> bv1 {"011111010010"};
    std::cout << "Original bitvec:  " << bv1.debug_string() << std::endl;
    bit::reverse(bv1.begin(), bv1.end());
    std::cout << "Reversed bitvec:  " << bv1.debug_string() << std::endl;
    bit::rotate(bv1.begin(), bv1.begin() + 3, bv1.end());
    std::cout << "Rotated bitvec:   " << bv1.debug_string() << std::endl;
    bv1.push_back(bit::bit0);
    bv1.insert(bv1.end(), 10, bit::bit1);
    std::cout << "Extended bitvec:  " << bv1.debug_string() << std::endl;
    return 0;
}
