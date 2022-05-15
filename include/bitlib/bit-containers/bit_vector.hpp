// ================================= BIT_VECTOR =================================== //
// Project:     The Experimental Bit Algorithms Library
// \file        bit_vector.hpp
// Description: Implementation of bit_vector
// Creator:     Vincent Reverdy
// Contributor: Bryce Kille [2019]
// License:     BSD 3-Clause License
// ========================================================================== //
#ifndef _BIT_VECTOR_HPP_INCLUDED
#define _BIT_VECTOR_HPP_INCLUDED
// ========================================================================== //



// ================================ PREAMBLE ================================ //
// C++ standard library
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <type_traits>
// Project sources
#include "bitlib/bit-iterator/bit.hpp"
#include "bitlib/bit-algorithms/bit_algorithm.hpp"
// Third-party libraries
// Miscellaneous
namespace bit {
// ========================================================================== //



/* ****************************** BIT VECTOR ****************************** */
//! A bit-vector with a similar interface to std::vector<bool>
template<class WordType, class Allocator = std::allocator<WordType>>
class bit_vector {
    private:
        static constexpr size_t digits = binary_digits<WordType>::value;
        std::vector<WordType, Allocator> word_vector;
        size_t length_ = 0;

        // TODO are privates always inlined?
        // @brief Get the number of words needed to represet num_bits bits
        constexpr uint64_t word_count(unsigned int num_bits) {
            return ((num_bits + digits - 1) / digits);
        }

        // Iterator pair constructor specializations
        // Passing in iterator over bool
        template<class RandomAccessIt>
        typename std::enable_if<
            std::is_same<
                typename std::iterator_traits<RandomAccessIt>::value_type,
                bool
            >::value
        >::type
        constexpr range_constructor(
                RandomAccessIt first,
                RandomAccessIt last,
                const Allocator& alloc);

        // Passing in iterator over WordType constructs via whole words
        template<class RandomAccessIt>
        typename std::enable_if<
            std::is_same<
                typename std::iterator_traits<RandomAccessIt>::value_type,
                WordType
            >::value
        >::type
        constexpr range_constructor(
                RandomAccessIt first, 
                RandomAccessIt last,
                const Allocator& alloc);

    public:
        /*
         * Types and typedefs
         */
        using value_type = bit_value;
        using base_type = WordType;
        using allocator_type = Allocator;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using reference = bit_reference<WordType>;
        using const_reference = const reference;
        using pointer = bit_pointer<WordType>;
        using iterator = bit_iterator<typename std::vector<WordType>::iterator>;
        using const_iterator = bit_iterator<const typename std::vector<WordType>::const_iterator>;
        

        /*
         * Constructors, copies and moves...
         */
        constexpr bit_vector() noexcept(noexcept(Allocator()));
        constexpr explicit bit_vector(const Allocator& alloc) noexcept;
        constexpr bit_vector(
                size_type count, 
                value_type bit_val, 
                const Allocator& alloc=Allocator());
        constexpr explicit bit_vector(
                size_type count,
                const Allocator& alloc=Allocator());
        template<class RandomAccessIt>
        constexpr bit_vector(
                bit_iterator<RandomAccessIt> first, 
                bit_iterator<RandomAccessIt> last, 
                const Allocator& alloc=Allocator());
        template<class RandomAccessIt>
        constexpr bit_vector(
                RandomAccessIt first, 
                RandomAccessIt last, 
                const Allocator& alloc=Allocator());
        constexpr bit_vector(const bit_vector<WordType>& other);
        constexpr bit_vector(const bit_vector<WordType>& other, const Allocator& alloc);
        constexpr bit_vector(const bit_vector<WordType>&& other) noexcept;
        constexpr bit_vector(const bit_vector<WordType>&& other, const Allocator& alloc);
        constexpr bit_vector(std::initializer_list<bit_value> init, const Allocator& alloc=Allocator());
        constexpr bit_vector(std::initializer_list<bool> init, const Allocator& alloc=Allocator());
        constexpr bit_vector(std::initializer_list<WordType> init, const Allocator& alloc=Allocator());
        constexpr bit_vector(std::string_view s);

        constexpr ~bit_vector();


        /* 
         * Assignment
         */
        constexpr bit_vector& operator=(const bit_vector<WordType>& other);
        constexpr bit_vector& operator=(bit_vector<WordType>&& other) noexcept;


        /* 
         * Element Access
         */
        constexpr reference operator[](size_type pos);
        constexpr reference operator[](size_type pos) const;
        constexpr reference at(size_type pos);
        constexpr const_reference at(size_type pos) const;
        constexpr reference front();
        constexpr const_reference front() const;
        constexpr reference back();
        constexpr const_reference back() const;
        constexpr WordType* data() noexcept;
        constexpr const WordType* data() const noexcept;

