// ============================= COPY BACKWARD ============================= //
// Project: The Experimental Bit Algorithms Library
// Name: copy_backward.hpp
// Description: bit_iterator overloads for std::copy_backward
// Creator: Vincent Reverdy
// Contributor(s): 
// License: BSD 3-Clause License
// ========================================================================== //
#ifndef _COPY_BACKWARD_HPP_INCLUDED
#define _COPY_BACKWARD_HPP_INCLUDED
// ========================================================================== //

// ============================== PREAMBLE ================================== //
// C++ standard library
// Project sources
// Third-party libraries
// Miscellaneous

namespace bit {
// ========================================================================== //



// Status: to do
template <class BidirIt1, class BidirIt2>
constexpr bit_iterator<BidirIt2> copy_backward(bit_iterator<BidirIt1> first,
    bit_iterator<BidirIt1> last, bit_iterator<BidirIt2> d_last) {
    (first, last);
    return d_last;
}

// ========================================================================== //
} // namespace bit

#endif // _COPY_BACKWARD_HPP_INCLUDED
// ========================================================================== //
