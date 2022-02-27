// ============================== REVERSE COPY ============================== //
// Project: The Experimental Bit Algorithms Library
// Name: reverse_copy.hpp
// Description: bit_iterator overloads for std::reverse_copy
// Creator: Vincent Reverdy
// Contributor(s): 
// License: BSD 3-Clause License
// ========================================================================== //
#ifndef _REVERSE_COPY_HPP_INCLUDED
#define _REVERSE_COPY_HPP_INCLUDED
// ========================================================================== //

// ============================== PREAMBLE ================================== //
// C++ standard library
// Project sources
// Third-party libraries
// Miscellaneous
namespace bit {
// ========================================================================== //



// Status: to do
template <class BidirIt, class OutputIt>
bit_iterator<OutputIt> reverse_copy(bit_iterator<BidirIt> first,
    bit_iterator<BidirIt> last, bit_iterator<OutputIt> d_first) {
    (first, last); 
    return d_first;
}

// Status: to do
template <class ExecutionPolicy, class BidirIt, class ForwardIt>
bit_iterator<ForwardIt> reverse_copy(ExecutionPolicy&& policy,
    bit_iterator<BidirIt> first, bit_iterator<BidirIt> last,
    bit_iterator<ForwardIt> d_first) {
    (policy, first, last);
    return d_first;
}

// ========================================================================== //
} // namespace bit

#endif // _REVERSE_COPY_HPP_INCLUDED
// ========================================================================== //
