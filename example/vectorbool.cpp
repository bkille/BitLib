#include <iostream>
#include "bitlib/bitlib.hpp"

// Here are a couple examples of what BitLib can accomplish that std::vector<bool> can not. While 
// we still have to explicitly define the type as either reference or pointer, they at least compile
// and behave as expected
//
// It should be noted that I am not responsible for the creation of this aspect of the library, 
// the bit_iterator/ is thanks to Dr. Vincent Reverdy.

template<typename WordType>
void flip_bits(bit::bit_vector<WordType>& bvec) {
    // Unable to take references to bool, but it works for bits!
    for (bit::bit_reference<WordType> bval :  bvec) {
        bval = ~bval;
    }
    return;
}

template<typename WordType>
void bit_pointers(bit::bit_vector<WordType>& bvec) {
    // Unable to take pointers to bool, but it works for bits!
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
