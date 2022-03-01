// ================================= EQUAL ================================= //
// Project: The Experimental Bit Algorithms Library
// Name: equal.hpp
// Description: bit_iterator overloads for std::equal
// Creator: Vincent Reverdy
// Contributor(s): 
// License: BSD 3-Clause License
// ========================================================================== //
#ifndef _EQUAL_HPP_INCLUDED
#define _EQUAL_HPP_INCLUDED
// ========================================================================== //

// ============================== PREAMBLE ================================== //
// C++ standard library
// Project sources
// Third-party libraries
// Miscellaneous

namespace bit {
// ========================================================================== //



// Status: to do
template <class InputIt1, class InputIt2>
constexpr bool equal(bit_iterator<InputIt1> first1, bit_iterator<InputIt1> last, 
    bit_iterator<InputIt2> first2) {
    (first1, last, first2); 
    return true;
}

// Status: to do
template <class ExecutionPolicy, class ForwardIt1, class ForwardIt2>
bool equal(ExecutionPolicy&& policy, bit_iterator<ForwardIt1> first1,
    bit_iterator<ForwardIt1> last1, bit_iterator<ForwardIt2> first2) {
    (policy, first1, last1, first2);
    return true;
}

// Status: on hold
template <class InputIt1, class InputIt2, class BinaryPredicate>
constexpr bool equal(bit_iterator<InputIt1> first1, bit_iterator<InputIt1> last1,
    bit_iterator<InputIt2> first2, BinaryPredicate p) {
    (first1, last1, first2, p);
    return true;
}

// Status: on hold
template <class ExecutionPolicy, class ForwardIt1, class ForwardIt2,
    class BinaryPredicate> bool equal(ExecutionPolicy&& policy,
    bit_iterator<ForwardIt1> first1, bit_iterator<ForwardIt1> last1,
    BinaryPredicate p) {
    (policy, first1, last1, p);
    return true;
}

// Status: on hold
template <class InputIt1, class InputIt2>
constexpr bool equal(bit_iterator<InputIt1> first1, bit_iterator<InputIt1> last1,
    bit_iterator<InputIt2> first2, bit_iterator<InputIt2> last2) {
    (first1, last1, first2, last2);
    return true;
}

// Status: to do
template <class ExecutionPolicy, class ForwardIt1, class ForwardIt2>
bool equal(ExecutionPolicy&& policy, bit_iterator<ForwardIt1> first1,
    bit_iterator<ForwardIt1> last1, bit_iterator<ForwardIt2> first2,
    bit_iterator<ForwardIt2> last2) {
    (policy, first1, last1, first2, last2); 
    return true;
} 

// Status: on hold
template <class ExecutionPolicy, class ForwardIt1, class ForwardIt2,
    class BinaryPredicate> constexpr bool equal(bit_iterator<ForwardIt1> first1,
    bit_iterator<ForwardIt1> last1, bit_iterator<ForwardIt2> first2,
    bit_iterator<ForwardIt2> last2, BinaryPredicate p) {
    (first1, last1, first2, last2, p);
    return true;
} 

// Status: on hold
template <class ExecutionPolicy, class ForwardIt1, class ForwardIt2,
    class BinaryPredicate> bool equal(ExecutionPolicy&& policy,
    bit_iterator<ForwardIt1> first1, bit_iterator<ForwardIt1> last1,
    bit_iterator<ForwardIt2> first2, bit_iterator<ForwardIt2> last2,
    BinaryPredicate p) {
    (policy, first1, last1, first2, last2, p);
    return true;
}

// ========================================================================== //
} // namespace bit

#endif // _EQUAL_HPP_INCLUDED
// ========================================================================== //
