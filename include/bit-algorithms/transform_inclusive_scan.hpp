// ======================== TRANSFORM INCLUSIVE SCAN ======================== //
// Project: The Experimental Bit Algorithms Library
// Name: transform_inclusive_scan.hpp
// Description: bit_iterator overloads for std::transform_inclusive_scan
// Creator: Vincent Reverdy
// Contributor(s): 
// License: BSD 3-Clause License
// ========================================================================== //
#ifndef _TRANSFORM_INCLUSIVE_SCAN_HPP_INCLUDED
#define _TRANSFORM_INCLUSIVE_SCAN_HPP_INCLUDED
// ========================================================================== //

// ============================== PREAMBLE ================================== //
// C++ standard library
// Project sources
// Third-party libraries
// Miscellaneous

namespace bit {
// ========================================================================== //


// Status: on hold
template <class InputIt, class OutputIt, class BinaryOperation, class UnaryOperation>
bit_iterator<OutputIt> transform_inclusive_scan(bit_iterator<InputIt> first,
    bit_iterator<InputIt> last, bit_iterator<OutputIt> d_first,
    BinaryOperation binary_op, UnaryOperation unary_op) {
    (first, last, binary_op, unary_op);
    return d_first;
}

// Status: on hold
template <class ExecutionPolicy, class ForwardIt1, class ForwardIt2,
    class BinaryOperation, class UnaryOperation> bit_iterator<ForwardIt2>
    transform_inclusive_scan(ExecutionPolicy&& policy, bit_iterator<ForwardIt1> first,
    bit_iterator<ForwardIt1> last, bit_iterator<ForwardIt2> d_first,
    BinaryOperation binary_op, UnaryOperation unary_op) {
    (policy, first, last, binary_op, unary_op);
    return d_first;
}

// Status: on hold
template <class InputIt, class OutputIt, class BinaryOperation, class UnaryOperation,
    class T> bit_iterator<OutputIt> transform_inclusive_scan(bit_iterator<InputIt> first,
    bit_iterator<InputIt> last, bit_iterator<OutputIt> d_first, BinaryOperation binary_op,
    UnaryOperation unary_op, T init) {
    (first, last, binary_op, unary_op, init); 
    return d_first;
}

// Status: on hold
template <class ExecutionPolicy, class ForwardIt1, class ForwardIt2, class BinaryOperation,
    class UnaryOperation, class T> bit_iterator<ForwardIt2> transform_inclusive_scan(
    ExecutionPolicy&& policy, bit_iterator<ForwardIt1> first, bit_iterator<ForwardIt1> last,
    bit_iterator<ForwardIt2> d_first, BinaryOperation binary_op, UnaryOperation unary_op,
    T init) {
    (policy, first, last, binary_op, unary_op, init);
    return d_first;
}

// ========================================================================== //
} // namespace bit

#endif // _TRANSFORM_INCLUSIVE_SCAN_HPP_INCLUDED
// ========================================================================== //
