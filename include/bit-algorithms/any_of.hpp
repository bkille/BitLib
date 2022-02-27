// ================================= ANY OF ================================= //
// Project: The Experimental Bit Algorithms Library
// Name: any_of.hpp
// Description: bit_iterator overloads for std::any_of
// Creator: Vincent Reverdy
// Contributor(s): 
// License: BSD 3-Clause License
// ========================================================================== //
#ifndef _ANY_OF_HPP_INCLUDED
#define _ANY_OF_HPP_INCLUDED
// ========================================================================== //

// ============================== PREAMBLE ================================== //
// C++ standard library
// Project sources
// Third-party libraries
// Miscellaneous

namespace bit {

// Status: on hold (UnaryPredicate)
template <class InputIt, class UnaryPredicate>
constexpr bool any_of(bit_iterator<InputIt> first, bit_iterator<InputIt> last,
    UnaryPredicate p) {
    return std::any_of(first, last, p);
}

// Status: on hold (UnaryPredicate)
template <class ExecutionPolicy, class ForwardIt, class UnaryPredicate>
constexpr bool any_of(ExecutionPolicy&& policy, bit_iterator<ForwardIt> first,
    bit_iterator<ForwardIt> last, UnaryPredicate p) {
    return std::any_of(std::forward<ExecutionPolicy>(policy), first, last, p);
}

// ========================================================================== //
} // namespace bit

#endif // _ANY_OF_HPP_INCLUDED
// ========================================================================== //
