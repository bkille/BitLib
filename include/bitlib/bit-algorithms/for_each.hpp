// ================================ FOR EACH ================================ //
// Project: The Experimental Bit Algorithms Library
// Name: for_each.hpp
// Description: bit_iterator overloads for std::for_each
// Creator: Vincent Reverdy
// Contributor(s): 
// License: BSD 3-Clause License
// ========================================================================== //
#ifndef _FOR_EACH_HPP_INCLUDED
#define _FOR_EACH_HPP_INCLUDED
// ========================================================================== //

// ============================== PREAMBLE ================================== //
// C++ standard library
// Project sources
// Third-party libraries
// Miscellaneous

namespace bit {

// Status: complete
template <class InputIt, class UnaryFunction>
constexpr UnaryFunction for_each(bit_iterator<InputIt> first, 
    bit_iterator<InputIt> last, UnaryFunction f) {
    return std::for_each(first, last, f);
}

// Status: complete
template <class ExecutionPolicy, class ForwardIt, class UnaryFunction2>
void for_each(ExecutionPolicy&& policy, bit_iterator<ForwardIt> first,
    bit_iterator<ForwardIt> last, UnaryFunction2 f) {
    return std::for_each(std::forward<ExecutionPolicy>(policy), first, last, f);
}

// ========================================================================== //
} // namespace bit

#endif // _FOR_EACH_HPP_INCLUDED
// ========================================================================== //
