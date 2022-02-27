// ================================ NONE OF ================================ //
// Project: The Experimental Bit Algorithms Library
// Name: none_of.hpp
// Description: bit_iterator overloads for std::none_of
// Creator: Vincent Reverdy
// Contributor(s): 
// License: BSD 3-Clause License
// ========================================================================== //
#ifndef _NONE_OF_HPP_INCLUDED
#define _NONE_OF_HPP_INCLUDED
// ========================================================================== //

// ============================== PREAMBLE ================================== //
// C++ standard library
// Project sources
// Third-party libraries
// Miscellaneous

namespace bit {

// Status: on hold (UnaryPredicate)
template <class InputIt, class UnaryPredicate>
constexpr bool none_of(bit_iterator<InputIt> first, bit_iterator<InputIt> last,
    UnaryPredicate p) {
    return std::none_of(first, last, p);
}

// Status: on hold (UnaryPredicate)
template <class ExecutionPolicy, class ForwardIt, class UnaryPredicate>
bool none_of(ExecutionPolicy&& policy, ForwardIt first, ForwardIt last,
    UnaryPredicate p) {
    return std::none_of(std::forward<ExecutionPolicy>(policy), first, last, p);
}

// ========================================================================== //
} // namespace bit

#endif // _NONE_OF_HPP_INCLUDED
// ========================================================================== //
