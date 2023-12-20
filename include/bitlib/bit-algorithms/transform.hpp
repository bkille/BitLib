// =============================== TRANSFORM =============================== //
// Project: The Experimental Bit Algorithms Library
// Name: transform.hpp
// Description: bit_iterator overloads for std::transform
// Creator: Vincent Reverdy
// Contributor(s):
// License: BSD 3-Clause License
// ========================================================================== //
#ifndef _TRANSFORM_HPP_INCLUDED
#define _TRANSFORM_HPP_INCLUDED
// ========================================================================== //

// ============================== PREAMBLE ================================== //
// C++ standard library
#include <functional>
#include <type_traits>
// Project sources
#include "bitlib/bit-iterator/bit.hpp"
// Third-party libraries
// Miscellaneous

namespace bit {
// ========================================================================== //



//template <class RandomAccessIt, class WordType>
//constexpr bit_iterator<RandomAccessIt> transform(
        //bit_iterator<RandomAccessIt> first,
        //bit_iterator<RandomAccessIt> last,
        //bit_iterator<RandomAccessIt> d_first,
        //std::bit_or<WordType>) {

    //return d_first;
//}
template <class RandomAccessIt, class UnaryOperation>
constexpr bit_iterator<RandomAccessIt> transform(
        bit_iterator<RandomAccessIt> first,
        bit_iterator<RandomAccessIt> last,
        bit_iterator<RandomAccessIt> d_first,
        UnaryOperation unary_op) {
    using word_type = typename bit_iterator<RandomAccessIt>::word_type;
    using size_type = typename bit_iterator<RandomAccessIt>::size_type;
    constexpr size_type digits = binary_digits<word_type>::value;

    // Assertions
    _assert_range_viability(first, last);
    if (first == last) return d_first;


    // Initialization
    const bool is_d_first_aligned = d_first.position() == 0;
    size_type total_bits_to_op = distance(first, last);
    size_type remaining_bits_to_op = total_bits_to_op;
    auto it = d_first.base();

    // d_first is not aligned. Copy partial word to align it
    if (!is_d_first_aligned) {
        size_type partial_bits_to_op = ::std::min(
                remaining_bits_to_op,
                digits - d_first.position()
                );
        *it = _bitblend(
                *it,
                unary_op(
                    static_cast<word_type>(
                      get_word<word_type>(first, partial_bits_to_op)
                        << static_cast<word_type>(d_first.position())
                    )
                ),
                static_cast<word_type>(d_first.position()),
                static_cast<word_type>(partial_bits_to_op));
        remaining_bits_to_op -= partial_bits_to_op;
        advance(first, partial_bits_to_op);
        it++;
    }
    auto firstIt = first.base();
    if (remaining_bits_to_op > 0) {
        const bool is_first_aligned = first.position() == 0;
        //size_type words_to_op = ::std::ceil(remaining_bits_to_op / static_cast<float>(digits));
        // d_first will be aligned at this point
        if (is_first_aligned && remaining_bits_to_op > digits) {
            auto N = ::std::distance(firstIt, last.base());
#ifdef BITLIB_HWY
            if constexpr (std::is_same_v<UnaryOperation, std::bit_not<word_type>>)
            {
                // Align to 64 bit boundary
                for (; firstIt != last.base() && !is_aligned(&*firstIt, 64); firstIt++, it++) {
                    *it = unary_op(*firstIt);
                }

                bool out_is_aligned = is_aligned(&*it, 64);

                constexpr hn::ScalableTag<word_type> d;
                for (; std::distance(firstIt, last.base()) >= hn::Lanes(d); firstIt += hn::Lanes(d), it += hn::Lanes(d))
                {
                    const auto v = hn::Not(hn::Load(d, &*firstIt));
                    if (out_is_aligned)
                    {
                        hn::Store(v, d, &*it);
                    } else {
                        hn::StoreU(v, d, &*it);
                    }
                }
            }
#endif
            size_t std_dist = ::std::distance(firstIt, last.base());
            it = std::transform(firstIt, last.base(), it, unary_op);
            firstIt += std_dist;
            first = bit_iterator<RandomAccessIt>(firstIt);
            remaining_bits_to_op -= digits * N;
        } else {
            while (remaining_bits_to_op >= digits) {
                *it = unary_op(get_word<word_type>(first, digits));
                remaining_bits_to_op -= digits;
                it++;
                advance(first, digits);
            }
        }
        if (remaining_bits_to_op > 0) {
            *it = _bitblend(
                    *it,
                    unary_op(get_word<word_type>(first, remaining_bits_to_op)),
                    static_cast<word_type>(
                        (static_cast<word_type>(1) << remaining_bits_to_op) - 1)
            );
        }
    }
    return d_first + total_bits_to_op;
}

template <class RandomAccessIt, class BinaryOperation>
constexpr bit_iterator<RandomAccessIt> transform(
        bit_iterator<RandomAccessIt> first1,
        bit_iterator<RandomAccessIt> last1,
        bit_iterator<RandomAccessIt> first2,
        bit_iterator<RandomAccessIt> d_first,
        BinaryOperation binary_op) {
    using word_type = typename bit_iterator<RandomAccessIt>::word_type;
    using size_type = typename bit_iterator<RandomAccessIt>::size_type;
    constexpr size_type digits = binary_digits<word_type>::value;

    // Assertions
    _assert_range_viability(first1, last1);
    if (first1 == last1) return d_first;


    // Initialization
    const bool is_d_first_aligned = d_first.position() == 0;
    size_type total_bits_to_op = distance(first1, last1);
    size_type remaining_bits_to_op = total_bits_to_op;
    auto it = d_first.base();

    // d_first is not aligned. Copy partial word to align it
    if (!is_d_first_aligned) {
        size_type partial_bits_to_op = ::std::min(
                remaining_bits_to_op,
                digits - d_first.position()
                );
        *it = _bitblend(
                *it,
                binary_op(
                    static_cast<word_type>(
                      get_word<word_type>(first1, partial_bits_to_op)
                        << static_cast<word_type>(d_first.position())
                    ),
                    static_cast<word_type>(
                      get_word<word_type>(first2, partial_bits_to_op)
                        << static_cast<word_type>(d_first.position())
                    )
                ),
                static_cast<word_type>(d_first.position()),
                static_cast<word_type>(partial_bits_to_op));
        remaining_bits_to_op -= partial_bits_to_op;
        advance(first1, partial_bits_to_op);
        advance(first2, partial_bits_to_op);
        it++;
    }
    if (remaining_bits_to_op > 0) {
        const bool is_first1_aligned = first1.position() == 0;
        const bool is_first2_aligned = first2.position() == 0;
        //size_type words_to_op = ::std::ceil(remaining_bits_to_op / static_cast<float>(digits));
        // d_first will be aligned at this point
        if (is_first1_aligned && is_first2_aligned && remaining_bits_to_op > digits) {
            auto N = ::std::distance(first1.base(), last1.base());
            it = std::transform(first1.base(), last1.base(), first2.base(), it, binary_op);
            first1 += digits * N;
            first2 += digits * N;
            remaining_bits_to_op -= digits * N;
        } else {
            while (remaining_bits_to_op >= digits) {
                *it = binary_op(
                        get_word<word_type>(first1, digits),
                        get_word<word_type>(first2, digits));
                remaining_bits_to_op -= digits;
                it++;
                advance(first1, digits);
                advance(first2, digits);
            }
        }
        if (remaining_bits_to_op > 0) {
            *it = _bitblend(
                    *it,
                    binary_op(
                        get_word<word_type>(first1, remaining_bits_to_op),
                        get_word<word_type>(first2, remaining_bits_to_op)
                    ),
                    static_cast<word_type>(
                        (static_cast<word_type>(1) << remaining_bits_to_op) - 1)
            );
        }
    }
    return d_first + total_bits_to_op;
}

//template <class RandomAccessIt1, class RandomAccessIt2, class RandomAccessIt3, class BinaryOperation>
//constexpr bit_iterator<RandomAccessIt> transform_word(bit_iterator<RandomAccessIt1> first,
    //bit_iterator<RandomAccessIt1> last, bit_iterator<RandomAccessIt2> first2,
    //bit_iterator<RandomAccessIt> d_first, BinaryOperation binary_op) {
    //// Assertions
     //_assert_range_viability(first, last);

