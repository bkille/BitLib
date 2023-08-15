// ================================ SHIFT ================================== //
// Project:         The Experimental Bit Algorithms Library
// Name:            shift.hpp
// Description:     Implementation of shift_left and shift_right 
// Creator:         Vincent Reverdy
// Contributor(s):  Bryce Kille [2019]
// License:         BSD 3-Clause License
// ========================================================================== //
#ifndef _SHIFT_HPP_INCLUDED
#define _SHIFT_HPP_INCLUDED
// ========================================================================== //


// ================================ PREAMBLE ================================ //
// C++ standard library
#include <algorithm>
#include <iterator>
// Project sources
// Third-party libraries
#ifdef BITLIB_HWY
#include "hwy/highway.h"
#endif
// Miscellaneous
#define is_aligned(POINTER, BYTE_COUNT) \
    (((uintptr_t)(const void *)(POINTER)) % (BYTE_COUNT) == 0)

#if __cplusplus >= 202002L
#define STD_SHIFT_RIGHT(FIRST, LAST, N) std::shift_right(FIRST, LAST, N) 
#define STD_SHIFT_LEFT(FIRST, LAST, N) std::shift_left(FIRST, LAST, N) 
#else
#define STD_SHIFT_RIGHT(FIRST, LAST, N) word_shift_right(FIRST, LAST, N) 
#define STD_SHIFT_LEFT(FIRST, LAST, N) word_shift_left(FIRST, LAST, N) 
#endif 

