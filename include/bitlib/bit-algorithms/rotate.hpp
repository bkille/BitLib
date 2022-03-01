// ================================= ROTATE ================================= //
// Project:         The Experimental Bit Algorithms Library
// Name:            rotate.hpp
// Description:     bit_iterator overloads for std::rotate
// Creator:         Vincent Reverdy
// Contributor(s):  Bryce Kille [2019]
// License:         BSD 3-Clause License
// ========================================================================== //
#ifndef _ROTATE_HPP_INCLUDED
#define _ROTATE_HPP_INCLUDED
// ========================================================================== //



// ============================== PREAMBLE ================================== //
// C++ standard library
#include <iostream>
// Project sources
#include "bit_algorithm.hpp"
// Third-party libraries
// Miscellaneous
namespace bit {
// ========================================================================== //

// Rotates a range by copying [first...n_first) to the stack, then shifting 
// the range to the left and appending the copied section to the end.
//
// Note: distance(first, n_first) <= 3*digits
template <class ForwardIt, int BufferSize>
bit_iterator<ForwardIt> rotate_via_copy_begin(
   bit_iterator<ForwardIt> first, 
   bit_iterator<ForwardIt> n_first,
   bit_iterator<ForwardIt> last
) {
    // Types and constants
    using word_type = typename bit_iterator<ForwardIt>::word_type;
    using size_type = typename bit_iterator<ForwardIt>::size_type;
    constexpr size_type digits = binary_digits<word_type>::value;

    size_type k = distance(first, n_first);
    assert(k <= BufferSize*digits);
    word_type copy_arr[BufferSize]; 
    copy_arr[0] = *first.base();
    ForwardIt it = ++first.base();
    short unsigned int pos = 1;
    short int bits_left_to_copy = k - (digits - first.position());
    while (bits_left_to_copy > 0) {
        copy_arr[pos++] = *it++;
        bits_left_to_copy -= digits;
    }
    bit_iterator<ForwardIt> ret = shift_left(first, last, k);
    copy(
        bit_iterator<word_type*>(copy_arr, first.position()), 
        bit_iterator<word_type*>(
                copy_arr, 
                first.position()
        ) + k,
        ret
    );
    return ret;
}

// Rotates a range by copying [n_first, last) to the stack, then shifting 
// the range to the right and prepending the copied section to the beginning.
//
// Note: distance(n_first, last) <= 3*digits
template <class ForwardIt, int BufferSize>
bit_iterator<ForwardIt> rotate_via_copy_end(
   bit_iterator<ForwardIt> first, 
   bit_iterator<ForwardIt> n_first,
   bit_iterator<ForwardIt> last
) {
    // Types and constants
    using word_type = typename bit_iterator<ForwardIt>::word_type;
    using size_type = typename bit_iterator<ForwardIt>::size_type;
    constexpr size_type digits = binary_digits<word_type>::value;

    size_type k = distance(n_first, last);
    assert(k <= BufferSize*digits);
    word_type copy_arr[BufferSize]; 
    copy_arr[0] = *n_first.base();
    ForwardIt it = ++n_first.base();
    short unsigned int pos = 1;
    short int bits_left_to_copy = k - (digits - n_first.position());
    while (bits_left_to_copy > 0) {
        copy_arr[pos++] = *it++;
        bits_left_to_copy -= digits;
    }
    bit_iterator<ForwardIt> ret = shift_right(first, last, k);
    copy(
        bit_iterator<word_type*>(copy_arr, n_first.position()), 
        bit_iterator<word_type*>(
                copy_arr, 
                n_first.position()
        ) + k,
        first
    );
    return ret;
}

// Rotates a range using forward iterators. Algorithm logic from the GCC
// implementation
template <class ForwardIt>
bit_iterator<ForwardIt> rotate_via_raw(
   bit_iterator<ForwardIt> first, 
   bit_iterator<ForwardIt> n_first,
   bit_iterator<ForwardIt> last,
   std::forward_iterator_tag
) {
    // Types and constants
    using word_type = typename bit_iterator<ForwardIt>::word_type;
    using difference_type = typename ForwardIt::difference_type;
    constexpr difference_type digits = binary_digits<word_type>::value;

    // Initialization
    bit_iterator<ForwardIt> first2 = n_first;
    ForwardIt it = std::next(first.base());

    // 1. Swap the first (last - n_first) bits to the correct position
    // CORRESPONDING GCC CODE:
    //  do {
    //      std::iter_swap(first, first2);
    //      ++first;
    //      ++first2;
    //      if (first == n_first)
    //          n_first = first2;
    //  } while (first2 != last);
    
    // 1a. Align first so that we are swapping a whole word
    first2 = swap_ranges(
            first,
            bit_iterator<ForwardIt>(it),
            first2
    );
    first = bit_iterator<ForwardIt>(it);

    // 1b. Swap chunks of size [digits] at a time, until no chunks remain.
    while (!is_within<digits-1>(first2, last)) {
        // Note 1b. If we swap past n_first, we need to update its location.
        if (distance(first, n_first) < digits) 
            n_first = first2 + distance(first, n_first);
        first2 = swap_ranges(
                first,
                first + digits,
                first2
        );
        first = bit_iterator<ForwardIt>(++it);
    }
    // 1c. Swap remaining chunk (which must be smaller than [digits]
    difference_type remainder = distance(first2, last);
    if (is_within(first, n_first, remainder))
        n_first = first2 + distance(first, n_first);
    first2 = swap_ranges(
            first,
            first + remainder,
            first2
    );
    advance(first, remainder);

    bit_iterator<ForwardIt> ret = first;
    first2 = n_first;

    // 2. At this point, positions first...first+(distance(n_first, last))
    // are correct. The subproblem is to now rotate on the current 
    // first, n_first, last
    //
    // We use the same strategy as above, with the change that now
    // when we encounter first2 == last in the loop, we set first2
    // to n_first. 
    //
    // 2a. See if we can use our rotates that simply copy and shift
    // TODO maybe we should check this at every iteratoration?
    if (is_within<2*digits>(first, n_first)) {
        rotate_via_copy_begin<ForwardIt, 3>(first, n_first, last);
    } else if (is_within<2*digits>(n_first, last)) {
        rotate_via_copy_end<ForwardIt, 3>(first, n_first, last);
    } else {
        // CORRESPONDING GCC CODE:
        //  while (first2 != last) {
        //      std::iter_swap(first, first2);
        //      ++first;
        //      ++first2;
        //      if (first == n_first)
        //          n_first = first2;
        //      else if (first2 == last)
        //          first2 = n_first;
        //  }
        //
        // 2b. Align the first iterator and use the same strategy as in 1b.
        it = std::next(first.base());
        first2 = swap_ranges(
                first,
                bit_iterator<ForwardIt>(it),
                first2
        );
        first = bit_iterator<ForwardIt>(it);
        while (first2 != last) {
            if (is_within<digits-1>(first, n_first)) {
                if (is_within(first2, last, distance(first, n_first)-1)) { 
                    // We reach the last iterator
                    remainder = distance(first2, last);
                    first2 = swap_ranges(
                            first,
                            first + distance(first2, last),
                            first2
                    );
                    first += remainder;
                    first2 = n_first;
                } else {
                    // We reach the n_first iterator
                    first2 = swap_ranges(
                            first,
                            first + distance(first, n_first),
                            first2
                    );
                    first += distance(first, n_first);
                    n_first = first2;
                }
                if (is_within<2*digits>(first, n_first)) {
                    rotate_via_copy_begin<ForwardIt, 3>(first, n_first, last);
                    break;
                } else if (is_within<2*digits>(n_first, last)) {
                    rotate_via_copy_end<ForwardIt, 3>(first, n_first, last);
                    break;
                }
            }
            else if (is_within<digits-1>(first2, last)) {
                // We reach the n_first iterator
                remainder = distance(first2, last);
                first2 = swap_ranges(
                        first,
                        first + distance(first2, last),
                        first2
                );
                advance(first, remainder);
                first2 = n_first;
                if (is_within<2*digits>(first, n_first)) {
                    rotate_via_copy_begin<ForwardIt, 3>(first, n_first, last);
                    break;
                } else if (is_within<2*digits>(n_first, last)) {
                    rotate_via_copy_end<ForwardIt, 3>(first, n_first, last);
                    break;
                }
            }
            else {
                first2 = swap_ranges(
                        first,
                        first + digits,
                        first2
                );
                first = bit_iterator<ForwardIt>(++it);
            }
        }
    }
    return ret;
}

// Rotates a range using bidirectional iterators. Algorithm logic from the GCC
// implementation
template <class BidirectionalIt>
bit_iterator<BidirectionalIt> rotate_via_raw(
   bit_iterator<BidirectionalIt> first, 
   bit_iterator<BidirectionalIt> n_first,
   bit_iterator<BidirectionalIt> last,
   std::bidirectional_iterator_tag
) {
    // Types and constants
    using word_type = typename bit_iterator<BidirectionalIt>::word_type;
    using size_type = typename bit_iterator<BidirectionalIt>::size_type;
    using difference_type = 
        typename bit_iterator<BidirectionalIt>::difference_type;
    constexpr difference_type digits = binary_digits<word_type>::value;

    // first, ... , n_first-1, n_first, ... , last-1, last
    reverse(first, n_first);
    // n_first-1, ... , first, n_first, ... , last-1, last
    reverse(n_first, last);
    // n_first-1, ... , first, last-1, ... , n_first, last
    
    // Now we reverse the entire sequence. We split the reverse up into two
    // sections:
    // 1. Swap the outside elements until we come across n_first
    //    
    //     CORRESPONDING GCC CODE:
    //     while (first != n_first && n_first != last) { 
    //         std::iter_swap(first++, --last);
    //     }
    //
    // 2. Call reverse on the remaining range.
    
    
    // 1a. Swap partial first word to align the first iterator.
    size_type digits_to_swap = digits - first.position();
    word_type temp1 = _bitswap(*first.base());
    advance(last, -(digits_to_swap));
    word_type temp2 = _bitswap(get_word<word_type>(last, digits_to_swap));
    write_word<word_type>(temp1, last, digits_to_swap);
    write_word<word_type>(temp2 >> first.position(), first, digits_to_swap);
    first += digits_to_swap;

    // 1b. Swap full words.
    BidirectionalIt it = first.base();
    while (!is_within<digits>(first, n_first) && !is_within<digits>(n_first, last)) {
        advance(last, -digits);
        temp2 = _bitswap(get_word<word_type>(last, digits));
        *it = _bitswap(*it);
        std::swap(*it, temp2);
        write_word<word_type>(temp2, last);
        ++it;
    }

    // 1.c Swap remaining bits.
    first = bit_iterator<BidirectionalIt>(it);
    digits_to_swap = (is_within<digits>(first, n_first)
            && !is_within(n_first, last, distance(first, n_first)))
            ? n_first.position() : distance(n_first, last); 
    temp1 = _bitswap(*first.base());
    advance(last, -(digits_to_swap));
    temp2 = _bitswap(get_word<word_type>(last, digits_to_swap));
    write_word<word_type>(temp1 >> (digits - digits_to_swap), last, digits_to_swap);
    write_word<word_type>(temp2 >> (digits - digits_to_swap), first, digits_to_swap);
    first += digits_to_swap;

    // 2. Reverse remaining range
    if (first == n_first) {
        reverse(n_first, last);
        return last;
    } else {
        reverse(first, n_first);
        return first;
    }
}

// Rotates a range using random-access iterators. Algorithm logic from the GCC
// implementation
template <class RandomAccessIt>
bit_iterator<RandomAccessIt> rotate_via_raw(
   bit_iterator<RandomAccessIt> first, 
   bit_iterator<RandomAccessIt> n_first,
   bit_iterator<RandomAccessIt> last,
   std::random_access_iterator_tag
) {
    // Types and constants
    using word_type = typename bit_iterator<RandomAccessIt>::word_type;
    using size_type = typename bit_iterator<RandomAccessIt>::size_type;
    using difference_type = 
        typename bit_iterator<RandomAccessIt>::difference_type;
    constexpr difference_type digits = binary_digits<word_type>::value;

    difference_type k = n_first - first;
    difference_type n = last - first;
    
    if (k == n - k) {
        swap_ranges(first, n_first, n_first);
        return n_first;
    }

    bit_iterator<RandomAccessIt> p = first;
    bit_iterator<RandomAccessIt> ret = first + (last - n_first);

    for (;;) {
        if (k < n - k) {
            if (k <= digits) {
                // BENCHMARK NOTE: may be better to do k <= 3*digits and use
                // the rotate_via_copy method.
                word_type temp_word = get_word<word_type>(p, k);
                bit_iterator<RandomAccessIt> temp_it = shift_left(p, p + n, k);
                write_word<word_type>(temp_word, temp_it, k);
                return ret;
            }
            bit_iterator<RandomAccessIt> q = p + k;
            unsigned int full_swaps = (n - k) / k;
            size_type remainder = (n - k) - full_swaps*k;
            while (full_swaps > 0) {
                swap_ranges(p, q, q);
                p += k;
                q += k;
                --full_swaps;
            }
            swap_ranges(p, p + remainder, q);
            p += remainder;
            q += remainder;
            n %= k;
            if (n == 0)
                return ret;
            std::swap(n, k);
            k = n - k;
        } else {
            k = n - k;
            if (k <= digits) {
                word_type temp_word = get_word<word_type>(p + n - k, k);
                shift_right(p, p + n, k);
                write_word<word_type>(temp_word, p, k);
                return ret;
            }
            bit_iterator<RandomAccessIt> q = p + n;
            p = q - k;
            unsigned int full_swaps = (n - k) / k;
            size_type remainder = (n - k) - full_swaps*k;
            while (full_swaps > 0) {
                p -= k;
                q -= k;
                swap_ranges(p, q, q);
                --full_swaps;
            }
            p -= remainder;
            q -= remainder;
            swap_ranges(p, p + remainder, q);
            n %= k;
            if (n == 0)
                return ret;
            std::swap(n, k);
        }
    }
}

// Main function for implementing the bit overload of std::rotate.
template <class ForwardIt>
bit_iterator<ForwardIt> rotate(
   bit_iterator<ForwardIt> first, 
   bit_iterator<ForwardIt> n_first,
   bit_iterator<ForwardIt> last
) {
    // Assertions
    _assert_range_viability(first, n_first);
    _assert_range_viability(n_first, last);
    _assert_range_viability(first, last);
    //if (first == n_first) return n_first;
    if (first == n_first) return last;

    // Types and constants
    using word_type = typename bit_iterator<ForwardIt>::word_type;
    using size_type = typename bit_iterator<ForwardIt>::size_type;
    using difference_type = 
        typename bit_iterator<ForwardIt>::difference_type;
    constexpr difference_type digits = binary_digits<word_type>::value;

    // Initialization
    const bool is_first_aligned = first.position() == 0;
    const bool is_last_aligned = last.position() == 0;

    // Within the same word
    if (std::next(first.base(), is_last_aligned) == last.base()) {
        if (is_first_aligned && is_last_aligned) {
            *first.base() = 
                (*first.base() >> n_first.position())
                | 
                static_cast<word_type>(
                        *first.base() << (digits - n_first.position())
            );
            return std::next(first, digits - n_first.position());
        } else {
            size_type last_pos = is_last_aligned ? digits : last.position();
            size_type k = n_first.position() - first.position();
            size_type p = last_pos - n_first.position();
            size_type d = last_pos - first.position();
            
            word_type mask = ((1ULL << d) - 1) << first.position();
            word_type rotated = *first.base() & mask;
            rotated = static_cast<word_type>(rotated >> k) 
                | static_cast<word_type>(rotated << p);
            *first.base() = _bitblend<word_type>(
                *first.base(),
                rotated,
                first.position(),
                d
            );
            return std::next(first, p);
        }
    }

    // Single word subcases 
    if (is_within<digits>(first, n_first)) {
        size_type k = distance(first, n_first);
        word_type temp = get_word(first, k);
        bit_iterator<ForwardIt> new_last = shift_left(first, last, k);
        write_word<word_type, ForwardIt>(temp, new_last, static_cast<word_type>(k));
        return new_last;
    } else if (is_within<digits>(n_first, last)) {
        size_type p = distance(n_first, last);
        word_type temp = get_word(n_first, p);
        auto new_last = shift_right(first, last, p);
        write_word(temp, first, static_cast<word_type>(p));
        return new_last;
    }
    return rotate_via_raw(
            first, 
            n_first, 
            last,
            typename std::iterator_traits<ForwardIt>::iterator_category()
    );
}


// Status: to do
//template <class ForwardIt>
//constexpr bit_iterator<ForwardIt> rotate(bit_iterator<ForwardIt> first,
    //bit_iterator<ForwardIt> n_first, bit_iterator<ForwardIt> last) {
    //(n_first, last);
    //return first;
//}

// Status: to do
template <class ExecutionPolicy, class ForwardIt>
bit_iterator<ForwardIt> rotate(ExecutionPolicy&& policy, 
    bit_iterator<ForwardIt> first, bit_iterator<ForwardIt> n_first,
    bit_iterator<ForwardIt> last) {
    (policy, n_first, last); 
    return first;
}

// ========================================================================== //
} // namespace bit
#endif // _ROTATE_HPP_INCLUDED
// ========================================================================== //
