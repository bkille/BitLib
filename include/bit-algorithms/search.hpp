// =============================== SEARCH =================================== //
// Project: The Experimental Bit Algorithms Library
// Name: search.hpp
// Description: bit_iterator overloads for std::search
// Creator: Vincent Reverdy
// Contributor(s): Vincent Reverdy [2019]
// License: BSD 3-Clause License
// ========================================================================== //
#ifndef _SEARCH_HPP_INCLUDED
#define _SEARCH_HPP_INCLUDED
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



// Status: to do
template <class ForwardIt1, class ForwardIt2>
constexpr bit_iterator<ForwardIt1> search(bit_iterator<ForwardIt1> first,
    bit_iterator<ForwardIt1> last, bit_iterator<ForwardIt2> s_first,
    bit_iterator<ForwardIt2> s_last) {
    // Assertions
     _assert_range_viability(first, last); 
     _assert_range_viability(s_first, s_last); 

    // Types and constants
    using word_type1 = typename bit_iterator<ForwardIt1>::word_type;
    using word_type2 = typename bit_iterator<ForwardIt2>::word_type;
    using size_type1 = typename bit_iterator<ForwardIt1>::size_type;
    using size_type2 = typename bit_iterator<ForwardIt2>::size_type;
    constexpr size_type1 digits1 = binary_digits<word_type1>::value;
    constexpr size_type2 digits2 = binary_digits<word_type2>::value;

    // Different word types ):
    if constexpr (digits1 != digits2) {
        return std::search(first, last, s_first, s_last);
    }

    // Same word types word types (:
    // QUESTION: If word_type1 and word_type2 are of the same size, does it matter
    // if I just treat all words as word_type1?
    
    using word_type = word_type1; 
    using size_type = size_type1; 
    constexpr size_type1 digits = digits1; 
    
    if (is_within<digits>(s_first, s_last)) {
        word_type mask = static_cast<word_type>(-1) 
                        >> (digits - distance(s_first, s_last));
        size_type m = distance(s_first, s_last); 
        word_type pattern = get_word(s_first, m) & mask;
        word_type pattern_popcnt = _popcnt(pattern);

        // We can keep a running _popcnt of the current word. We can
        // advance our iterator by 
        // abs(_popcnt(current_word) - _popcnt(pattern))

        while (!is_within<digits>(first, last)) {
            word_type current_word = get_word(first, m) & mask;
            if (current_word == pattern) {
                return first;
            } else {
                advance(
                    first, 
                    std::max(
                        1,
                        std::abs(static_cast<int>(
                                pattern_popcnt - _popcnt(current_word))
                        )
                    )
                );
            }
        }
    } else {
        int delta = 0;
        int k = 0;
        int current_k = 0;
        int m = distance(s_first, s_last);
        if (is_within(first, last, m+1))
            return last;
        auto pattern_k_it = s_first;
        auto pattern_k_it_end = pattern_k_it;
        auto best_k_it = pattern_k_it;
        // Preprocess pattern
        while (pattern_k_it_end != s_last) {
            // Initialize head value and iterators
            auto it = pattern_k_it;
            auto head_bv = *it;
            auto current_delta = 1;
            ++pattern_k_it_end;

            // Extend range to obtain maximal head-tailed substring
            ++it;
            while (it != s_last && *it != head_bv) {
                ++it;
                ++current_delta;
            }

            // If we have a new maximum, record it.
            if (current_delta > delta) {
                delta = current_delta;
                best_k_it = pattern_k_it;
                k = current_k; 
                pattern_k_it_end = it;
            } 

            // Try new value of k
            ++current_k;
            ++pattern_k_it;
        } 
        pattern_k_it = best_k_it;

        auto t_window_first = first;
        auto t_window_k = first + k;
        auto t_window_last = first + m;
        while (true) {
            // Phase 1: search from t_window_k_it...t_window_last
            // -------: If we mismatch on the ith try, shift window by
            // -------: min(i, k)
            // -------: Else, if no mismatches at all, move to Phase 2
            // Phase 2: search from t_window_k_first...t_window_k_it
            // -------: If we mismatch anywhere, advance our window by k and
            // -------: go to phase 1.
            
            
            // Phase 1
            auto mm_pair = mismatch(pattern_k_it, s_last, t_window_k);
            auto pattern_it = mm_pair.first;
            auto text_it = mm_pair.second;
            int  skip_val = distance(pattern_k_it, pattern_it); // Need to change for FwdIt

            // Phase 2
            if (pattern_it == s_last) {
                pattern_it = s_first;
                text_it = t_window_first;
                mm_pair = mismatch(s_first, pattern_k_it, t_window_first);
                pattern_it = mm_pair.first;
                text_it = mm_pair.second;
                if (pattern_it == pattern_k_it) {
                    return t_window_first;
                }
            }

            skip_val = std::min(std::max(skip_val, 1), delta);
            t_window_first += skip_val;
            t_window_k += skip_val;
            int i = 0;
            while (t_window_last != last && ++i <= skip_val) {
                ++t_window_last;
            }
            if (i < skip_val)
                break;
        }
    }
    return last;
}

