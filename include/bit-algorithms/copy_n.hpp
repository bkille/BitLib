// ================================= COPY N ================================= //
// Project: The Experimental Bit Algorithms Library
// Name: copy_n.hpp
// Description: bit_iterator overloads for std::copy_n
// Creator: Vincent Reverdy
// Contributor(s): 
// License: BSD 3-Clause License
// ========================================================================== //
#ifndef _COPY_N_HPP_INCLUDED
#define _COPY_N_HPP_INCLUDED
// ========================================================================== //

// ============================== PREAMBLE ================================== //
// C++ standard library
// Project sources
// Third-party libraries
// Miscellaneous

namespace bit {
// ========================================================================== //



// Status: to do
template <class InputIt, class Size, class OutputIt>
constexpr bit_iterator<OutputIt> copy_n(bit_iterator<InputIt> first,
    Size count, bit_iterator<OutputIt> result) {
    (first, count);
    return result;
}

// Status: to do
template <class ExecutionPolicy, class ForwardIt1, class Size, class ForwardIt2>
bit_iterator<ForwardIt2> copy_n(ExecutionPolicy&& policy, 
    bit_iterator<ForwardIt1> first, Size count, bit_iterator<ForwardIt2> result) {
    (policy, first, count);
    return result;
}

// ========================================================================== //
} // namespace bit

#endif // _COPY_N_HPP_INCLUDED
// ========================================================================== //
