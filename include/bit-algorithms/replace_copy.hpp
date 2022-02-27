// ============================== REPLACE COPY ============================== //
// Project: The Experimental Bit Algorithms Library
// Name: replace_copy.hpp
// Description: bit_iterator overloads for std::replace_copy, std::replace_copy_if
// Creator: Vincent Reverdy
// Contributor(s): 
// License: BSD 3-Clause License
// ========================================================================== //
#ifndef _REPLACE_COPY_HPP_INCLUDED
#define _REPLACE_COPY_HPP_INCLUDED
// ========================================================================== //

// ============================== PREAMBLE ================================== //
// C++ standard library
// Project sources
// Third-party libraries
// Miscellaneous
namespace bit {
// ========================================================================== //


// Status: to do
// Notes: signature is strange (old_value, new_value)
template <class InputIt, class OutputIt>
constexpr OutputIt replace_copy(bit_iterator<InputIt> first, 
    bit_iterator<InputIt> last, bit_iterator<OutputIt> d_first,
    bit_value old_value, bit_value new_value) {
    (first, last, old_value, new_value);
    return d_first;
}

// Status: to do
// Notes: signature is strange (old_value, new_value)
template <class ExecutionPolicy, class ForwardIt1, class ForwardIt2>
bit_iterator<ForwardIt2> replace_copy(ExecutionPolicy&& policy,
    bit_iterator<ForwardIt1> first, bit_iterator<ForwardIt1> last,
    bit_iterator<ForwardIt2> d_first, bit_value old_value, bit_value new_value) {
    (policy, first, last, old_value, new_value); 
    return d_first;
}

// Status: to do 
template <class InputIt, class OutputIt, class UnaryPredicate>
constexpr bit_iterator<OutputIt> replace_copy_if(bit_iterator<InputIt> first,
    bit_iterator<InputIt> last, bit_iterator<OutputIt> d_first,
    UnaryPredicate p, bit_value new_value) {
    (first, last, p, new_value);
    return d_first;
}

// Status: to do 
template <class ExecutionPolicy, class ForwardIt1, class ForwardIt2,
    class UnaryPredicate> bit_iterator<ForwardIt2> replace_copy_if(
    ExecutionPolicy&& policy, bit_iterator<ForwardIt1> first,
    bit_iterator<ForwardIt1> last, bit_iterator<ForwardIt2> d_first,
    UnaryPredicate p, bit_value new_value) {
    (policy, first, last, p, new_value);
    return d_first;
}

// ========================================================================== //
} // namespace bit

#endif // _REPLACE_COPY_HPP_INCLUDED
// ========================================================================== //
