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
#include "bit-iterator/bit.hpp"
// Third-party libraries
#include "simdpp/simd.h"
#define is_aligned(POINTER, BYTE_COUNT) \
    (((uintptr_t)(const void *)(POINTER)) % (BYTE_COUNT) == 0)
// Miscellaneous

namespace bit {

template <class RandomAccessIt>
bit_iterator<RandomAccessIt> find_dispatch(
        bit_iterator<RandomAccessIt> first, 
        bit_iterator<RandomAccessIt> last,
        bit_value bv,
        std::random_access_iterator_tag
) {
    using word_type = typename bit_iterator<RandomAccessIt>::word_type;
    using size_type = typename bit_iterator<RandomAccessIt>::size_type;
    std::size_t digits = binary_digits<word_type>::value;

    // Initialization
    const bool is_first_aligned = first.position() == 0;
    const bool is_last_aligned = last.position() == 0;
    assert(is_first_aligned);

    auto it = first.base();
    const auto N = SIMDPP_FAST_INT64_SIZE;
    const auto N_native_words = (N*64)/digits;

    // Align the iterator to 64 bit boundary
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
    
    // Use SIMD operations on large chunks
    for (; std::distance(it, last.base()) >= (unsigned int) N_native_words + 2; it += N_native_words ) {
        using vec_type = simdpp::uint64<N>;
        vec_type v = simdpp::load(&(*it));
        if (bv == bit0) {
            simdpp::for_each(v, [](auto a) {return ~a;});
        }
        if (simdpp::test_bits_any(v)) {
            return find_dispatch(bit_iterator(it), last, bv, std::input_iterator_tag());
        }
    }

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

template <class InputIt>
bit_iterator<InputIt> find_dispatch(
        bit_iterator<InputIt> first, 
        bit_iterator<InputIt> last,
        bit_value bv,
        std::input_iterator_tag
) {
    using word_type = typename bit_iterator<InputIt>::word_type;
    using size_type = typename bit_iterator<InputIt>::size_type;

    // Initialization
    const bool is_first_aligned = first.position() == 0;
    const bool is_last_aligned = last.position() == 0;
    assert(is_first_aligned);

    auto it = first.base();
    while (it != last.base()) {
        if ((bv == bit1 && (*it == 0)) || (bv == bit0 && (*it ==static_cast<word_type>(-1)))) {
            ++it;
            continue;
        }

        size_type num_trailing_complementary_bits = (bv == bit0) 
            ? _tzcnt(static_cast<word_type>(~*it))
            : _tzcnt(static_cast<word_type>(*it));
        return bit_iterator(it, (size_type) num_trailing_complementary_bits);
    } 
    if (!is_last_aligned) {
        size_type num_trailing_complementary_bits = (bv == bit0) 
            ? _tzcnt(static_cast<word_type>(~*it))
            : _tzcnt(static_cast<word_type>(*it));
        return bit_iterator(it, (size_type) std::min(num_trailing_complementary_bits, last.position()));
    }

    return last;
}


template <class InputIt>
constexpr bit_iterator<InputIt> find(
        bit_iterator<InputIt> first,
        bit_iterator<InputIt> last, bit::bit_value bv
) {

    using word_type = typename bit_iterator<InputIt>::word_type;
    using size_type = typename bit_iterator<InputIt>::size_type;
    std::size_t digits = binary_digits<word_type>::value;

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

    return find_dispatch(
            first, 
            last, 
            bv, 
            typename std::iterator_traits<InputIt>::iterator_category()
    );
}

// Status: complete
template <class InputIt, class T>
constexpr bit_iterator<InputIt> find(bit_iterator<InputIt> first,
    bit_iterator<InputIt> last, const T& value) {
    return bit::find(first, last, value); 
}


// Status: to do
template <class ExecutionPolicy, class ForwardIt, class T>
bit_iterator<ForwardIt> find(ExecutionPolicy&& policy, 
    bit_iterator<ForwardIt> first, bit_iterator<ForwardIt> last, const T& value) {
    (policy, last, value);
    return first;
}

// Status: on hold
template <class InputIt, class UnaryPredicate>
constexpr bit_iterator<InputIt> find_if(bit_iterator<InputIt> first,
    bit_iterator<InputIt> last, UnaryPredicate p) {
    (last, p);
    return first;
}

// Status: on hold
template <class ExecutionPolicy, class ForwardIt, class UnaryPredicate>
bit_iterator<ForwardIt> find_if(ExecutionPolicy&& policy, 
    bit_iterator<ForwardIt> first, bit_iterator<ForwardIt> last,
    UnaryPredicate p) {
    (policy, last, p);
    return first;
}

// Status: on hold
template <class InputIt, class UnaryPredicate>
constexpr bit_iterator<InputIt> find_if_not(bit_iterator<InputIt> first,
    bit_iterator<InputIt> last, UnaryPredicate q) {
    (last, q);
    return first;
}

// Status: on hold
template <class ExecutionPolicy, class ForwardIt, class UnaryPredicate>
bit_iterator<ForwardIt> find_if_not(ExecutionPolicy&& policy,
    bit_iterator<ForwardIt> first, bit_iterator<ForwardIt> last,
    UnaryPredicate q) {
    (policy, last, q); 
    return first;
}

// ========================================================================== //
} // namespace bit

#endif // _FIND_HPP_INCLUDED
// ========================================================================== //
