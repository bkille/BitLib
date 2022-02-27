// =============================== TRANSFORM =============================== //
// Project: The Experimental Bit Algorithms Library
// Name: transform.hpp
// Description: bit_iterator overloads for std::transform
// Creator: Vincent Reverdy
// Contributor(s): 
// License: BSD 3-Clause License
// ========================================================================== //
#ifndef _TRANSFORM_HPP_INCLUDED
#define _TRANSFORM_HPP_INCLUDED
// ========================================================================== //

// ============================== PREAMBLE ================================== //
// C++ standard library
// Project sources
// Third-party libraries
// Miscellaneous

namespace bit {
// ========================================================================== //



// Status: on hold
//template <class InputIt, class OutputIt, class UnaryOperation>
//constexpr bit_iterator<OutputIt> transform(bit_iterator<InputIt> first1,
    //bit_iterator<InputIt> last1, bit_iterator<OutputIt> d_first,
    //UnaryOperation unary_op) {
    //(first1, last1, unary_op);
    //return d_first;
//}

//// Status: on hold
//template <class ExecutionPolicy, class ForwardIt1, class ForwardIt2,
    //class UnaryOperation> bit_iterator<ForwardIt2> transform(ExecutionPolicy&& policy,
    //bit_iterator<ForwardIt1> first1, bit_iterator<ForwardIt1> last1,
    //bit_iterator<ForwardIt2> d_first, UnaryOperation unary_op) {
    //(policy, first1, last1, unary_op);
    //return d_first;
//}

//// Status: on hold
//template <class InputIt1, class InputIt2, class OutputIt, class BinaryOperation>
//constexpr bit_iterator<OutputIt> transform(bit_iterator<InputIt1> first1,
    //bit_iterator<InputIt1> last1, bit_iterator<InputIt2> first2,
    //bit_iterator<OutputIt> d_first, BinaryOperation binary_op) {
    //(first1, last1, first2, binary_op); 
    //return d_first;
//}

template <class InputIt1, class InputIt2, class OutputIt, class BinaryOperation>
constexpr bit_iterator<OutputIt> transform_word(bit_iterator<InputIt1> first1,
    bit_iterator<InputIt1> last1, bit_iterator<InputIt2> first2,
    bit_iterator<OutputIt> d_first, BinaryOperation binary_op) {
    // Assertions
     _assert_range_viability(first1, last1); 

    // Types and constants
    using word_type1    = typename bit_iterator<InputIt1>::word_type;
    using word_type2    = typename bit_iterator<InputIt2>::word_type;
    using word_type_out = typename bit_iterator<OutputIt>::word_type;
    using size_type1    = typename bit_iterator<InputIt1>::size_type;
    using size_type2    = typename bit_iterator<InputIt2>::size_type;
    using size_type_out = typename bit_iterator<OutputIt>::size_type;
    constexpr size_type1 digits1     = binary_digits<word_type1>::value;
    constexpr size_type2 digits2     = binary_digits<word_type2>::value;
    constexpr size_type_out digits_out  = binary_digits<word_type_out>::value;

    // This is just for now. Perhaps later we can expand to different word sizes
    assert(digits1 == digits2);
    assert(digits2 == digits_out);
    using word_type = word_type1; 
    using size_type = size_type1; 
    constexpr size_type digits = digits1; 

    if (is_within<digits>(first1, last1)) {
        word_type d = distance(first1, last1);
        write_word(
            binary_op(
                get_word(first1, d),
                get_word(first2, d)
            ),
            d_first, 
            d
        );
        return next(d_first, d);
    } 
    
    InputIt1 it1    = first1.base();
    if (first1.position() != 0) {
        word_type d = digits - first1.position();
        write_word(
            binary_op(
                static_cast<word_type>(*first1.base() >> first1.position()),
                get_word(first2, d)
            ),
            d_first,
            d
        );
        ++it1; 
        advance(first2, d);
        advance(d_first, d);
    }
    
    while (it1 != last1.base()) {
        write_word(
            binary_op(
                *it1++,
                get_word<word_type>(first2)
            ),
            d_first,
            (word_type)digits
        );
        advance(d_first, digits);
        advance(first2, digits);
    }

    if (last1.position() != 0) {
        write_word(
            binary_op(
                *it1,
                get_word(first2, last1.position())
            ),
            d_first,
            static_cast<word_type>(last1.position())
        );
        advance(d_first, last1.position());
    }
    return d_first;
}

// Status: on hold
//template <class ExecutionPolicy, class ForwardIt1, class ForwardIt2, class ForwardIt3,
    //class BinaryOperation> bit_iterator<ForwardIt3> transform(ExecutionPolicy&& policy,
    //bit_iterator<ForwardIt1> first1, bit_iterator<ForwardIt1> last1,
    //bit_iterator<ForwardIt2> first2, bit_iterator<ForwardIt3> d_first, 
    //BinaryOperation binary_op) {
    //(policy, first1, last1, first2, binary_op);
    //return d_first;
//}

// ========================================================================== //
} // namespace bit

#endif // _TRANSFORM_HPP_INCLUDED
// ========================================================================== //
