// ============================== EQUAL RANGE ============================== //
// Project: The Experimental Bit Algorithms Library
// Name: equal_range.hpp
// Description: bit_iterator overloads for std::equal_range
// Creator: Vincent Reverdy
// Contributor(s): 
// License: BSD 3-Clause License
// ========================================================================== //
#ifndef _EQUAL_RANGE_HPP_INCLUDED
#define _EQUAL_RANGE_HPP_INCLUDED
// ========================================================================== //

// ============================== PREAMBLE ================================== //
// C++ standard library
// Project sources
// Third-party libraries
// Miscellaneous

namespace bit {
// ========================================================================== //



// Status: to do 
template <class ForwardIt, class T>
constexpr std::pair<bit_iterator<ForwardIt>, bit_iterator<ForwardIt>> equal_range (
    bit_iterator<ForwardIt> first, bit_iterator<ForwardIt> last, const T& value) {
    (value);
    return std::make_pair(first, last);
}

// Status: on hold 
template <class ForwardIt, class T, class Compare>
constexpr std::pair<bit_iterator<ForwardIt>, bit_iterator<ForwardIt>> equal_range ( 
    bit_iterator<ForwardIt> first, bit_iterator<ForwardIt> last, const T& value,
    Compare comp) {
    (value, comp); 
    return std::make_pair(first, last);
}

// ========================================================================== //
} // namespace bit

#endif // _EQUAL_RANGE_HPP_INCLUDED
// ========================================================================== //
