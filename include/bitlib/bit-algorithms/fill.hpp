// ================================= FILL =================================== //
// Project:         The Experimental Bit Algorithms Library
// Name:            fill.hpp
// Description:     bit_iterator overloads for std::fill 
// Creator:         Vincent Reverdy
// Contributor(s):  Vincent Reverdy [2019]
//                  Bryce Kille [2019]
// License:         BSD 3-Clause License
// ========================================================================== //
#ifndef _FILL_HPP_INCLUDED
#define _FILL_HPP_INCLUDED
// ========================================================================== //



// ============================== PREAMBLE ================================== //
// C++ standard library
// Project sources
#include "bit_algorithm_details.hpp"
// Third-party libraries
#include <simdpp/simd.h>
// Miscellaneous
#define is_aligned(POINTER, BYTE_COUNT) \
        (((uintptr_t)(const void *)(POINTER)) % (BYTE_COUNT) == 0)
namespace bit {
// ========================================================================== //



// Status: needs revisions
template <class ForwardIt>
void fill(bit_iterator<ForwardIt> first, bit_iterator<ForwardIt> last, 
    bit::bit_value bv) {
    // Assertions
    _assert_range_viability(first, last);

    // Types and constants
    using word_type = typename bit_iterator<ForwardIt>::word_type;
    constexpr word_type digits = binary_digits<word_type>::value;

    // Initializations
    const word_type fill_word = bv == bit0 ? 0 : -1;
    const uint64_t fill_word_64 = bv == bit0 ? 0 : -1;

    if (distance(first, last) == 0) {
        return;
    }
    if (is_within<digits>(first, last)) {
        write_word<word_type>(fill_word, first, distance(first, last));
    } else {
        auto it = first.base();
        if (first.position() != 0) {
            write_word<word_type>(fill_word, first, digits - first.position());
            ++it;
        }
        const unsigned long N = SIMDPP_FAST_INT64_SIZE;
        const unsigned long N_native_words = (N*64)/digits;
        for (; it != last.base() && !is_aligned(&*it, 64); it++) {
            *it = fill_word;
        }
        for (; std::distance(it, last.base()) >= N_native_words + 2; it += N_native_words) {
            using vec_type = simdpp::uint64<N>;
            vec_type v = simdpp::load_splat(&fill_word_64);
            simdpp::store(&(*it), v);
        }
        std::fill(it, last.base(), fill_word);
        if (last.position() != 0) {
            it = last.base();
            write_word<word_type>(fill_word, bit_iterator<word_type*>(&(*it)), last.position());
        }
    }
}

// ========================================================================== //
} // namespace bit
#endif // _FILL_HPP_INCLUDED
// ========================================================================== //
