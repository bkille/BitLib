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
#ifdef SIMDPP
#include <simdpp/simd.h>
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
// ========================================================================== //



// --------------------------- Shift Algorithms ----------------------------- //
template <class ForwardIt>
bit_iterator<ForwardIt> shift_right_dispatch(
        bit_iterator<ForwardIt> first,
        bit_iterator<ForwardIt> last,
        typename bit_iterator<ForwardIt>::difference_type n,
        std::forward_iterator_tag
)
{
    // Assertions
     _assert_range_viability(first, last); 

    // Types and constants
    using word_type = typename bit_iterator<ForwardIt>::word_type;
    using size_type = typename bit_iterator<ForwardIt>::size_type;
    constexpr size_type digits = binary_digits<word_type>::value;

    // Initialization
    size_type word_shifts = n / digits; 
    size_type remaining_bitshifts = n - digits*(word_shifts);
    const bool is_first_aligned = first.position() == 0;
    const bool is_last_aligned = last.position() == 0;
    auto d = distance(first, last);

    // Out of range cases
    if (n <= 0) return first;
    else if (n >= d) return last;

    // Single word case
    if (first.base() == last.base()) {
        *first.base() = _bitblend<word_type>(
                *first.base(),
                (
                    *first.base() & (
                        static_cast<word_type>(-1) << first.position()
                    )
                ) << n,
                first.position(),
                last.position() - first.position()
        );
        return bit_iterator<ForwardIt>(
                first.base(), 
                first.position() +  n
        );
    }

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
    ForwardIt it = STD_SHIFT_RIGHT(first.base(), 
                               std::next(last.base(), 
                                         !is_last_aligned
                                         ),
                               word_shifts
    );
    bit_iterator<ForwardIt> d_first(it, first.position());
    // Shift bit sequence to the msb 
    if (remaining_bitshifts) {
        word_type temp_1 = *it;
        word_type temp_2;
        *it = *it << remaining_bitshifts;
        it++;
        //TODO probably a way to do this with 1 temp or
        // at least no value swapping
        for (; it != std::next(last.base(), !is_last_aligned); ++it) {
            temp_2 = *it;
            *it = _shld<word_type>(*it, temp_1, remaining_bitshifts);
            temp_1 = temp_2; 
        }
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

template <class ForwardIt>
bit_iterator<ForwardIt> shift_left_dispatch(
        bit_iterator<ForwardIt> first,
        bit_iterator<ForwardIt> last,
        typename bit_iterator<ForwardIt>::difference_type n,
        std::forward_iterator_tag
)
{
    // Types and constants
    using word_type = typename bit_iterator<ForwardIt>::word_type;
    using size_type = typename bit_iterator<ForwardIt>::size_type;
    constexpr size_type digits = binary_digits<word_type>::value;

    // Initialization
    size_type word_shifts = n / digits; 
    size_type remaining_bitshifts = n - digits*(word_shifts);
    const bool is_first_aligned = first.position() == 0;
    const bool is_last_aligned = last.position() == 0;
    auto d = distance(first, last);

    // Multiple word case
    word_type first_value = *first.base();
    word_type last_value = !is_last_aligned ? *last.base() : 0;

    // Shift words to the left using std::shift 
    ForwardIt new_last_base = STD_SHIFT_LEFT(first.base(), 
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
        ForwardIt it = first.base();
        // _shrd all words except the last
        for (; std::next(it, is_last_aligned) != new_last_base; ++it) {
            *it = _shrd<word_type>(*it, *std::next(it), remaining_bitshifts);
            //latent_it = it;
        }
        // For the last word simpy right shift
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
    bit_iterator<ForwardIt> d_last = next(first, d-n);
    return d_last;
}

template <class RandomAccessIt>
bit_iterator<RandomAccessIt> shift_right_dispatch(
        bit_iterator<RandomAccessIt> first,
        bit_iterator<RandomAccessIt> last,
        typename bit_iterator<RandomAccessIt>::difference_type n,
        std::random_access_iterator_tag
) {
    // Types and constants
    using word_type = typename bit_iterator<RandomAccessIt>::word_type;
    using size_type = typename bit_iterator<RandomAccessIt>::size_type;
    constexpr size_type digits = binary_digits<word_type>::value;

    // Initialization
    size_type word_shifts = n / digits; 
    size_type remaining_bitshifts = n - digits*(word_shifts);
    const bool is_first_aligned = first.position() == 0;
    const bool is_last_aligned = last.position() == 0;

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
#ifdef SIMDPP
        // Align iterator
        const auto N = SIMDPP_FAST_INT64_SIZE;
        const auto N_native_words = (N*64)/digits;
        for (; std::distance(new_first_base, it)*digits >= N_native_words + 2 && !is_aligned(&(*(it - N_native_words + 1)), 64); --it) {
            *it = _shld<word_type>(*it, *(it - 1), remaining_bitshifts);
        }
        for (; std::distance(new_first_base, it) >= (unsigned int) N_native_words + 2; it -= N_native_words ) {
            using vec_type = simdpp::uint64<N>;
            auto it_rewind = it - N_native_words + 1;
            vec_type v = simdpp::load(&(*it_rewind));
            vec_type v_minus1 = simdpp::load_u(&(*(it_rewind-1)));
            vec_type ls = simdpp::shift_l(v, remaining_bitshifts);
            vec_type rs = simdpp::shift_r(v_minus1, digits - remaining_bitshifts);
            vec_type ret = simdpp::bit_or(ls, rs);
            simdpp::store(&(*(it_rewind)), ret);
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

template <class RandomAccessIt>
bit_iterator<RandomAccessIt> shift_left_dispatch(
        bit_iterator<RandomAccessIt> first,
        bit_iterator<RandomAccessIt> last,
        typename bit_iterator<RandomAccessIt>::difference_type n,
        std::random_access_iterator_tag
) {
    // Types and constants
    using word_type = typename bit_iterator<RandomAccessIt>::word_type;
    using size_type = typename bit_iterator<RandomAccessIt>::size_type;
    constexpr size_type digits = binary_digits<word_type>::value;

    // Initialization
    size_type word_shifts = n / digits; 
    size_type remaining_bitshifts = n - digits*(word_shifts);
    const bool is_first_aligned = first.position() == 0;
    const bool is_last_aligned = last.position() == 0;
    auto d = distance(first, last);

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

        // _shrd all words except the last until we reach alignment
#ifdef SIMDPP
        // TODO set alignment based off of instruction set used. 
        for (; std::next(it, is_last_aligned) != new_last_base && !is_aligned(&*it, 64); ++it) {
            *it = _shrd<word_type>(*it, *std::next(it), remaining_bitshifts);
        }
        // For the last word simply right shift
        const auto N = SIMDPP_FAST_INT64_SIZE;
        for (; std::distance(it, new_last_base)*digits >= (N+2)*64 ; it += N*(64/digits)) {
            using vec_type = simdpp::uint64<N>;
            vec_type v = simdpp::load(&(*(it)));
            vec_type v_plus1 = simdpp::load_u(&(*(it+1)));
            vec_type rs = simdpp::shift_r(v, remaining_bitshifts);
            vec_type ls = simdpp::shift_l(v_plus1, digits - remaining_bitshifts);
            vec_type ret = simdpp::bit_or(ls, rs);
            simdpp::store(&(*it), ret);
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


template <class ForwardIt>
bit_iterator<ForwardIt> shift_left(
        bit_iterator<ForwardIt> first,
        bit_iterator<ForwardIt> last,
        typename bit_iterator<ForwardIt>::difference_type n
) {
    // Assertions
     _assert_range_viability(first, last); 

    // Types and constants
    using word_type = typename bit_iterator<ForwardIt>::word_type;
    using size_type = typename bit_iterator<ForwardIt>::size_type;
    constexpr size_type digits = binary_digits<word_type>::value;

    // Initialization
    auto d = distance(first, last);
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
        return bit_iterator<ForwardIt>(
                first.base(), 
                first.position() + d - n
        );
    }
    else {
        return shift_left_dispatch(
                first, 
                last,
                n,
                typename std::iterator_traits<ForwardIt>::iterator_category()
        );
    }
}

template <class ForwardIt>
bit_iterator<ForwardIt> shift_right(
        bit_iterator<ForwardIt> first,
        bit_iterator<ForwardIt> last,
        typename bit_iterator<ForwardIt>::difference_type n
) {
    // Types and constants
    using word_type = typename bit_iterator<ForwardIt>::word_type;

    // Initialization
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
        return bit_iterator<ForwardIt>(
                first.base(), 
                first.position() +  n
        );
    }

    return shift_right_dispatch(
            first, 
            last,
            n,
            typename std::iterator_traits<ForwardIt>::iterator_category()
    );
}
// -------------------------------------------------------------------------- //



// ========================================================================== //
} // namespace bit
#endif // _SHIFT_HPP_INCLUDED
// ========================================================================== //
