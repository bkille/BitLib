// ========================= BIT ALGORITHM DETAILS  ========================= //
// Project: The Experimental Bit Algorithms Library
// Name: bit_algorithm_details.hpp
// Description: A set of utilities to assist in writing algorithms 
// Creator: Vincent Reverdy
// Contributor(s): Vincent Reverdy [2019]
//                 Collin Gress [2019]
//                 Bryce Kille [2019]
// License: BSD 3-Clause License
// ========================================================================== //
#ifndef _BIT_ALGORITHM_DETAILS_HPP_INCLUDED
#define _BIT_ALGORITHM_DETAILS_HPP_INCLUDED
// ========================================================================== //



// ============================== PREAMBLE ================================== //
// C++ standard library
// Project sources
#include "bit-iterator/bit.hpp"
// Third-party libraries
// Miscellaneous
namespace bit {
// ========================================================================== //



// -------------------------- Iterator Algorithms --------------------------- //
// Returns the number of increments needed to get to last from first. 
// May be negative if last comes before first (Only when input is RAI)
template <class InputIt>
typename bit_iterator<InputIt>::difference_type 
    distance(bit_iterator<InputIt> first,
             bit_iterator<InputIt> last
)
{
    _assert_range_viability(first, last); 
    using word_type = typename bit_iterator<InputIt>::word_type;
    using size_type = typename bit_iterator<InputIt>::size_type;
    constexpr size_type digits = binary_digits<word_type>::value;
    return std::distance(first.base(), last.base())*digits 
           + (last.position() - first.position());
}

// Increments the iterator n times. (If n is negative, the iterator is decremented n times)
template <class InputIt, class Distance>
void advance(bit_iterator<InputIt>& first, Distance n)
{
    first += n;    
}

template<class ForwardIt>
bit_iterator<ForwardIt> next(
        bit_iterator<ForwardIt> bit_it, 
        typename bit_iterator<ForwardIt>::difference_type n = 1 
) {
    return bit_it + n;
}

// -------------------------------------------------------------------------- //



// --------------------------- Utility Functions ---------------------------- //

// Returns distance(first, last) <= n
template <class InputIt>
bool is_within(
        bit_iterator<InputIt> first,
        bit_iterator<InputIt> last,
        typename InputIt::difference_type n
) {
    //using word_type = typename bit_iterator<InputIt>::word_type;
    //using size_type = typename bit_iterator<InputIt>::size_type;
    //constexpr size_type digits = binary_digits<word_type>::value;

    return distance(first, last) <= n;
}

template <int N, class InputIt>
constexpr bool is_within(
        bit_iterator<InputIt> first,
        bit_iterator<InputIt> last
) {
    using word_type = typename bit_iterator<InputIt>::word_type;
    //using size_type = typename bit_iterator<InputIt>::size_type;
    constexpr int digits = binary_digits<word_type>::value;
    constexpr int full_words = N / digits;
    constexpr int remainder_bits = N % digits;

    if constexpr (full_words > 0) {
        return (first.base() == last.base())
            || (std::next(first.base()) == last.base() && first.position() >= last.position())
            || is_within<N-digits>(first + digits, last)
        ;
    } else if (remainder_bits >= 0) {
        return (first.base() == last.base() 
                && first.position() + remainder_bits >= last.position()
               ) || (std::next(first.base()) == last.base()
                   && (static_cast<int>(first.position()) + remainder_bits - digits >= static_cast<int>(last.position()))
               )
        ;
    }
}

// Get next len bits beginning at start and store them in a word of type T
template <class T, class InputIt>
T get_word(bit_iterator<InputIt> first, T len=binary_digits<T>::value)
{
    using native_word_type = typename bit_iterator<InputIt>::word_type;
    constexpr T native_digits = binary_digits<native_word_type>::value; 
    constexpr T ret_digits = binary_digits<T>::value; 
    assert(ret_digits >= len);
    T offset = native_digits - first.position();
    T ret_word = *first.base() >> first.position();

    // We've already assigned enough bits
    if (len <= offset) { 
        return ret_word;
    } 

    InputIt it = std::next(first.base());
    len -= offset;
    // Fill up ret_word starting at bit [offset] using it
    // TODO define a mask and use the _bitblend that takes in the extra mask
    while (len > native_digits) {
        ret_word = _bitblend(
                ret_word,      
                static_cast<T>(static_cast<T>(*it) << offset),   
                offset,
                native_digits
        );
        ++it;
        offset += native_digits;
        len -= native_digits;
    }
    // Assign remaining len bits of last word
    ret_word = _bitblend(
            ret_word,            
            static_cast<T>(static_cast<T>(*it) << offset),   
            offset,
            len
    );
    return ret_word;
}

// Get next len bits beginning at start and store them in a word of type T
// If we reach `last` before we get len bits, break and return the current word
// bits_read will store the number of bits that we read.
//template <class T, class InputIt>
//T get_word(bit_iterator<InputIt> first, bit_iterator<InputIt> last,
        //T& bits_read, T len=binary_digits<T>::value
        //)
//{
    //using native_word_type = typename bit_iterator<InputIt>::word_type;
    //constexpr T native_digits = binary_digits<native_word_type>::value; 
    //constexpr T ret_digits = binary_digits<T>::value; 
    //assert(ret_digits >= len);
    //bits_read = native_digits - first.position();
    //T ret_word = *first.base() >> first.position();

