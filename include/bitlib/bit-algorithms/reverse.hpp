// =============================== reverse.hpp ================================= //
// Project: The Experimental Bit Algorithms Library
// Name: copy.hpp
// Description: Implementation of reverse
// Creator: Vincent Reverdy
// Contributor: Vincent Reverdy [2019]
// License: BSD 3-Clause License
// ========================================================================== //
#ifndef _REVERSE_HPP_INCLUDED
#define _REVERSE_HPP_INCLUDED
#pragma once
// ========================================================================== //



// ================================ PREAMBLE ================================ //
// C++ standard library
// Project sources
// Third-party libraries
// Miscellaneous
namespace bit {
// ========================================================================== //



// --------------------------- Reverse Algorithms --------------------------- //
// Status: complete
template <class RandomAccessIt>
constexpr void reverse(
    bit_iterator<RandomAccessIt> first,
    bit_iterator<RandomAccessIt> last
)
{
    // Assertions
    _assert_range_viability(first, last);

    // Types and constants
    using word_type = typename bit_iterator<RandomAccessIt>::word_type;
    using size_type = typename bit_iterator<RandomAccessIt>::size_type;
    constexpr size_type digits = binary_digits<word_type>::value;

    // Initialization
    const bool is_first_aligned = first.position() == 0;
    const bool is_last_aligned = last.position() == 0;
    size_type gap = (digits - last.position()) * !is_last_aligned;
    auto it = first.base();
    word_type first_value = {};
    word_type last_value = {};

    // Reverse when bit iterators are aligned
    if (is_first_aligned && is_last_aligned) {
        std::reverse(first.base(), last.base());
        std::transform(it, last.base(), it, [](word_type w) { return _bitswap(w); });
        // Reverse when bit iterators do not belong to the same underlying word
    } else if (first.base() != last.base()) {
        // Save first and last element
        first_value = *first.base();
        last_value = *std::prev(last.base(), is_last_aligned);
        // Reverse the underlying sequence
        std::reverse(first.base(), std::next(last.base(), !is_last_aligned));
        // Bitswap every element of the underlying sequence
        std::transform(
                first.base(),
                std::next(last.base(), !is_last_aligned),
                first.base(),
                [](word_type w) { return _bitswap(w); }
        );
        // Shift the underlying sequence to the left
        if (first.position() < gap) {
            gap = gap - first.position();
            shift_left(
                bit_iterator(first.base()),
                bit_iterator(std::next(last.base())),
                gap);
            it = first.base();
        // Shift the underlying sequence to the right
        } else if (first.position() > gap) {
            gap = first.position() - gap;
            shift_right(
                bit_iterator(first.base()),
                bit_iterator(std::next(last.base(), !is_last_aligned)),
                gap);
            it = first.base();
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
        // Reverse when bit iterators belong to the same underlying word
    } else {
        *it = _bitblend<word_type>(
                *it,
                _bitswap<word_type>(*it >> first.position()) >> gap,
                first.position(),
                last.position() - first.position()
        );
    }
}


// -------------------------------------------------------------------------- //



// ========================================================================== //
} // namespace bit
#endif // _REVERSE_HPP_INCLUDED
// ========================================================================== //
