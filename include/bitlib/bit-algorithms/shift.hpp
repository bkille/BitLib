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
HWY_BEFORE_NAMESPACE();
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
    using difference_type = typename bit_iterator<RandomAccessIt>::difference_type;
    constexpr size_type digits = binary_digits<word_type>::value;

    // Initialization
    auto d = bit::distance(first, last);
    const bool is_first_aligned = first.position() == 0;
    const bool is_last_aligned = last.position() == 0;
    auto middle = first + n;

    // Out of range cases
    if (n <= 0) return last;
    if (n >= d) 
    {
        //bit::fill(first, last, bit::bit0);    
        return first;
    }

    // Single word case
    // Triggered if all relevant bits are in first.base()
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

    // Triggered if all remaining bits can fit in a word
    if (d - n <= digits)
    {
        word_type new_word = get_word<word_type, RandomAccessIt>(middle, d - n);
        write_word<word_type, RandomAccessIt>(new_word, first, d - n);
        first += d - n;
        return first;
    }
    // Multiple word case
    word_type first_value = *first.base();
    word_type last_value = !is_last_aligned ? *last.base() : 0;

    // Align first
    if (!is_first_aligned) 
    {
        if (first.position() >= middle.position())
        {
            *first.base() = _bitblend<word_type>(
                    *first.base(),
                    (*middle.base()) << (first.position() - middle.position()),
                    first.position(),
                    digits - first.position()
            );
        }
        else
        {
            const int n1 = digits - middle.position();
            const int n2 = digits - first.position() - n1;
            *first.base() = _bitblend<word_type>(
                    *first.base(),
                    (*middle.base()) >> (middle.position() - first.position()),
                    first.position(),
                    n1
            );
            *first.base() = _bitblend<word_type>(
                    *first.base(),
                    (*std::next(middle.base())) << (digits - n2),
                    first.position() + n1,
                    n2
            );
        }
        const int shifted = std::min<difference_type>(d - n, (digits - first.position()));
        first += shifted;
        middle += shifted;
    }
    if (middle.base() == last.base())
    {
        const int bits_left = last.position() - middle.position();
        if (bits_left > 0)
        {
            *first.base() = _bitblend<word_type>(
                    *first.base(),
                    *middle.base() >> middle.position(),
                    0,
                    bits_left
            );
            first += bits_left;
        }
        // https://en.cppreference.com/w/cpp/algorithm/shift
        // "Elements that are in the original range but not the new range 
        // are left in a valid but unspecified state."
        //
        //bit::fill(first, last, bit::bit0);
        return first;
    }

    // More initialization
    d = bit::distance(first, last);
    const size_type word_shifts = n / digits;
    const size_type offset = middle.position();

    // At this point, first is aligned
    if (offset == 0)
    {
        first = bit::bit_iterator<RandomAccessIt>(
                STD_SHIFT_LEFT(first.base(),
                    last.base(),
                    word_shifts),
                0
        );
        if (!is_last_aligned)
        {
            write_word<word_type, RandomAccessIt>(*last.base(), first, last.position());
            first += last.position();
        }
        // https://en.cppreference.com/w/cpp/algorithm/shift
        // "Elements that are in the original range but not the new range 
        // are left in a valid but unspecified state."
        //
        //bit::fill(first, last, bit::bit0);
        return first;
    }

    // Shift bit sequence to the lsb
#ifdef BITLIB_HWY
    // Align to 64 bit boundary
    while (std::next(middle.base()) < last.base() && !is_aligned(&*first.base(), 64)) {
        *first.base() = _shrd<word_type>(*middle.base(), *std::next(middle.base()), offset);
        first += digits;
        middle += digits;
    }

    const hn::ScalableTag<word_type> d_tag;
    while (std::distance(middle.base(), last.base()) >= hn::Lanes(d_tag) + 10 + !is_last_aligned)
    {
        const auto v = hn::ShiftRightSame(hn::LoadU(d_tag, &*middle.base()), offset);
        const auto v_plus1 = hn::ShiftLeftSame(hn::LoadU(d_tag, &*(middle.base()+1)), digits - offset);
        hn::Store(v | v_plus1, d_tag, &*first.base());
        first += hn::Lanes(d_tag)*digits;
        middle += hn::Lanes(d_tag)*digits;
    }
