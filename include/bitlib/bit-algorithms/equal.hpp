// ================================= EQUAL =================================== //
// Project:     The Experimental Bit Algorithms Library
// Name:        equal.hpp
// Creator:     Vincent Reverdy
// Contributor: Bryce Kille [2019]
// License:     BSD 3-Clause License
// ========================================================================== //
#ifndef _EQUAL_HPP_INCLUDED
#define _EQUAL_HPP_INCLUDED
// ========================================================================== //



// ================================ PREAMBLE ================================ //
// C++ standard library
#include <type_traits>
#include <math.h>
// Project sources
#include "bitlib/bitlib.hpp"
// Third-party libraries
#include "fmt/core.h"
// Miscellaneous
namespace bit {
// ========================================================================== //



// ---------------------------- Equal Algorithms ----------------------------- //

// Status: Does not work for Input/Output iterators due to distance call
template <class RandomAccessIt1, class RandomAccessIt2>
constexpr bool equal(
        bit_iterator<RandomAccessIt1> first,
        bit_iterator<RandomAccessIt1> last,
        bit_iterator<RandomAccessIt2> d_first
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
    if (first == last) return true;

    // Initialization
    const bool is_d_first_aligned = d_first.position() == 0;
    size_type total_bits_to_check = distance(first, last);
    size_type remaining_bits_to_check = total_bits_to_check;
    auto it = d_first.base();

    // d_first is not aligned. 
    if (!is_d_first_aligned) {
        const size_type partial_bits_to_check = ::std::min(
                remaining_bits_to_check,
                digits - d_first.position());
        const word_type mask = static_cast<word_type>(
                (static_cast<word_type>(1) << partial_bits_to_check) - 1
        ) << d_first.position();
        const word_type comp = static_cast<word_type>(
              get_word<word_type>(first, partial_bits_to_check) 
                << d_first.position());
        if ((mask & *it) != (mask & comp)) { return false; }
        remaining_bits_to_check -= partial_bits_to_check;
        ::std::advance(first, partial_bits_to_check);
        it++;
    }

    if (remaining_bits_to_check > 0) { 
        const bool is_first_aligned = first.position() == 0;
        // d_first will be aligned at this point
        if (is_first_aligned && remaining_bits_to_check >= digits) {
            auto N = ::std::distance(first.base(), last.base());
            bool found_mismatch = !::std::equal(first.base(), last.base(), it);
            if (found_mismatch) {return false;}
            it += N;
            first += digits * N;
            remaining_bits_to_check -= digits * N;
        } else {
            // TODO benchmark if its faster to ::std::check the entire range then shift
            while (remaining_bits_to_check >= digits) {
                if (*it != get_word<word_type>(first, digits)) {return false;}
                remaining_bits_to_check -= digits;
                it++; 
                advance(first, digits);
            }
        }
        if (remaining_bits_to_check > 0) {
            const word_type mask = static_cast<word_type>(
                    (static_cast<word_type>(1) << remaining_bits_to_check) - 1
            );
            const word_type comp = get_word<word_type>(first, remaining_bits_to_check);
            if ((mask & *it) != (mask & comp)) { return false; }
        }
    }
    return true;
}
// -------------------------------------------------------------------------- //



// ========================================================================== //
} // namespace bit
#endif // _EQUAL_HPP_INCLUDED
// ========================================================================== //
