// ============================ TRANSFORM REDUCE ============================ //
// Project: The Experimental Bit Algorithms Library
// Name: transform_reduce.hpp
// Description: bit_iterator overloads for std::transform_reduce
// Creator: Vincent Reverdy
// Contributor(s): 
// License: BSD 3-Clause License
// ========================================================================== //
#ifndef _TRANSFORM_REDUCE_HPP_INCLUDED
#define _TRANSFORM_REDUCE_HPP_INCLUDED
// ========================================================================== //

// ============================== PREAMBLE ================================== //
// C++ standard library
// Project sources
// Third-party libraries
// Miscellaneous

namespace bit {
// ============================== PREAMBLE ================================== //



// Status: will not implement?
template <class InputIt1, class InputIt2, class T>
T transform_reduce(bit_iterator<InputIt1> first1, bit_iterator<InputIt1> last1,
    bit_iterator<InputIt2> first2, T init) {
    (first1, last1, first2);
    return init;
}

// Status: on hold
template <class InputIt1, class InputIt2, class T, class BinaryOp1, class BinaryOp2>
T transform_reduce(bit_iterator<InputIt1> first1, bit_iterator<InputIt1> last1,
    bit_iterator<InputIt2> first2, T init, BinaryOp1 binary_op1, BinaryOp2 binary_op2) {
    (first1, last1, first2, binary_op1, binary_op2);
    return init;    
}

// Status: on hold
template <class InputIt, class T, class BinaryOp, class UnaryOp>
T transform_reduce(bit_iterator<InputIt> first, bit_iterator<InputIt> last, T init,
    BinaryOp binop, UnaryOp unary_op) {
    (first, last, binop, unary_op);
    return init;
}

// Status: will not implement?
template <class ExecutionPolicy, class ForwardIt1, class ForwardIt2, class T>
T transform_reduce(ExecutionPolicy&& policy, bit_iterator<ForwardIt1> first1,
    bit_iterator<ForwardIt1> last1, bit_iterator<ForwardIt2> first2, T init) {
    (policy, first1, last1, first2);
    return init;
}

// Status: on hold
template <class ExecutionPolicy, class ForwardIt1, class ForwardIt2, class T,
    class BinaryOp1, class BinaryOp2> T transform_reduce(ExecutionPolicy&& policy,
    bit_iterator<ForwardIt1> first1, bit_iterator<ForwardIt1> last1,
    bit_iterator<ForwardIt2> first2, T init, BinaryOp1 binary_op1, BinaryOp2 binary_op2) {
    (policy, first1, last1, first2, binary_op1, binary_op2);
    return init;
}

// Status: on hold
template <class ExecutionPolicy, class ForwardIt, class T, class BinaryOp, class UnaryOp>
T transform_reduce(ExecutionPolicy&& policy, bit_iterator<ForwardIt> first,
    bit_iterator<ForwardIt> last, T init, BinaryOp binary_op, UnaryOp unary_op) {
    (policy, first, last, binary_op, unary_op);
    return init;
}

// ========================================================================== //
} // namespace bit

#endif // _TRANSFORM_REDUCE_HPP_INCLUDED
// ========================================================================== //
