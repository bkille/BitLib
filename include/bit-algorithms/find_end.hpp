// ================================ FIND END ================================ //
// Project: The Experimental Bit Algorithms Library
// Name: find_end.hpp
// Description: bit_iterator overloads for std::find_end
// Creator: Vincent Reverdy
// Contributor(s): 
// License: BSD 3-Clause License
// ========================================================================== //
#ifndef _FIND_END_HPP_INCLUDED
#define _FIND_END_HPP_INCLUDED
// ========================================================================== //

// ============================== PREAMBLE ================================== //
// C++ standard library
// Project sources
// Third-party libraries
// Miscellaneous

namespace bit {
// ========================================================================== //



// Status: to do
template <class ForwardIt1, class ForwardIt2>
constexpr bit_iterator<ForwardIt1> find_end(bit_iterator<ForwardIt1> first,
    bit_iterator<ForwardIt1> last, bit_iterator<ForwardIt2> s_first,
    bit_iterator<ForwardIt2> s_last) {
    (last, s_first, s_last); 
    return first;
}

// Status: to do
template <class ExecutionPolicy, class ForwardIt1, class ForwardIt2>
bit_iterator<ForwardIt1> find_end(ExecutionPolicy&& policy,
    bit_iterator<ForwardIt1> first, bit_iterator<ForwardIt1> last,
    bit_iterator<ForwardIt2> s_first, bit_iterator<ForwardIt2> s_last) {
    (policy, last, s_first, s_last); 
    return first;
}

// Status: on hold
template <class ForwardIt1, class ForwardIt2,  class BinaryPredicate>
constexpr bit_iterator<ForwardIt1> find_end(bit_iterator<ForwardIt1> first,
    bit_iterator<ForwardIt1> last, bit_iterator<ForwardIt2> s_first,
    bit_iterator<ForwardIt2> s_last, BinaryPredicate p) {
    (last, s_first, s_last, p); 
    return first;
}

// Status: on hold
template <class ExecutionPolicy, class ForwardIt1, class ForwardIt2,
    class BinaryPredicate> bit_iterator<ForwardIt1> find_end(
    ExecutionPolicy&& policy, bit_iterator<ForwardIt1> first,
    bit_iterator<ForwardIt1> last, bit_iterator<ForwardIt2> s_first,
    bit_iterator<ForwardIt2> s_last, BinaryPredicate p) {
    (policy, last, s_first, s_last, p); 
    return first;
} 

// ========================================================================== //
} // namespace bit

#endif // _FIND_END_HPP_INCLUDED
// ========================================================================== //