        /* 
         * Iterators
         */
        constexpr iterator begin() noexcept;
        constexpr iterator end() noexcept;
        constexpr const_iterator begin() const noexcept;
        constexpr const_iterator end() const noexcept;
        constexpr const_iterator cbegin() const noexcept;
        constexpr const_iterator cend() const noexcept;
        

        /* 
         * Capacity
         */
        constexpr bool empty() const noexcept;
        constexpr size_type size() const noexcept;
        constexpr size_type max_size() const noexcept;
        constexpr void reserve(size_type new_cap);
        constexpr size_type capacity() const noexcept;
        constexpr void shrink_to_fit();


        /*
         * Modifiers
         */
        constexpr void clear() noexcept;
        constexpr iterator insert(const_iterator pos, const value_type& value);
        constexpr iterator insert(const_iterator pos, size_type count, const value_type& value);
        constexpr iterator insert(const_iterator pos, iterator first, iterator last);
        constexpr iterator erase(iterator pos);
        constexpr iterator erase(iterator first, iterator last);
        constexpr void push_back(const value_type& value);
        constexpr void pop_back();
        constexpr void resize(size_type count);
        constexpr void resize(size_type count, const value_type& value);


        /*
         * Helper functions
         */
        constexpr std::string debug_string(const_iterator first, const_iterator last);
        constexpr std::string debug_string();

