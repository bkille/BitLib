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
#ifdef BITLIB_HWY
#include "hwy/highway.h"
#endif
// Miscellaneous
#define is_aligned(POINTER, BYTE_COUNT) \
        (((uintptr_t)(const void *)(POINTER)) % (BYTE_COUNT) == 0)

//HWY_BEFORE_NAMESPACE();
namespace bit {

#ifdef BITLIB_HWY
namespace hn = hwy::HWY_NAMESPACE;
#endif
// ========================================================================== //



// Status: needs revisions
template <class RandomAccessIt>
void fill(bit_iterator<RandomAccessIt> first, bit_iterator<RandomAccessIt> last, 
    bit::bit_value bv) {
    // Assertions
    _assert_range_viability(first, last);

    // Types and constants
    using word_type = typename bit_iterator<RandomAccessIt>::word_type;
    constexpr word_type digits = binary_digits<word_type>::value;

    // Initializations
    constexpr word_type ones = -1;
    const word_type fill_word = bv == bit0 ? 0 : ones;

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

#ifdef BITLIB_HWY
        // Align to 64 bit boundary
        for (; it != last.base() && !is_aligned(&*it, 64); it++) {
            *it = fill_word;
        }
        const hn::ScalableTag<word_type> d;
        const auto fill_vec = bv == bit0 ? hn::Set(d, 0) : hn::Set(d, ones);
        for (; std::distance(it, last.base()) >= hn::Lanes(d); it += hn::Lanes(d))
        {
            hn::Store(fill_vec, d, &*it);  
        }
#endif
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
