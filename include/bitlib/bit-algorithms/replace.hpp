// ================================ REPLACE ================================ //
// Project: The Experimental Bit Algorithms Library
// Name: replace.hpp
// Description: bit_iterator overloads for std::replace
// Creator: Vincent Reverdy
// Contributor(s): 
// License: BSD 3-Clause License
// ========================================================================== //
#ifndef _REPLACE_HPP_INCLUDED
#define _REPLACE_HPP_INCLUDED
// ========================================================================== //

// ============================== PREAMBLE ================================== //
// C++ standard library
// Project sources
// Third-party libraries
// Miscellaneous
namespace bit {
// ========================================================================== //


/*
 * Case 0: ^10011101^
 * Case 1: ^10011^101
 * Case 2: 100^1110^1
 * Case 3: 10011^101^
 */
template <class ForwardIt>
constexpr void replace(bit_iterator<ForwardIt> first, 
    bit_iterator<ForwardIt> last, bit_value old_value, bit_value new_value) {

    if (old_value == new_value) {
        return;
    }

    using word_type = typename bit_iterator<ForwardIt>::word_type;
    constexpr std::size_t num_digits = binary_digits<word_type>::value;

    bit_iterator<ForwardIt> cursor = first;
    word_type mask;
    word_type all_ones = _all_ones();

    if (!_is_aligned_lsb(cursor)) {
        //word_type first_word = *cursor.base();
        if (_in_same_word(cursor, last)) {
              // Case 2
              mask = _shift_towards_msb(all_ones, cursor.position());
              mask &= _shift_towards_lsb(all_ones, num_digits - last.position());
              if (old_value == bit0) {
                  *(cursor.base()) |= mask;
              } else {
                  *(cursor.base()) &= ~mask;
              }
              return;
        }
        // Case 1
        mask = _shift_towards_msb(all_ones, cursor.position());

        if (old_value == bit0) {
            *(cursor.base()) |= mask;
        } else {
            *(cursor.base()) &= ~mask;
        }

        cursor = bit_iterator(std::next(cursor.base()));
    }

    ForwardIt word_cursor = cursor.base();

    word_type full_word_value = old_value == bit0 ? _all_ones() : _all_zeros();

    while (word_cursor != last.base()) {
        // Case 0
        *word_cursor = full_word_value;
        ++word_cursor;
    }

    cursor = bit_iterator(word_cursor);

    if (cursor == last) {
        return;
    }

    // Case 3
    mask = _shift_towards_lsb(all_ones, num_digits - last.position());

    if (old_value == bit0) {
        *(cursor.base()) |= mask;
    } else {
        *(cursor.base()) &= ~mask;
    }
}

// Status: to do
template <class ExecutionPolicy, class ForwardIt>
void replace(ExecutionPolicy&& policy, bit_iterator<ForwardIt> first,
    bit_iterator<ForwardIt> last, bit_value old_value, bit_value new_value) {
    (policy, first, last, old_value, new_value); 
}

// Status: on hold
template <class ForwardIt, class UnaryPredicate>
constexpr void replace_if(bit_iterator<ForwardIt> first, 
    bit_iterator<ForwardIt> last, UnaryPredicate p, bit_value new_value) {
    (first, last, p, new_value);
}

// Status: on hold
template <class ExecutionPolicy, class ForwardIt, class UnaryPredicate>
void replace_if(ExecutionPolicy&& policy, bit_iterator<ForwardIt> first,
    bit_iterator<ForwardIt> last, UnaryPredicate p, bit_value new_value) {
    (policy, first, last, p, new_value);
}

// ========================================================================== //
} // namespace bit

#endif // _REPLACE_HPP_INCLUDED
// ========================================================================== //