    //// TODO vincent mentioned that we should aim for only 1 return function 
    //// per function. However I'm not sure how that can be accomplished here
    //// without suffering a minor performance loss
   
    //// We have reached the last iterator
    //if (first.base() == last.base()) {
        //bits_read -= (native_digits - last.position());
        //return ret_word;
    //}
    //// We've already assigned enough bits
    //if (len <= bits_read) {
        //return ret_word;
    //} 

    //InputIt it = std::next(first.base());
    //len -= bits_read;
    //// Fill up ret_word starting at bit [bits_read] using it
    //// TODO define a mask and use the _bitblend that takes in the extra mask
    //while (len > native_digits && it != last.base()) {
        //ret_word = _bitblend(
                //ret_word,      
                //static_cast<T>(static_cast<T>(*it) << bits_read),   
                //bits_read,
                //native_digits
        //);
        //++it;
        //bits_read += native_digits;
        //len -= native_digits;
    //}

    //// Assign remaining len bits of last word
    //if (it == last.base()) {
        //bits_read -= (native_digits - last.position());
        //ret_word = _bitblend(
                //ret_word,            
                //static_cast<T>(static_cast<T>(*it) << bits_read),   
                //bits_read,
                //last.position()
        //);
    //} else { 
        //ret_word = _bitblend(
                //ret_word,            
                //static_cast<T>(static_cast<T>(*it) << bits_read),   
                //bits_read,
                //len
        //);
    //}
    //return ret_word;
//}


// Writes len bits from src beginning at dstIt
template <class src_type, class OutputIt>
void write_word(src_type src, bit_iterator<OutputIt> dst_bit_it, 
        src_type len=binary_digits<src_type>::value
        )
{
    using dst_type = typename bit_iterator<OutputIt>::word_type;
    constexpr dst_type dst_digits = binary_digits<dst_type>::value; 
    constexpr dst_type src_digits = binary_digits<src_type>::value; 

    if constexpr (dst_digits >= src_digits) {
        if (dst_bit_it.position() == 0 && len == dst_digits) {
            *dst_bit_it.base() = src;
        }
        else {
            *dst_bit_it.base() = _bitblend<src_type>(
                   *dst_bit_it.base(),
                   src << dst_bit_it.position(),
                   dst_bit_it.position(),
                   std::min<src_type>(
                       dst_digits - dst_bit_it.position(), 
                       len
                   )
            ); 
            if (len > dst_digits - dst_bit_it.position()) {
                OutputIt overflow_dst = std::next(dst_bit_it.base()); 
                *overflow_dst = _bitblend<src_type>(
                        *overflow_dst,
                        src >> (dst_digits - dst_bit_it.position()),
                        0,
                        len - (dst_digits - dst_bit_it.position())
                    );
            }
        }
    } else {
        OutputIt it = dst_bit_it.base();
        if (dst_bit_it.position() != 0) {
            *it = _bitblend(
                    *it,
                    static_cast<dst_type>(src),
                    static_cast<dst_type>(-1) << dst_bit_it.position()
            );
            len -= dst_digits - dst_bit_it.position();
            // TODO would it be faster to jsut shift src every time it is
            // passed as an argument and keep track of how much we need to
            // shift?
            src >>= dst_digits - dst_bit_it.position();
            ++it;
        }
        while (len >= dst_digits) {
            *it = static_cast<dst_type>(src);
            src >>= dst_digits;
            len -= dst_digits;
            ++it;
        }
        if (len > 0 ) {
            *it = _bitblend(
                    *it,
                    static_cast<dst_type>(src),
                    (1 << len) - 1
            );
        }
    }
    return;
}


// Shifts the range [first, last) to the left by n, filling the empty
// bits with 0
// NOT OPTIMIZED. Will be replaced with std::shift eventually.
template <class ForwardIt>
ForwardIt word_shift_left(ForwardIt first,
                          ForwardIt last,
                          typename ForwardIt::difference_type n
)
{
    if (n <= 0) return last;
    if (n >= distance(first, last)) return first;
    ForwardIt mid = first;
    std::advance(mid, n);
    auto ret = std::copy(mid, last, first);
    std::fill(ret, last, 0);
    return ret;
}


// Shifts the range [first, right) to the left by n, filling the empty
// bits with 0
// NOT OPTIMIZED. Will be replaced with std::shift eventually.
template <class ForwardIt>
ForwardIt word_shift_right_dispatch(ForwardIt first,
                          ForwardIt last,
                          typename ForwardIt::difference_type n,
                          std::forward_iterator_tag
) {
        auto d = distance(first, last);
    if (n <= 0) return first;
    if (n >= d) return last;
    ForwardIt it = first;
    std::advance(it, d-n);
    std::rotate(first, it, last);
    it = first;
    std::advance(it, n);
    std::fill(first, it, 0); 
    return std::next(first, n);
}

template <class ForwardIt>
ForwardIt word_shift_right_dispatch(ForwardIt first,
                          ForwardIt last,
                          typename ForwardIt::difference_type n,
                          std::random_access_iterator_tag
) {
    auto d = distance(first, last);
    if (n <= 0) return first;
    if (n >= d) return last;
    ForwardIt it = first;
    std::advance(it, d-n);
    auto ret = std::copy_backward(first, it, last);
    std::fill(first, ret, 0); 
    return ret;
}

template <class ForwardIt>
ForwardIt word_shift_right(ForwardIt first,
                          ForwardIt last,
                          typename ForwardIt::difference_type n
)
{
    return word_shift_right_dispatch(
        first, 
        last,
        n,
        typename std::iterator_traits<ForwardIt>::iterator_category());
}

// returns a word consisting of all one bits 
constexpr auto _all_ones() {
    return -1;
} 

// returns a word consisting of all zero bits
constexpr auto _all_zeros() {
    return 0;
}

// checks that the passed iterator points to the first bit of a word
template <class It>
bool _is_aligned_lsb(bit_iterator<It> iter) {
    return iter.position() == 0;
}

// checks that maybe_end is one position past the last bit of base
template <class ForwardIt>
bool _is_one_past_last_bit(bit_iterator<ForwardIt> maybe_end, 
    ForwardIt base) {
    return maybe_end.position() == 0 && std::next(base) == maybe_end.base(); 
}

// checks that two bit iterators point to the same word
template <class It>
constexpr bool _in_same_word(bit_iterator<It> lhs, bit_iterator<It> rhs) {
    return lhs.base() == rhs.base();
}

// simple alias for right shift
template <class WordType>
WordType _shift_towards_lsb(WordType word, std::size_t n) {
    return word >> n; 
} 

// simple alias for left shift
template <class WordType>
WordType _shift_towards_msb(WordType word, std::size_t n) {
    return word << n;
}

/* Used to read partial/full words and pad any missing digits. Will not
 * read outside of the word pointed to by the first iterator (see case 4) 
 *
 * Case 0: 01011101
 *        L       F
 * Case 1: 01011101 -> padded with 0s -> 00001101
 *            L   F
 * Case 2: 01011101 -> padded with 1s -> 01011111 
 *        L    F 
 * Case 3: 01011101 -> padded with 0s -> 00011100
 *           L  F
 * Case 4: 01011101 11111111 -> treated as 01011101
 *           F           L                L  F
 *
 * Note: word is read from [first, last), meaning the element pointed
 * to by last is not included in the read. if first == last, behavior
 * is undefined
 */
template <class It>
typename bit_iterator<It>::word_type _padded_read(bit_iterator<It> first, 
    bit_iterator<It> last, const bit::bit_value bv) {

    using word_type = typename bit_iterator<It>::word_type;

    constexpr std::size_t num_digits = binary_digits<word_type>::value;
    const std::size_t first_position = first.position();
    const std::size_t last_position = last.position();
    const word_type read = *(first.base());
    constexpr word_type all_ones = _all_ones();

    word_type mask;

    if (_is_aligned_lsb(first)) {
        if (_in_same_word(first, last)) {
            // Case 1
            if (bv == bit0) {
                mask = _shift_towards_lsb(all_ones, num_digits - last_position);
                return read & mask;
            } else {
                mask = _shift_towards_msb(all_ones, last_position); 
                return read | mask;
            }
        } else {
            // Case 0
            return read;
        }
    } else {
        if (!_in_same_word(first, last)) {
            // Case 2
            if (bv == bit0) {
                mask = _shift_towards_msb(all_ones, first_position); 
                return read & mask;
            } else {
                mask = _shift_towards_lsb(all_ones, num_digits - first_position); 
                return read | mask;
            }
        } else {
            // Case 3
            if (bv == bit0) {
                mask = _shift_towards_msb(all_ones, first_position); 
                mask &= _shift_towards_lsb(all_ones, num_digits - last_position);
                return read & mask;
            } else {
                mask = _shift_towards_lsb(all_ones, num_digits - first_position);
                mask |= _shift_towards_msb(all_ones, last_position);
                return read | mask;
            }
        } 
    }
}
// -------------------------------------------------------------------------- //



// ========================================================================== //
} // namespace bit
#endif // _BIT_ALGORITHM_DETAILS_HPP_INCLUDED
// ========================================================================== //