#endif
    auto first_base = first.base();
    auto middle_base = middle.base();

    while (std::next(middle_base) < last.base()) {
        *first_base = _shrd<word_type>(*middle_base, *std::next(middle_base), offset);
        first_base++;
        middle_base++;;
    }
    first = bit_iterator<RandomAccessIt>(first_base, 0);
    middle = bit_iterator<RandomAccessIt>(middle_base, middle.position());

    // If middle is now penultimate word
    if (std::next(middle.base()) == last.base())
    {
        *first.base() = _bitblend<word_type>(
                *first.base(),
                *middle.base() >> offset,
                0,
                digits - offset
        );
        first += digits - offset;
        middle += digits - offset;
    }

    if (!is_last_aligned)
    {
        const difference_type bits_left = last.position() - middle.position();
        const word_type new_word = get_word<word_type, RandomAccessIt>(middle, bits_left);
        write_word<word_type, RandomAccessIt>(new_word, first, bits_left);
        first += bits_left;
    }

    //bit::fill(first, last, bit::bit0);
    return first;
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
    auto d = bit::distance(first, last);
    bit_iterator<RandomAccessIt> middle = last - n;

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
        return first + n;
    }

    // Align last
    if (last.position() != 0)
    {
        const size_type bits_to_align = std::min<size_type>(
                last.position(), 
                bit::distance(first, middle));
        const word_type word_to_write = get_word<word_type, RandomAccessIt>(
                middle - bits_to_align, 
                bits_to_align);
        write_word<word_type, RandomAccessIt>(
                word_to_write,
                last - bits_to_align, 
                bits_to_align);
        middle -= bits_to_align;
        last   -= bits_to_align;

        // Nothing left to do
        if (middle == first)
            return first + n;
    }

    // More initialization
    const size_type word_shifts = n / digits;
    const size_type offset = middle.position();

    // Shift bit sequence to the msb
    if (offset == 0) {
        auto new_first = bit::bit_iterator<RandomAccessIt>(
                STD_SHIFT_RIGHT(
                    first.base(),
                    last.base(),
                    word_shifts),
                first.position()
        );
        // https://en.cppreference.com/w/cpp/algorithm/shift
        // "Elements that are in the original range but not the new range 
        // are left in a valid but unspecified state."
        //
        //bit::fill(first, new_first, bit::bit0);
        return first + n;
    }

    if (bit::distance(first, middle) >= digits)
    {
#ifdef BITLIB_HWY
        // Align to 64 bit boundary
        const hn::ScalableTag<word_type> d;
        while (std::prev(middle.base()) > first.base() && !is_aligned(&*(last.base() - hn::Lanes(d)), 64)) {
            *std::prev(last.base()) = _shrd<word_type>(*std::prev(middle.base()), *middle.base(), offset);
            last -= digits;
            middle -= digits;
        }

        while (std::distance(first.base(), middle.base()) > hn::Lanes(d) + 1)
        {
            const auto v = hn::ShiftRightSame(
                    hn::LoadU(d, &*(middle.base() - hn::Lanes(d))),
                    offset);
            const auto v_plus1 = hn::ShiftLeftSame(
                    hn::LoadU(d, &*(middle.base() - hn::Lanes(d) + 1)),
                    digits - offset);
            hn::Store(v | v_plus1, d, &*(last.base() - hn::Lanes(d)));

            last -= digits * hn::Lanes(d);
            middle -= digits * hn::Lanes(d);
        }
#endif

        auto last_base_prev   = std::prev(last.base());
        auto middle_base_prev = std::prev(middle.base());

        while (middle_base_prev + (first.position() <= middle.position())  > first.base()) {
            *last_base_prev = _shrd<word_type>(*middle_base_prev, *std::next(middle_base_prev), offset);
            last_base_prev--;
            middle_base_prev--;
        }

        last = bit_iterator<RandomAccessIt>(std::next(last_base_prev), last.position());
        middle = bit_iterator<RandomAccessIt>(std::next(middle_base_prev), middle.position());
    }

    if (first.position() != middle.position())
    {
        const size_type bits_to_align = bit::distance(first, middle);
        const word_type word_to_write = get_word<word_type, RandomAccessIt>(
                first, 
                bits_to_align);
        write_word<word_type, RandomAccessIt>(
                word_to_write,
                last - bits_to_align, 
                bits_to_align);
    }

    return first + n;
}
// -------------------------------------------------------------------------- //



// ========================================================================== //
} // namespace bit
#ifdef BITLIB_HWY
HWY_AFTER_NAMESPACE();
#endif
#endif // _SHIFT_HPP_INCLUDED
// ========================================================================== //
