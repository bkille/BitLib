// ================================= ALL OF ================================= //
// Project: The Experimental Bit Algorithms Library
// Name: all_of.hpp
// Description: bit_iterator overloads for std::all_of
// Creator: Vincent Reverdy
// Contributor(s): 
// License: BSD 3-Clause License
// ========================================================================== //
#ifndef _ALL_OF_HPP_INCLUDED
#define _ALL_OF_HPP_INCLUDED
// ========================================================================== //

// ============================== PREAMBLE ================================== //
// C++ standard library
// Project sources
// Third-party libraries
// Miscellaneous

namespace bit {

// Status: on hold (UnaryPredicate)
template <class InputIt, class UnaryPredicate>
constexpr bool all_of(bit_iterator<InputIt> first, bit_iterator<InputIt> last,
    UnaryPredicate p) {
    return std::all_of(first, last, p);
}

// Status: on hold (UnaryPredicate)
template <class ExecutionPolicy, class ForwardIt, class UnaryPredicate>
bool all_of(ExecutionPolicy&& policy, bit_iterator<ForwardIt> first,
    bit_iterator<ForwardIt> last, UnaryPredicate p) {
    return std::all_of(std::forward<ExecutionPolicy>(policy), first, last, p);
}

// ========================================================================== //
} // namespace bit

#endif // _ALL_OF_HPP_INCLUDED
// ========================================================================== //
