// ================================= REDUCE ================================= //
// Project: The Experimental Bit Algorithms Library
// Name: reduce.hpp
// Description: bit_iterator overloads for std::reduce
// Creator: Vincent Reverdy
// Contributor(s): 
// License: BSD 3-Clause License
// ========================================================================== //
#ifndef _REDUCE_HPP_INCLUDED
#define _REDUCE_HPP_INCLUDED
// ========================================================================== //

// ============================== PREAMBLE ================================== //
// C++ standard library
// Project sources
// Third-party libraries
// Miscellaneous

namespace bit {
// ========================================================================== //



// Status: will not implement?
template <class InputIt>
typename bit_iterator<InputIt>::value_type reduce(bit_iterator<InputIt> first,
    bit_iterator<InputIt> last) {
    (first, last);
    return 0;
}

// Status: will not implement?
template <class ExecutionPolicy, class ForwardIt>
typename bit_iterator<ForwardIt>::value_type reduce(ExecutionPolicy&& policy,
    bit_iterator<ForwardIt> first, bit_iterator<ForwardIt> last) {
    (policy, first, last);
    return 0;
}

// Status: will not implement?
template <class InputIt, class T>
T reduce(bit_iterator<InputIt> first, bit_iterator<InputIt> last, T init) {
    (first, last, init);
    return init;
}

// Status: will not implement?
template <class ExecutionPolicy, class ForwardIt, class T>
T reduce(ExecutionPolicy&& policy, bit_iterator<ForwardIt> first,
    bit_iterator<ForwardIt> last, T init) {
    (policy, first, last);
    return init;
}

// Status: on hold
template <class InputIt, class T, class BinaryOp>
T reduce(bit_iterator<InputIt> first, bit_iterator<InputIt> last, T init,
    BinaryOp binary_op) {
    (first, last, binary_op);
    return init;
}

// Status: on hold
template <class ExecutionPolicy, class ForwardIt, class T, class BinaryOp>
T reduce(ExecutionPolicy&& policy, bit_iterator<ForwardIt> first,
    bit_iterator<ForwardIt> last, T init, BinaryOp binary_op) {
    (policy, first, last, binary_op);
    return init;
}

// ========================================================================== //
} // namespace bit

#endif // _REDUCE_HPP_INCLUDED
// ========================================================================== //