        // TODO Make constexpr
        //friend std::ostream& operator<<(std::ostream& os, bit_vector bv) {
            //return os << bv.debug_string(bv.cbegin(), bv.cend());;
        //}
};
/* ************************************************************************** */



// ------------------------ BIT VECTOR: LIFECYCLE ------------------------- //
template<class WordType, class Allocator>
constexpr bit_vector<WordType, Allocator>::bit_vector() noexcept(noexcept(Allocator())) : word_vector{}, length_(0) {}

template<class WordType, class Allocator>
constexpr bit_vector<WordType, Allocator>::bit_vector(const Allocator& alloc) noexcept : word_vector(alloc), length_(0) {}

// TODO this should take a bit_reference, not a bit_value. The issue is that passing bit::bit1 
// does not currenlty work if you try to pass it as reference
template<class WordType, class Allocator>
constexpr bit_vector<WordType, Allocator>::bit_vector(size_type count, value_type bit_val, const Allocator& alloc) 
    : word_vector(word_count(count), static_cast<WordType>(bit_val == bit1 ? -1 : 0), alloc),
      length_(count)
      {}
      
template<class WordType, class Allocator>
constexpr bit_vector<WordType, Allocator>::bit_vector(size_type count, const Allocator& alloc) 
    : word_vector(word_count(count), alloc), length_(count) {}

//TODO needs to work for input iterators
template<class WordType, class Allocator>
template<class RandomAccessIt>
constexpr bit_vector<WordType, Allocator>::bit_vector(
        bit_iterator<RandomAccessIt> first, 
        bit_iterator<RandomAccessIt> last, 
        const Allocator& alloc) 
    : word_vector(distance(first, last), alloc), length_(distance(first, last)) {
    copy(first, last, this->begin());
}


//TODO please don't look at this yet ): 
template<class WordType, class Allocator>
template<class RandomAccessIt>
constexpr bit_vector<WordType, Allocator>::bit_vector(
        RandomAccessIt first, 
        RandomAccessIt last, 
        const Allocator& alloc) {
    bit_vector<WordType, Allocator>::range_constructor(
        first,
        last,
        alloc
    );
}

template<class WordType, class Allocator>
constexpr bit_vector<WordType, Allocator>::bit_vector(const bit_vector<WordType>& other) 
    : word_vector{other.word_vector.begin(), other.word_vector.end()}, length_(other.length_) {}

template<class WordType, class Allocator>
constexpr bit_vector<WordType, Allocator>::bit_vector(const bit_vector<WordType>&& other) noexcept
    : word_vector(std::move(other.word_vector)), length_(other.length_) {}

template<class WordType, class Allocator>
constexpr bit_vector<WordType, Allocator>::bit_vector(const bit_vector<WordType>&& other, const Allocator& alloc)
    : word_vector(std::move(other.word_vector), alloc), length_(other.length_) {}

// TODO maybe make template, or allow bool to be cast as bit
template<class WordType, class Allocator>
constexpr bit_vector<WordType, Allocator>::bit_vector(std::initializer_list<bit_value> init, const Allocator& alloc) 
    : word_vector(alloc), length_(0) {
    for (const_reference b : init) {
        this->push_back(b);
    }
}

// TODO use length of init to set vector length, then add at each index.
template<class WordType, class Allocator>
constexpr bit_vector<WordType, Allocator>::bit_vector(std::initializer_list<bool> init, const Allocator& alloc) 
    : word_vector(alloc), length_(0) {
    for (const bool& b : init) {
        this->push_back(static_cast<bit_value>(b));
    }
}

// TODO are these constructors executed in order?
template<class WordType, class Allocator>
constexpr bit_vector<WordType, Allocator>::bit_vector(
        std::initializer_list<WordType> init, 
        const Allocator& alloc) 
    : word_vector(init, alloc), length_(this->word_vector.size() * digits) {}

// Skip all characters that are not 0/1. This allows punctuation/spacing for byte/word boundaries
template<class WordType, class Allocator>
constexpr bit_vector<WordType, Allocator>::bit_vector(std::string_view s) {
    this->length_ = std::count(s.begin(), s.end(), '0') + std::count(s.begin(), s.end(), '1');
    this->word_vector = std::vector<WordType, Allocator>(this->length_);
    size_type i = 0;
    for (char c : s) {
        if (c == '0') {
            begin()[i++] = bit0;
        } else if (c == '1') {
            begin()[i++] = bit1;
        }
    }
} 

// Iterator pair constructor specializations
// Passing in iterator over bool
template<class WordType, class Allocator>
template<class RandomAccessIt>
typename std::enable_if<
    std::is_same<
        typename std::iterator_traits<RandomAccessIt>::value_type,
        bool
    >::value
>::type
constexpr bit_vector<WordType, Allocator>::range_constructor(
        RandomAccessIt first,
        RandomAccessIt last,
        const Allocator& alloc) {
    word_vector = std::vector<WordType, Allocator>(std::distance(first, last), alloc);
    length_ = std::distance(first, last);
    std::transform(
            first,
            last,
            begin(), 
            [](bool b) {return static_cast<bit_value>(b);}
    ); 
}

// Passing in iterator over WordType constructs via whole words
template<class WordType, class Allocator>
template<class RandomAccessIt>
typename std::enable_if<
    std::is_same<
        typename std::iterator_traits<RandomAccessIt>::value_type,
        WordType
    >::value
>::type
constexpr bit_vector<WordType, Allocator>::range_constructor(
        RandomAccessIt first, 
        RandomAccessIt last,
        const Allocator& alloc) {
    word_vector = std::vector<WordType, Allocator>(first, last, alloc);
    length_ = digits * std::distance(first, last);
}

template<class WordType, class Allocator>
constexpr bit_vector<WordType, Allocator>::~bit_vector() {
    length_ = 0;
}
// -------------------------------------------------------------------------- //



// ------------------------ BIT VECTOR: ASSIGNMENT ------------------------ //
template<class WordType, class Allocator>
constexpr bit_vector<WordType, Allocator>&
bit_vector<WordType, Allocator>::operator=(const bit_vector<WordType>& other) {
    length_ = other.length_;
    word_vector = other.word_vector;
    return *this;
}

template<class WordType, class Allocator>
constexpr bit_vector<WordType, Allocator>&
bit_vector<WordType, Allocator>::operator=(bit_vector<WordType>&& other) noexcept {
    length_ = other.length_;
    word_vector = std::move(other.word_vector);
    other.length_ = 0;
    return *this;
}
// -------------------------------------------------------------------------- //



// -------------------------- BIT VECTOR: ACCESS -------------------------- //
template<class WordType, class Allocator>
constexpr typename bit_vector<WordType, Allocator>::reference 
bit_vector<WordType, Allocator>::operator[](size_type pos) {return begin()[pos];}

// TODO is this supposed to be const_reference?
template<class WordType, class Allocator>
constexpr bit_vector<WordType, Allocator>::reference 
bit_vector<WordType, Allocator>::operator[](size_type pos) const {return begin()[pos];}

template<class WordType, class Allocator>
constexpr bit_vector<WordType, Allocator>::reference 
bit_vector<WordType, Allocator>::at(size_type pos) {
    if (pos < length_) {
        return begin()[pos];
    } else {
        throw std::out_of_range("Position is out of range");
    }
}

template<class WordType, class Allocator>
constexpr bit_vector<WordType, Allocator>::const_reference 
bit_vector<WordType, Allocator>::at(size_type pos) const {
    if (pos < length_) {
        return begin()[pos];
    } else {
        throw std::out_of_range("Position is out of range");
    }
}

template<class WordType, class Allocator>
constexpr bit_vector<WordType, Allocator>::reference 
bit_vector<WordType, Allocator>::front() {
    return begin()[0];
}

template<class WordType, class Allocator>
constexpr bit_vector<WordType, Allocator>::const_reference 
bit_vector<WordType, Allocator>::front() const {
    return begin()[0];
}

template<class WordType, class Allocator>
constexpr bit_vector<WordType, Allocator>::reference 
bit_vector<WordType, Allocator>::back() {
    return begin()[length_ - 1];
}

template<class WordType, class Allocator>
constexpr bit_vector<WordType, Allocator>::const_reference 
bit_vector<WordType, Allocator>::back() const {
    return begin()[length_ - 1];
}

template<class WordType, class Allocator>
constexpr WordType* 
bit_vector<WordType, Allocator>::data() noexcept {
    return length_ ? &(word_vector[0]) : 0;
}

template<class WordType, class Allocator>
constexpr const WordType* 
bit_vector<WordType, Allocator>::data() const noexcept {
    return word_vector.size() ? &(word_vector[0]) : 0;
}
// -------------------------------------------------------------------------- //



// ------------------------ BIT VECTOR: ITERATORS --------------------------- //
template<class WordType, class Allocator>
constexpr bit_vector<WordType, Allocator>::iterator 
bit_vector<WordType, Allocator>::begin() noexcept {
    return iterator(word_vector.begin());
}

template<class WordType, class Allocator>
constexpr bit_vector<WordType, Allocator>::iterator 
bit_vector<WordType, Allocator>::end() noexcept {
    return begin() + length_;
}

// TODO Need bit constructors for const iterators
template<class WordType, class Allocator>
constexpr bit_vector<WordType, Allocator>::const_iterator 
bit_vector<WordType, Allocator>::begin() const noexcept {
    return const_iterator(word_vector.begin());
}

template<class WordType, class Allocator>
constexpr bit_vector<WordType, Allocator>::const_iterator 
bit_vector<WordType, Allocator>::end() const noexcept {
    return const_iterator(word_vector.cbegin()) + length_;
}

template<class WordType, class Allocator>
constexpr bit_vector<WordType, Allocator>::const_iterator 
bit_vector<WordType, Allocator>::cbegin() const noexcept {
    return const_iterator(word_vector.begin());
}

template<class WordType, class Allocator>
constexpr bit_vector<WordType, Allocator>::const_iterator 
bit_vector<WordType, Allocator>::cend() const noexcept {
    return const_iterator(word_vector.cbegin()) + length_;
}
// -------------------------------------------------------------------------- //



// ------------------------ BIT VECTOR: CAPACITY ---------------------------- //
template<class WordType, class Allocator>
constexpr bool bit_vector<WordType, Allocator>::empty() const noexcept {
    return length_ == 0;
}

template<class WordType, class Allocator>
constexpr bit_vector<WordType, Allocator>::size_type 
bit_vector<WordType, Allocator>::size() const noexcept {
    return length_;
} 

template<class WordType, class Allocator>
constexpr bit_vector<WordType, Allocator>::size_type 
bit_vector<WordType, Allocator>::max_size() const noexcept {
    return word_vector.max_size() * digits;
}

template<class WordType, class Allocator>
constexpr void bit_vector<WordType, Allocator>::reserve(size_type new_cap) {
    word_vector.reserve(word_count(new_cap));
}

template<class WordType, class Allocator>
constexpr bit_vector<WordType, Allocator>::size_type
bit_vector<WordType, Allocator>::capacity() const noexcept {
    return word_vector.capacity() * digits;
}

template<class WordType, class Allocator>
constexpr void bit_vector<WordType, Allocator>::shrink_to_fit() {
    word_vector.shrink_to_fit();
}
// -------------------------------------------------------------------------- //



// ------------------------ BIT VECTOR: MODIFYING -------------------------- //
template<class WordType, class Allocator>
constexpr void bit_vector<WordType, Allocator>::clear() noexcept {
    word_vector.clear(); length_ = 0;
}

template<class WordType, class Allocator>
constexpr bit_vector<WordType, Allocator>::iterator 
bit_vector<WordType, Allocator>::insert(
        const_iterator pos, 
        const value_type& value) {
    const auto d = distance(cbegin(), pos);
    if (this->word_vector.size()*digits == length_) {
        word_vector.push_back(0U);
    }
    length_ += 1;
    shift_right(begin() + d, begin() + length_, 1);
    begin()[d] = value;
    return begin() + d;
}

template<class WordType, class Allocator>
constexpr bit_vector<WordType, Allocator>::iterator 
bit_vector<WordType, Allocator>::insert(
        const_iterator pos, 
        size_type count, 
        const bit_vector<WordType, Allocator>::value_type& value) {
    const auto d = distance(cbegin(), pos);
    if (count == 0) {
        return begin() + d;
    }
    const float bits_available = word_vector.size()*digits;
    const auto need_to_add = length_ + count > bits_available;
    if (need_to_add) {
        const auto words_to_add = word_count(length_ + count - bits_available);
        word_vector.resize(word_vector.size() + words_to_add);
    }
    length_ += count;
    shift_right(begin() + d, begin() + length_, count);
    fill(begin() + d, begin() + d + count, value);
    return begin() + d;
}

template<class WordType, class Allocator>
constexpr bit_vector<WordType, Allocator>::iterator 
bit_vector<WordType, Allocator>::insert(
        const_iterator pos, 
        iterator first, 
        iterator last) {
    const auto d = distance(cbegin(), pos);
    const size_t count = distance(first, last);    
    if (count == 0) {
        return begin() + d;
    }
    const float bits_available = word_vector.size()*digits;
    const auto need_to_add = length_ + count > bits_available;
    if (need_to_add) {
        const auto words_to_add = word_count(length_ + count - bits_available);
        word_vector.resize(word_vector.size() + words_to_add);
    }
    length_ += count;
    shift_right(begin() + d, begin() + length_, count);
    copy(first, last, begin() + d);
    return begin() + d;
}

template<class WordType, class Allocator>
constexpr bit_vector<WordType, Allocator>::iterator 
bit_vector<WordType, Allocator>::erase(iterator pos) {
    shift_left(pos, begin() + length_, 1);    
    length_ -= 1;
    if (length_ % digits == 0) {
        word_vector.pop_back();
    }
    return pos;
}

template<class WordType, class Allocator>
constexpr bit_vector<WordType, Allocator>::iterator 
bit_vector<WordType, Allocator>::erase(iterator first, iterator last) {
    const auto d = distance(begin(), first);
    auto count = distance(first, last);    
    if (count == 0) {
        return last;
    }
    shift_left(first, end(), count);
    length_ -= count;
    word_vector.resize(word_count(length_));
    return begin() + d;
}

template<class WordType, class Allocator>
constexpr void bit_vector<WordType, Allocator>::push_back(const value_type& value) {
    if (this->word_vector.size()*digits == length_) {
        word_vector.push_back(0U);
    }
    begin()[length_] = value;
    length_ += 1;
    return;
}

template<class WordType, class Allocator>
constexpr void bit_vector<WordType, Allocator>::pop_back() {
    length_ -= 1;
    if (length_ % digits == 0) {
        word_vector.pop_back();
    }
    return;
}

template<class WordType, class Allocator>
constexpr void bit_vector<WordType, Allocator>::resize(size_type count) {
    word_vector.resize(word_count(count));
    length_ = count;
    return;
}

template<class WordType, class Allocator>
constexpr void bit_vector<WordType, Allocator>::resize(size_type count, const value_type& value) {
    auto old_length = length_;
    word_vector.resize(word_count(count));
    length_ = count;
    if (length_ > old_length) {
        fill(begin() + length_, end(), value);
    }
    return;
}
// -------------------------------------------------------------------------- //



// ------------------------ BIT VECTOR: DEBUGGING -------------------------- //
template<class WordType, class Allocator>
constexpr std::string bit_vector<WordType, Allocator>::debug_string(const_iterator first, const_iterator last) {
    std::string ret = "";
    iterator mem = first;
    auto position = 0;
    for (iterator it = first; it != last; ++it) {
        if (position % digits == 0 && position != 0) {
            ret += " ";
        } else if (position % 8 == 0 && position != 0) {
            ret += '.';
        }
        ret += *it == bit1 ? '1' : '0';
        mem = it;
        ++position;
    }
    return ret;
}

template<class WordType, class Allocator>
constexpr std::string bit_vector<WordType, Allocator>::debug_string() {
    auto first = begin();
    auto last = end();
    std::string ret = "";
    iterator mem = first;
    auto position = 0;
    for (iterator it = first; it != last; ++it) {
        if (position % digits == 0 && position != 0) {
            ret += " ";
        } else if (position % 8 == 0 && position != 0) {
            ret += '.';
        }
        ret += *it == bit1 ? '1' : '0';
        mem = it;
        ++position;
    }
    return ret;
}
// -------------------------------------------------------------------------- //


// ========================================================================== //
} // namespace bit
#endif // _BIT_VECTOR_HPP_INCLUDED
// ========================================================================== //
