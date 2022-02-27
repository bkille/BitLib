// ============================= INNER PRODUCT ============================= //
// Project: The Experimental Bit Algorithms Library
// Name: inner_product.hpp
// Description: bit_iterator overloads for std::inner_product
// Creator: Vincent Reverdy
// Contributor(s): 
// License: BSD 3-Clause License
// ========================================================================== //
#ifndef _INNER_PRODUCT_HPP_INCLUDED
#define _INNER_PRODUCT_HPP_INCLUDED
// ========================================================================== //

// ============================== PREAMBLE ================================== //
// C++ standard library
// Project sources
// Third-party libraries
// Miscellaneous

namespace bit {
// ========================================================================== //



// Status: will not implement? 
template <class InputIt1, class InputIt2, class T>
T inner_product(bit_iterator<InputIt1> first1, bit_iterator<InputIt1> last1,
    bit_iterator<InputIt2> first2, T init) {
    (first1, last1, first2);
    return init;
}

// Status: on hold 
template <class InputIt1, class InputIt2, class T, class BinaryOperation1,
    class BinaryOperation2> T inner_product(bit_iterator<InputIt1> first1,
    bit_iterator<InputIt1> last1, bit_iterator<InputIt2> first2, T init, 
    BinaryOperation1 op1, BinaryOperation2 op2) {
    (first1, last1, first2, op1, op2);
    return init;
}

// ========================================================================== //
} // namespace bit

#endif // _INNER_PRODUCT_HPP_INCLUDED
// ========================================================================== //