    //// Types and constants
    //using word_type1    = typename bit_iterator<RandomAccessIt1>::word_type;
    //using word_type2    = typename bit_iterator<RandomAccessIt2>::word_type;
    //using word_type_out = typename bit_iterator<RandomAccessIt>::word_type;
    //using size_type1    = typename bit_iterator<RandomAccessIt1>::size_type;
    //using size_type2    = typename bit_iterator<RandomAccessIt2>::size_type;
    //using size_type_out = typename bit_iterator<RandomAccessIt>::size_type;
    //constexpr size_type1 digits1     = binary_digits<word_type1>::value;
    //constexpr size_type2 digits2     = binary_digits<word_type2>::value;
    //constexpr size_type_out digits_out  = binary_digits<word_type_out>::value;

    //// This is just for now. Perhaps later we can expand to different word sizes
    //assert(digits1 == digits2);
    //assert(digits2 == digits_out);
    //using word_type = word_type1;
    //using size_type = size_type1;
    //constexpr size_type digits = digits1;

    //if (is_within<digits>(first, last)) {
        //word_type d = distance(first, last);
        //write_word(
            //binary_op(
                //get_word(first, d),
                //get_word(first2, d)
            //),
            //d_first,
            //d
        //);
        //return next(d_first, d);
    //}

    //RandomAccessIt1 it1    = first.base();
    //if (first.position() != 0) {
        //word_type d = digits - first.position();
        //write_word(
            //binary_op(
                //static_cast<word_type>(*first.base() >> first.position()),
                //get_word(first2, d)
            //),
            //d_first,
            //d
        //);
        //++it1;
        //advance(first2, d);
        //advance(d_first, d);
    //}

    //while (it1 != last1.base()) {
        //write_word(
            //binary_op(
                //*it1++,
                //get_word<word_type>(first2)
            //),
            //d_first,
            //(word_type)digits
        //);
        //advance(d_first, digits);
        //advance(first2, digits);
    //}

    //if (last1.position() != 0) {
        //write_word(
            //binary_op(
                //*it1,
                //get_word(first2, last1.position())
            //),
            //d_first,
            //static_cast<word_type>(last1.position())
        //);
        //advance(d_first, last1.position());
    //}
    //return d_first;
//}


// ========================================================================== //
} // namespace bit

#endif // _TRANSFORM_HPP_INCLUDED
// ========================================================================== //
