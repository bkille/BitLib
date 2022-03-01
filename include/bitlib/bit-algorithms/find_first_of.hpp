// ============================= FIND FIRST OF ============================= //
// Project: The Experimental Bit Algorithms Library
// Name: find_first_of.hpp
// Description: bit_iterator overloads for std::find_first_of
// Creator: Vincent Reverdy
// Contributor(s): 
// License: BSD 3-Clause License
// ========================================================================== //
#ifndef _FIND_FIRST_OF_HPP_INCLUDED
#define _FIND_FIRST_OF_HPP_INCLUDED
// ========================================================================== //

// ============================== PREAMBLE ================================== //
// C++ standard library
// Project sources
// Third-party libraries
// Miscellaneous

namespace bit {

// Status: to do
template <class InputIt, class ForwardIt>
constexpr bit_iterator<InputIt> find_first_of(bit_iterator<InputIt> first,
    bit_iterator<InputIt> last, bit_iterator<ForwardIt> s_first,
    bit_iterator<ForwardIt> s_last) {
    (last, s_first, s_last); 
    return first;
}

// Status: to do
template <class ExecutionPolicy, class ForwardIt1, class ForwardIt2>
bit_iterator<ForwardIt1> find_first_of(ExecutionPolicy&& policy,
    bit_iterator<ForwardIt1> first, bit_iterator<ForwardIt1> last,
    bit_iterator<ForwardIt2> s_first, bit_iterator<ForwardIt2> s_last) {
    (policy, last, s_first, s_last); 
    return first;
}

// Status: on hold
template <class InputIt, class ForwardIt, class BinaryPredicate>
constexpr bit_iterator<InputIt> find_first_of(bit_iterator<InputIt> first,
    bit_iterator<InputIt> last, bit_iterator<ForwardIt> s_first,
    bit_iterator<ForwardIt> s_last, BinaryPredicate p) {
    (last, s_first, s_last, p);
    return first;
}

// Status: on hold
template <class ExecutionPolicy, class ForwardIt1, class ForwardIt2,
    class BinaryPredicate> bit_iterator<ForwardIt1> find_first_of(
    ExecutionPolicy&& policy, bit_iterator<ForwardIt1> first,
    bit_iterator<ForwardIt1> last, bit_iterator<ForwardIt2> s_first,
    bit_iterator<ForwardIt2> s_last, BinaryPredicate p) {
    (policy, last, s_first, s_last, p); 
    return first;
}

// ========================================================================== //
} // namespace bit

#endif // _FIND_FIRST_OF_HPP_INCLUDED
// ========================================================================== //