// Status: to do
template <class ForwardIt1, class ForwardIt2>
constexpr bit_iterator<ForwardIt1> search_shift_or(bit_iterator<ForwardIt1> first,
    bit_iterator<ForwardIt1> last, bit_iterator<ForwardIt2> s_first,
    bit_iterator<ForwardIt2> s_last) {
    // Assertions
     _assert_range_viability(first, last); 
     _assert_range_viability(s_first, s_last); 

    // Types and constants
    using word_type1 = typename bit_iterator<ForwardIt1>::word_type;
    using word_type2 = typename bit_iterator<ForwardIt2>::word_type;
    using size_type1 = typename bit_iterator<ForwardIt1>::size_type;
    using size_type2 = typename bit_iterator<ForwardIt2>::size_type;
    constexpr size_type1 digits1 = binary_digits<word_type1>::value;
    constexpr size_type2 digits2 = binary_digits<word_type2>::value;

    // Different word types ):
    if constexpr (digits1 != digits2) {
        return std::search(first, last, s_first, s_last);
    }

    // Same word types word types (:
    // QUESTION: If word_type1 and word_type2 are of the same size, does it matter
    // if I just treat all words as word_type1?
    
    using word_type = word_type1; 
    using size_type = size_type1; 
    constexpr size_type digits = digits1; 
    
    auto bit_m = distance(s_first, s_last);
    if (is_within(first, last, bit_m-1)) {
        return last;
    }
    auto word_m = (bit_m / digits) + (bit_m % digits != 0);    
    std::vector<word_type> D(word_m, 0);


    auto D_first = bit_iterator(D.begin());
    auto D_last = D_first + bit_m;
    auto t_first = first;
    auto t_last = first + bit_m;
    word_type val_mask;
    auto shift_or_mask_f = [&val_mask] (const word_type s_word, const word_type D_word) {
                return static_cast<word_type>(D_word & (~(s_word ^ val_mask)));
    };

    while (t_first != first + bit_m) {
        shift_right(D_first, D_last, 1);
        *D_first = bit1;
        val_mask = *t_first ? -1 : 0;
        transform_word(s_first, s_last, D_first, D_first, shift_or_mask_f);
        ++t_first; ++t_last;
    } 
    if (D_first[bit_m - 1] == bit1) {
        return first;
    }
    auto ret = first + 1;

    do {
        shift_right(D_first, D_last, 1);
        *D_first = bit1;
        val_mask = *t_first ? -1 : 0;
        transform_word(s_first, s_last, D_first, D_first, shift_or_mask_f);
        if (D_first[bit_m - 1] == bit1) {
            return ret;
        }
        ++t_first; ++t_last; ++ret;
    } while (t_last != last);
    return last;
}

// Status: to do
template <class ExecutionPolicy, class ForwardIt1, class ForwardIt2>
bit_iterator<ForwardIt1> search(ExecutionPolicy&& policy, 
    bit_iterator<ForwardIt1> first, bit_iterator<ForwardIt1> last,
    bit_iterator<ForwardIt2> s_first, bit_iterator<ForwardIt2> s_last) {
    (policy, last, s_first, s_last);
    return first; 
}

// Status: on hold
template <class ForwardIt1, class ForwardIt2, class BinaryPredicate>
constexpr bit_iterator<ForwardIt1> search(bit_iterator<ForwardIt1> first,
    bit_iterator<ForwardIt1> last, bit_iterator<ForwardIt2> s_first,
    bit_iterator<ForwardIt2> s_last, BinaryPredicate p) {
    (last, s_first, s_last, p);
    return first;
}

// Status: on hold 
template <class ExecutionPolicy, class ForwardIt1, class ForwardIt2, 
         class BinaryPredicate> bit_iterator<ForwardIt1> search(
         ExecutionPolicy&& policy, bit_iterator<ForwardIt1> first,
         bit_iterator<ForwardIt1> last, bit_iterator<ForwardIt2> s_first,
         bit_iterator<ForwardIt2> s_last, BinaryPredicate p) {
    (policy, last, s_first, s_last, p);
    return first; 
}
         
// Status: to do
template <class ForwardIt, class Searcher>
constexpr bit_iterator<ForwardIt> search(bit_iterator<ForwardIt> first,
    bit_iterator<ForwardIt> last, const Searcher& searcher) {
    (last, searcher);
    return first;
}

// ========================================================================== //
} // namespace bit

#endif // _SEARCH_HPP_INCLUDED
// ========================================================================== //
