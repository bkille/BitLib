// ============================ NEXT PERMUTATION ============================ //
// Project: The Experimental Bit Algorithms Library
// Name: next_permutation.hpp
// Description: bit_iterator overloads for std::next_permutation
// Creator: Vincent Reverdy
// Contributor(s): 
// License: BSD 3-Clause License
// ========================================================================== //
#ifndef _NEXT_PERMUTATION_HPP_INCLUDED
#define _NEXT_PERMUTATION_HPP_INCLUDED
// ========================================================================== //

// ============================== PREAMBLE ================================== //
// C++ standard library
// Project sources
// Third-party libraries
// Miscellaneous

namespace bit {
// ========================================================================== //



// Status: to do
template <class BidirIt>
constexpr bool next_permutation(bit_iterator<BidirIt> first, 
    bit_iterator<BidirIt> last) {
    (first, last);
    return true;
}

// Status: on hold
template <class BidirIt, class Compare>
constexpr bool next_permutation(bit_iterator<BidirIt> first, 
    bit_iterator<BidirIt> last, Compare comp) {
    (first, last, comp);
    return true;
} 

// ========================================================================== //
} // namespace bit

#endif // _NEXT_PERMUTATION_HPP_INCLUDED
// ========================================================================== //
