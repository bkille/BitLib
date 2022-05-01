#include <iostream>
#include "bitlib/bitlib.hpp"

template<typename WordType>
void flip_bits(bit::bit_vector<WordType>& bvec) {
    for (bit::bit_reference<WordType> bval :  bvec) {
        bval = ~bval;
    }
    return;
}

template<typename WordType>
void bit_pointers(bit::bit_vector<WordType>& bvec) {
    bit::bit_pointer<WordType> p = &bvec[0];
    *p = bit::bit1;
    return;
}

int main() {
    bit::bit_vector<unsigned char> bvec("111000110010");
    std::cout << bvec.debug_string() << std::endl;
    flip_bits(bvec);
    std::cout << bvec.debug_string() << std::endl;
    bit_pointers(bvec);
    std::cout << bvec.debug_string() << std::endl;
    return 0;
}
