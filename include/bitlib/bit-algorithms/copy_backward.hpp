// ============================= COPY BACKWARD ============================= //
// Project: The Experimental Bit Algorithms Library
// Name: copy_backward.hpp
// Description: bit_iterator overloads for std::copy_backward
// Creator: Vincent Reverdy
// Contributor(s): 
// License: BSD 3-Clause License
// ========================================================================== //
#ifndef _COPY_BACKWARD_HPP_INCLUDED
#define _COPY_BACKWARD_HPP_INCLUDED
// ========================================================================== //



// ================================ PREAMBLE ================================ //
// C++ standard library
#include <type_traits>
#include <math.h>
// Project sources
#include "bitlib/bitlib.hpp"
// Third-party libraries
// Miscellaneous
namespace bit {
// ========================================================================== //


// Status: Does not work for Input/Output iterators due to distance call
template <class RandomAccessIt1, class RandomAccessIt2>
constexpr bit_iterator<RandomAccessIt2> copy_backward(bit_iterator<RandomAccessIt1> first,
                            bit_iterator<RandomAccessIt1> last,
                            bit_iterator<RandomAccessIt2> d_last
)
{
    // Types and constants
    using dst_word_type = typename bit_iterator<RandomAccessIt2>::word_type;
    using src_word_type = typename bit_iterator<RandomAccessIt1>::word_type;
    using word_type = dst_word_type;
    using size_type = typename bit_iterator<RandomAccessIt2>::size_type;
    constexpr size_type digits = binary_digits<word_type>::value;

    // Assertions
    _assert_range_viability(first, last);
    static_assert(::std::is_same<dst_word_type, src_word_type>::value, "Underlying word types must be equal");
    if (first == last) return d_last;


    // Initialization
    const bool is_d_last_aligned = d_last.position() == 0;
    size_type total_bits_to_copy = distance(first, last);
    size_type remaining_bits_to_copy = total_bits_to_copy;
    auto it = d_last.base();

    // d_last is not aligned. Copy partial word to align it
    if (!is_d_last_aligned) {
        size_type partial_bits_to_copy = ::std::min(
                remaining_bits_to_copy,
                d_last.position()
        );
        *it = _bitblend<word_type>(
                *it,
                static_cast<word_type>(
                  get_word<word_type>(last - partial_bits_to_copy, partial_bits_to_copy) 
                ) << d_last.position() - partial_bits_to_copy,
                d_last.position() - partial_bits_to_copy,
                static_cast<word_type>(partial_bits_to_copy)
        );
        remaining_bits_to_copy -= partial_bits_to_copy;
        advance(last, -partial_bits_to_copy);
    }

    if (remaining_bits_to_copy > 0) { 
        const bool is_last_aligned = last.position() == 0;
        //size_type words_to_copy = ::std::ceil(remaining_bits_to_copy / static_cast<float>(digits));
        // d_last will be aligned at this point
        if (is_last_aligned && remaining_bits_to_copy > digits) {
            auto N = ::std::distance(first.base(), last.base()) - 1;
            it = ::std::copy_backward(first.base() + 1, last.base(), it);
            last -= digits * N;
            remaining_bits_to_copy -= digits * N;
            it--;
        } else {
            // TODO benchmark if its faster to ::std::copy the entire range then shift
            while (remaining_bits_to_copy >= digits) {
                *(--it) = get_word<word_type>(last - digits, digits);
                remaining_bits_to_copy -= digits;
                advance(last, -digits);
            }
            it--;
        }
        if (remaining_bits_to_copy > 0) {
            *it = _bitblend<word_type>(
                    *it,
                    get_word<word_type>(last - remaining_bits_to_copy, remaining_bits_to_copy)
                       << (digits - remaining_bits_to_copy),
                    digits - remaining_bits_to_copy,
                    remaining_bits_to_copy
            );
            remaining_bits_to_copy = 0;
        }
    }
    return d_last - total_bits_to_copy;
}



// ========================================================================== //
} // namespace bit

#endif // _COPY_BACKWARD_HPP_INCLUDED
// ========================================================================== //
