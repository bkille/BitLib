// ================================== FIND ================================== //
// Project: The Experimental Bit Algorithms Library
// Name: find.hpp
// Description: bit_iterator overloads for std::find, std::find_if, std::find_if_not
// Creator: Vincent Reverdy
// Contributor(s): Collin Gress [2019] 
// License: BSD 3-Clause License
// ========================================================================== //
#ifndef _FIND_HPP_INCLUDED
#define _FIND_HPP_INCLUDED
// ========================================================================== //

// ============================== PREAMBLE ================================== //
// C++ standard library
// Project sources
// Third-party libraries
// Miscellaneous

namespace bit {

// Status: needs revisions
template <class InputIt>
constexpr bit_iterator<InputIt> find(bit_iterator<InputIt> first,
    bit_iterator<InputIt> last, bit::bit_value bv) {

    using word_type = typename bit_iterator<InputIt>::word_type;
    std::size_t word_type_digits = binary_digits<word_type>::value;

    bit_iterator<InputIt> cursor = first;
    std::size_t bits_scanned = 0;

    while (cursor != last) {
        std::size_t bits_to_read = std::min<size_t>(
            distance(cursor, last), word_type_digits);

        word_type cur = get_word(cursor, bits_to_read); 

        std::size_t num_trailing_complementary_bits = (bv == bit0) 
            ? _tzcnt(static_cast<word_type>(~cur))
            : _tzcnt(static_cast<word_type>(cur));

        if (num_trailing_complementary_bits < bits_to_read) {
            bits_scanned += num_trailing_complementary_bits;
            break;
        } else {
            bits_scanned += bits_to_read;
            std::advance(cursor, bits_to_read);
        }
    }

    return first + bits_scanned;
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