namespace bit {

#ifdef BITLIB_HWY
namespace hn = hwy::HWY_NAMESPACE;
#endif
// ========================================================================== //



// --------------------------- Shift Algorithms ----------------------------- //

template <class RandomAccessIt>
bit_iterator<RandomAccessIt> shift_left(
        bit_iterator<RandomAccessIt> first,
        bit_iterator<RandomAccessIt> last,
        typename bit_iterator<RandomAccessIt>::difference_type n
) {
    // Assertions
     _assert_range_viability(first, last); 

    // Types and constants
    using word_type = typename bit_iterator<RandomAccessIt>::word_type;
    using size_type = typename bit_iterator<RandomAccessIt>::size_type;
    constexpr size_type digits = binary_digits<word_type>::value;

    // Initialization
    auto d = distance(first, last);
    const bool is_first_aligned = first.position() == 0;
    const bool is_last_aligned = last.position() == 0;

    // Out of range cases
    if (n <= 0) return last;
    if (n >= d) return first;


    // Single word case
    if (std::next(first.base(), is_last_aligned) == last.base()) {
        *first.base() = _bitblend<word_type>(
                *first.base(),
                ((
                    *first.base() & (
                        static_cast<word_type>(-1) >> (
                            digits - (is_last_aligned ? digits : last.position())
                        )
                    )
                )) >> n,
                first.position(),
                (is_last_aligned ? digits : last.position()) - first.position()
        );
        return bit_iterator<RandomAccessIt>(
                first.base(), 
                first.position() + d - n
        );
    }

    // More initialization
    size_type word_shifts = n / digits; 
    size_type remaining_bitshifts = n - digits*(word_shifts);

    // Multiple word case
    word_type first_value = *first.base();
    word_type last_value = !is_last_aligned ? *last.base() : 0;

    // Shift words to the left using std::shift 
    RandomAccessIt new_last_base = STD_SHIFT_LEFT(first.base(), 
                                    last.base(),
                                    word_shifts
    );
    if (!is_last_aligned) {
        // Mask out-of-range bits so that we don't incorporate them
        *last.base() &= (static_cast<word_type>(1) << last.position()) - 1; 
        *new_last_base = *last.base();
        if (word_shifts > 0) {
            *last.base() = 0;
        }
    }
    // Shift bit sequence to the lsb 
    if (remaining_bitshifts) {
        RandomAccessIt it = first.base();

#ifdef BITLIB_HWY
        // Align to 64 bit boundary
        for (; std::next(it, is_last_aligned) != new_last_base && !is_aligned(&*it, 64); it++) {
            *it = _shrd<word_type>(*it, *std::next(it), remaining_bitshifts);
        }

        const hn::ScalableTag<word_type> d;
        for (; std::distance(it, new_last_base) >= hn::Lanes(d) + 10 + !is_last_aligned; it += hn::Lanes(d))
        {
            const auto v = hn::ShiftRightSame(hn::Load(d, &*it), remaining_bitshifts);
            const auto v_plus1 = hn::ShiftLeftSame(hn::LoadU(d, &*(it+1)), digits - remaining_bitshifts);
            hn::Store(v | v_plus1, d, &*it);
        }
#endif
        // _shrd all words except the last
        for (; std::next(it, is_last_aligned) != new_last_base; ++it) {
            *it = _shrd<word_type>(*it, *std::next(it), remaining_bitshifts);
        }
        // For the last word simply right shift
        *it >>= remaining_bitshifts;
    }
    // Blend bits of the first element
    if (!is_first_aligned) {
        *first.base() = _bitblend<word_type>(
                first_value,
                *first.base(),
                first.position(),
                digits - first.position()
        );
    }
    // Blend bits of the last element
    if (!is_last_aligned) {
        *last.base() = _bitblend<word_type>(
                *last.base(),
                last_value,
                last.position(),
                digits - last.position()
        );
    }
    //TODO is this more or less inefficient than having a latent iterator?
    bit_iterator<RandomAccessIt> d_last = next(first, d-n);
    return d_last;
}

template <class RandomAccessIt>
bit_iterator<RandomAccessIt> shift_right(
        bit_iterator<RandomAccessIt> first,
        bit_iterator<RandomAccessIt> last,
        typename bit_iterator<RandomAccessIt>::difference_type n
) {
    // Types and constants
    using word_type = typename bit_iterator<RandomAccessIt>::word_type;
    using size_type = typename bit_iterator<RandomAccessIt>::size_type;

    // Initialization
    const bool is_first_aligned = first.position() == 0;
    const bool is_last_aligned = last.position() == 0;
    constexpr auto digits = binary_digits<word_type>::value;
    auto d = distance(first, last);

    // Out of range cases
    if (n <= 0) return first;
    else if (n >= d) return last;

    // Single word case
    if (std::next(first.base(), is_last_aligned) == last.base()) {
        *first.base() = _bitblend<word_type>(
                *first.base(),
                (
                    *first.base() & (
                        static_cast<word_type>(-1) << first.position()
                    )
                ) << n,
                first.position(),
                (is_last_aligned ? digits : last.position()) - first.position()
        );
        return bit_iterator<RandomAccessIt>(
                first.base(), 
                first.position() +  n
        );
    }

    // More initialization
    size_type word_shifts = n / digits; 
    size_type remaining_bitshifts = n - digits*(word_shifts);

    // Multiple word case
    word_type first_value = *first.base();
    word_type last_value = !is_last_aligned ? *last.base() : 0;
    word_type mask = is_first_aligned ? 
        static_cast<word_type>(-1)
        : 
        static_cast<word_type>(
                (static_cast<word_type>(1) << (digits - first.position())) - 1
        ) << first.position();
    *first.base() = *first.base() & mask;
    // Shift words to the right
    RandomAccessIt new_first_base = STD_SHIFT_RIGHT(
        first.base(), 
        std::next(
            last.base(), 
            !is_last_aligned),
        word_shifts
    );
    bit_iterator<RandomAccessIt> d_first(new_first_base, first.position());
    // Shift bit sequence to the msb 
    if (remaining_bitshifts) {
        auto it = is_last_aligned ? last.base() - 1 : last.base();

#ifdef BITLIB_HWY
        // Align to 64 bit boundary
        const hn::ScalableTag<word_type> d;
        for (; it != new_first_base  && !is_aligned(&*(it - hn::Lanes(d) + 1), 64); it--) {
            *it = _shld<word_type>(*it, *(it - 1), remaining_bitshifts);
        }

        for (; std::distance(new_first_base, it) >= hn::Lanes(d); it -= hn::Lanes(d))
        {
            const auto v = hn::ShiftLeftSame( 
                    hn::Load(d, &*(it - hn::Lanes(d) + 1)), 
                    remaining_bitshifts);
            const auto v_plus1 = hn::ShiftRightSame(
                    hn::LoadU(d, &*(it - hn::Lanes(d))), 
                    digits - remaining_bitshifts);
            hn::Store(v | v_plus1, d, &*(it - hn::Lanes(d) + 1));
        }
#endif

        for(; it != new_first_base; --it) {
            *it = _shld<word_type>(*it, *(it - 1), remaining_bitshifts);
        }
        *it <<= remaining_bitshifts;
    }
    // Blend bits of the first element
    if (!is_first_aligned) {
        *first.base() = _bitblend<word_type>(
                first_value,
                *first.base(),
                first.position(),
                digits - first.position()
        );
    }
    // Blend bits of the last element
    if (!is_last_aligned) {
        *last.base() = _bitblend<word_type>(
                *last.base(),
                last_value,
                last.position(),
                digits - last.position()
        );
    }
    advance(d_first, remaining_bitshifts);
    return d_first;
}
// -------------------------------------------------------------------------- //



// ========================================================================== //
} // namespace bit
#endif // _SHIFT_HPP_INCLUDED
// ========================================================================== //
