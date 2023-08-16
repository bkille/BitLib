// ================================== FIND ================================== //
// Project: The Experimental Bit Algorithms Library
// Description: bit_iterator overloads for std::find, std::find_if, std::find_if_not
// License: BSD 3-Clause License
// ========================================================================== //
#ifndef _FIND_HPP_INCLUDED
#define _FIND_HPP_INCLUDED
// ========================================================================== //

// ============================== PREAMBLE ================================== //
// C++ standard library
#include <iterator>
// Project sources
#include "bitlib/bit-iterator/bit.hpp"
// Third-party libraries
#ifdef BITLIB_HWY
#include "hwy/highway.h"
#endif

// Miscellaneous
#define is_aligned(POINTER, BYTE_COUNT) \
    (((uintptr_t)(const void *)(POINTER)) % (BYTE_COUNT) == 0)

namespace bit {

#ifdef BITLIB_HWY
namespace hn = hwy::HWY_NAMESPACE;
#endif
// ========================================================================== //


template <class RandomAccessIt>
constexpr bit_iterator<RandomAccessIt> find(
        bit_iterator<RandomAccessIt> first,
        bit_iterator<RandomAccessIt> last, bit::bit_value bv
) {

    using word_type = typename bit_iterator<RandomAccessIt>::word_type;
    using size_type = typename bit_iterator<RandomAccessIt>::size_type;
    const std::size_t digits = binary_digits<word_type>::value;

    // Initialization
    const bool is_first_aligned = first.position() == 0;
    const bool is_last_aligned = last.position() == 0;


    if (!is_first_aligned) {
        word_type shifted_first = *first.base() >> first.position();
        size_type num_trailing_complementary_bits = (bv == bit0) 
            ? _tzcnt(static_cast<word_type>(~shifted_first))
            : _tzcnt(static_cast<word_type>(shifted_first));
        if (std::next(first.base(), is_last_aligned) == last.base()) {
            return first + std::min(num_trailing_complementary_bits, (size_type) distance(first, last));
        } else if (num_trailing_complementary_bits + first.position() < digits) {
            return first + num_trailing_complementary_bits;
        } else {
            first += digits - first.position();
        }
    }

    // Initialization
    auto it = first.base();

// Waiting on github issue
#ifdef BITLIB_HWY
    // Align the iterator
    while (it != last.base() && !is_aligned(&(*it), 64)) {
        if ((bv == bit1 && (*it == 0)) || (bv == bit0 && (*it == static_cast<word_type>(-1)))) {
            ++it;
            continue;
        }

        size_type num_trailing_complementary_bits = (bv == bit0) 
            ? _tzcnt(static_cast<word_type>(~*it))
            : _tzcnt(static_cast<word_type>(*it));
        return bit_iterator(it, (size_type) num_trailing_complementary_bits);
    }

    // SIMD
    hn::ScalableTag<word_type> d;
    const auto z = hn::Zero(d);
    for (; std::distance(it, last.base()) >= hn::Lanes(d); it += hn::Lanes(d))
    {
        auto v = hn::Load(d, &*it);
        if (bv == bit0)
        {
            v = hn::Not(v);
        }
        const auto found = hn::Gt(v, z);
        if (! hn::AllFalse(d, found))
        {
            it += hn::FindKnownFirstTrue(d, found);
            size_type num_trailing_complementary_bits = (bv == bit0) 
                ? _tzcnt(static_cast<word_type>(~*it))
                : _tzcnt(static_cast<word_type>(*it));
            return bit_iterator(it, (size_type) num_trailing_complementary_bits);
        }
    }
#endif

    // Finish out the remainder with typical for loop
    while (it != last.base()) {
        if ((bv == bit1 && (*it == 0)) || (bv == bit0 && (*it == static_cast<word_type>(-1)))) {
            ++it;
            continue;
        }

        size_type num_trailing_complementary_bits = (bv == bit0) 
            ? _tzcnt(static_cast<word_type>(~*it))
            : _tzcnt(static_cast<word_type>(*it));
        return bit_iterator(it, (size_type) num_trailing_complementary_bits);
    } 

    // Deal with any unaligned boundaries
    if (!is_last_aligned) {
        size_type num_trailing_complementary_bits = (bv == bit0) 
            ? _tzcnt(static_cast<word_type>(~*it))
            : _tzcnt(static_cast<word_type>(*it));
        return bit_iterator(it, (size_type) std::min(num_trailing_complementary_bits, last.position()));
    }
    return last;
}

// ========================================================================== //
} // namespace bit

#endif // _FIND_HPP_INCLUDED
// ========================================================================== //
