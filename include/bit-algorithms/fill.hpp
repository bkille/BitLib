// ================================= FILL =================================== //
// Project:         The Experimental Bit Algorithms Library
// Name:            fill.hpp
// Description:     bit_iterator overloads for std::fill 
// Creator:         Vincent Reverdy
// Contributor(s):  Vincent Reverdy [2019]
//                  Bryce Kille [2019]
// License:         BSD 3-Clause License
// ========================================================================== //
#ifndef _FILL_HPP_INCLUDED
#define _FILL_HPP_INCLUDED
// ========================================================================== //



// ============================== PREAMBLE ================================== //
// C++ standard library
// Project sources
#include "bit_algorithm_details.hpp"
// Third-party libraries
// Miscellaneous
namespace bit {
// ========================================================================== //



// Status: needs revisions
template <class ForwardIt>
void fill(bit_iterator<ForwardIt> first, bit_iterator<ForwardIt> last, 
    bit::bit_value bv) {
    // Assertions
    _assert_range_viability(first, last);

    // Types and constants
    using word_type = typename bit_iterator<ForwardIt>::word_type;
    constexpr word_type digits = binary_digits<word_type>::value;

    // Initializations
    const word_type fill_word = bv == bit0 ? 0 : -1;

    if (is_within<digits>(first, last)) {
        write_word<word_type>(fill_word, first, distance(first, last));
    } else {
        auto it = first.base();
        if (first.position() != 0) {
            write_word<word_type>(fill_word, first, digits - first.position());
            ++it;
        }
        while (it != last.base()) {
            *it++ = fill_word;
        }
        if (last.position() != 0) {
            write_word<word_type>(fill_word, bit_iterator<word_type*>(&(*it)), last.position());
        }
    }



}

// Status: to do
template <class ExecutionPolicy, class ForwardIt>
void fill(ExecutionPolicy&& policy, bit_iterator<ForwardIt> first,
    bit_iterator<ForwardIt> last, const bit_value& bv) {
    (policy, first, last, bv);
}


// ========================================================================== //
} // namespace bit
#endif // _FILL_HPP_INCLUDED
// ========================================================================== //
