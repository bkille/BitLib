// ================================ FILL N ================================== //
// Project: The Experimental Bit Algorithms Library
// Name: fill_n.hpp
// Description: bit_iterator overloads for std::fill_n
// Creator: Vincent Reverdy
// Contributor(s): Vincent Reverdy [2019]
//                 Collin Gress [2019]
// License: BSD 3-Clause License
// ========================================================================== //
#ifndef _FILL_N_HPP_INCLUDED
#define _FILL_N_HPP_INCLUDED
// ========================================================================== //

// ============================== PREAMBLE ================================== //
// C++ standard library
// Project sources
// Third-party libraries
// Miscellaneous

namespace bit {
// ========================================================================== //



// Status: to do
template <class OutputIt, class Size>
constexpr bit_iterator<OutputIt> fill_n(bit_iterator<OutputIt> first, Size count, 
    const bit_value& bv) {
  bit_iterator<OutputIt> last = first + count;
  bit::fill(first, last, bv); 
}

// Status: to do
template <class ExecutionPolicy, class ForwardIt, class Size>
bit_iterator<ForwardIt> fill_n(ExecutionPolicy&& policy, 
    bit_iterator<ForwardIt> first, Size count, const bit_value& bv) {
  (bv, policy, count);
  return first;
}

// ========================================================================== //
} // namespace bit

#endif // _FILL_N_HPP_INCLUDED
// ========================================================================== //
