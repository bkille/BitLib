// ======================== TRANSFORM EXCLUSIVE SCAN ======================== //
// Project: The Experimental Bit Algorithms Library
// Name: transform_exclusive_scan.hpp
// Description: bit_iterator overloads for std::transform_exclusive_scan
// Creator: Vincent Reverdy
// Contributor(s): 
// License: BSD 3-Clause License
// ========================================================================== //
#ifndef _TRANSFORM_EXCLUSIVE_SCAN_HPP_INCLUDED
#define _TRANSFORM_EXCLUSIVE_SCAN_HPP_INCLUDED
// ========================================================================== //

// ============================== PREAMBLE ================================== //
// C++ standard library
// Project sources
// Third-party libraries
// Miscellaneous

namespace bit {
// ========================================================================== //



// Status: on hold
template <class InputIt, class OutputIt, class T, class BinaryOperation,
    class UnaryOperation> bit_iterator<OutputIt> transform_exclusive_scan(
    bit_iterator<InputIt> first, bit_iterator<InputIt> last, 
    bit_iterator<OutputIt> d_first, T init, BinaryOperation binary_op,
    UnaryOperation unary_op) {
    (first, last, init, binary_op, unary_op);
    return d_first;
}

// Status: on hold
template <class ExecutionPolicy, class ForwardIt1, class ForwardIt2, class T,
    class BinaryOperation, class UnaryOperation> bit_iterator<ForwardIt2>
    transform_exclusive_scan(ExecutionPolicy&& policy, bit_iterator<ForwardIt1> first,
    bit_iterator<ForwardIt1> last, bit_iterator<ForwardIt2> d_first, T init,
    BinaryOperation binary_op, UnaryOperation unary_op) {
    (policy, first, last, init, binary_op, unary_op); 
    return d_first; 
}
  
// ========================================================================== //
} // namespace bit

#endif // _TRANSFORM_EXCLUSIVE_SCAN_HPP_INCLUDED
// ========================================================================== //
