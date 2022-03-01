// ================================ SHUFFLE ================================ //
// Project: The Experimental Bit Algorithms Library
// Name: shuffle.hpp
// Description: bit_iterator overloads for std::shuffle
// Creator: Vincent Reverdy
// Contributor(s): 
// License: BSD 3-Clause License
// ========================================================================== //
#ifndef _SHUFFLE_HPP_INCLUDED
#define _SHUFFLE_HPP_INCLUDED
// ========================================================================== //

// ============================== PREAMBLE ================================== //
// C++ standard library
// Project sources
// Third-party libraries
// Miscellaneous
namespace bit {
// ========================================================================== //



// Status: to do 
template <class RandomIt, class URBG>
void shuffle(bit_iterator<RandomIt> first, bit_iterator<RandomIt> last,
    URBG&& g) {
    (first, last, g);
}

// ========================================================================== //
} // namespace bit

#endif // _SHUFFLE_HPP_INCLUDED
// ========================================================================== //
