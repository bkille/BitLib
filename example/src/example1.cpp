#include <iostream>
#include <string>
#include "bitlib/bitlib.hpp"

int main() {
    bit::bit_vector<unsigned char> bv ("011111010010");
    std::cout << "Original bitvec:  " << bv.debug_string() << std::endl;
    // Original bitvec:  01111101 0010

    // Same behavior as std::reverse
    bit::reverse(bv.begin(), bv.end());
    std::cout << "Reversed bitvec:  " << bv.debug_string() << std::endl;
    // Reversed bitvec:  01001011 1110

    // Same behavior as std::rotate
    bit::rotate(bv.begin(), bv.begin() + 3, bv.end());
    std::cout << "Rotated bitvec:   " << bv.debug_string() << std::endl;
    // Rotated bitvec:   01011111 0010

    // Same behavior as the corresponding std::vector::push_back and std::vector::insert
    bv.push_back(bit::bit0);
    bv.insert(bv.end(), 10, bit::bit1);
    std::cout << "Extended bitvec:  " << bv.debug_string() << std::endl;
    // Extended bitvec:  01011111 00100111 1111111

    return 0;
}
