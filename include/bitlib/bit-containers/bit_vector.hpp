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
// Project sources
#include "bitlib/bit-iterator/bit.hpp"
#include "bitlib/bit-algorithms/bit_algorithm.hpp"
// Third-party libraries
// Miscellaneous
#define WORDS(bits, digits) \
    ((bits + digits - 1) / digits)
namespace bit {
// ========================================================================== //



// -------------------------------------------------------------------------- //
//! A bit-vector with a similar interface to std::vector<bool>
template<class WordType, class Allocator = std::allocator<WordType>>
class bit_vector {
    private:
        static constexpr size_t digits = binary_digits<WordType>::value;
        std::vector<WordType, Allocator> word_vector;
        size_t length_;

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
        constexpr bit_vector() noexcept(noexcept(Allocator())) : word_vector{}, length_(0) {};
        constexpr explicit bit_vector(const Allocator& alloc) noexcept : word_vector(alloc), length_(0) {};
        constexpr bit_vector(size_type count, const_reference bit_ref, const Allocator& alloc=Allocator()) 
            : word_vector(WORDS(count, digits), static_cast<WordType>(bit_ref == bit1 ? -1 : 0), alloc),
              length_(count)
              {};
              
        constexpr explicit bit_vector(size_type count, const Allocator& alloc=Allocator()) 
            : word_vector(WORDS(count,digits), alloc), length_(count) {};

        //TODO needs to work for input iterators
        template<class RandomAccessIt>
        constexpr bit_vector(RandomAccessIt first, RandomAccessIt last, const Allocator& alloc=Allocator()) 
                : word_vector(WORDS(distance(first, last), digits), alloc), length_(distance(first, last)) {
            copy(first, last, this->begin());
        }

        constexpr bit_vector(const bit_vector<WordType>& other)
                : word_vector{other.word_vector.begin(), other.word_vector.end()}, length_(other.length_) {};

        constexpr bit_vector(const bit_vector<WordType>&& other)
                : word_vector(std::move(other.word_vector)), length_(other.length_) {
            other.length = 0;     
        }

        constexpr bit_vector(const bit_vector<WordType>&& other, const Allocator& alloc)
                : word_vector(std::move(other.word_vector), alloc), length_(other.length_) {
            other.length = 0;     
        }

        // TODO maybe make template, or allow bool to be cast as bit
        constexpr bit_vector(std::initializer_list<bit_value> init, const Allocator& alloc=Allocator()) 
                : word_vector(alloc), length_(0) {
            for (const_reference b : init) {
                this->push_back(b);
            }
        }

        constexpr bit_vector(std::initializer_list<WordType> init, const Allocator& alloc=Allocator()) 
                : word_vector(init, alloc) {
            // Not sure if this would work as a member initializer
            length_ = this->word_vector.size() * digits;
        }

        // Skip all characters that are not 0/1. This allows punctuation/spacing for byte/word boundaries
        constexpr bit_vector(std::string_view s) {
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

        ~bit_vector() {};

        bit_vector& operator=(bit_vector& other) {
            length_ = other.length_;
            word_vector = other.word_vector;
            return *this;
        };

        bit_vector& operator=(bit_vector&& other) {
            length_ = other.length_;
            word_vector = std::move(other.word_vector);
            return *this;
        };


        /* 
         * Element Access
         */
        reference operator[](size_type pos) {return begin()[pos];};
        constexpr reference operator[](size_type pos) const {return begin()[pos];};
       
        constexpr reference at(size_type pos) {
            if (pos < length_) {
                return begin()[pos];
            } else {
                throw std::out_of_range("Position is out of range");
            }
        }

        constexpr const_reference at(size_type pos) const {
            if (pos < length_) {
                return begin()[pos];
            } else {
                throw std::out_of_range("Position is out of range");
            }
        }

        constexpr reference front() {return begin()[0];};
        constexpr const_reference front() const {return begin()[0];};
        constexpr reference back() {return begin()[length_ - 1];};
        constexpr const_reference back() const {return begin()[length_ - 1];};
        constexpr WordType* data() noexcept {return length_ ? &(word_vector[0]) : 0;};
        constexpr const WordType* data() const noexcept {return word_vector.size() ? &(word_vector[0]) : 0;};

        /* 
         * Iterators
         */
        constexpr iterator begin() noexcept {return iterator(word_vector.begin());}
        constexpr iterator end() noexcept {return begin() + length_;}
        // TODO Need bit constructors for const iterators
        constexpr const_iterator begin() const noexcept {return const_iterator(word_vector.begin());}
        constexpr const_iterator end() const noexcept {return const_iterator(word_vector.cbegin()) + length_;}
        constexpr const_iterator cbegin() const noexcept {return const_iterator(word_vector.begin());}
        constexpr const_iterator cend() const noexcept {return const_iterator(word_vector.cbegin()) + length_;}
        

        /* 
         * Capacity
         */
        constexpr bool empty() const noexcept {return length_ == 0;};
        constexpr size_type size() const noexcept {return length_;}; 
        constexpr size_type max_size() const noexcept {return word_vector.max_size() * digits;};
        constexpr void reserve(size_type new_cap) {word_vector.reserve(std::ceil(float(new_cap) / digits));};
        constexpr size_type capacity() const noexcept {return word_vector.capacity() * digits;};
        constexpr void shrink_to_fit() {word_vector.shrink_to_fit();};

        /*
         * Modifiers
         */
        constexpr void clear() noexcept {word_vector.clear(); length_ = 0;};
        constexpr iterator insert(const_iterator pos, const value_type& value) {
            const auto d = distance(cbegin(), pos);
            if (this->word_vector.size()*digits == length_) {
                word_vector.push_back(0U);
            }
            length_ += 1;
            shift_right(begin() + d, begin() + length_, 1);
            begin()[d] = value;
            return begin() + d;
        };
        constexpr iterator insert(const_iterator pos, size_type count, const value_type& value) {
            const auto d = distance(cbegin(), pos);
            if (count == 0) {
                return begin() + d;
            }
            const float bits_available = word_vector.size()*digits;
            const auto need_to_add = length_ + count > bits_available;
            if (need_to_add) {
                const auto words_to_add = std::ceil((length_ + count - bits_available) / digits);
                word_vector.resize(word_vector.size() + words_to_add);
            }
            length_ += count;
            shift_right(begin() + d, begin() + length_, count);
            fill(begin() + d, begin() + d + count, value);
            return begin() + d;
        };
        constexpr iterator insert(const_iterator pos, iterator first, iterator last) {
            const auto d = distance(cbegin(), pos);
            const size_t count = distance(first, last);    
            if (count == 0) {
                return begin() + d;
            }
            const float bits_available = word_vector.size()*digits;
            const auto need_to_add = length_ + count > bits_available;
            if (need_to_add) {
                const auto words_to_add = std::ceil((length_ + count - bits_available) / digits);
                word_vector.resize(word_vector.size() + words_to_add);
            }
            length_ += count;
            shift_right(begin() + d, begin() + length_, count);
            copy(first, last, begin() + d);
            return begin() + d;
        };
        constexpr iterator erase(iterator pos) {
            shift_left(pos, begin() + length_, 1);    
            length_ -= 1;
            if (length_ % digits == 0) {
                word_vector.pop_back();
            }
            return pos;
        };
        constexpr iterator erase(iterator first, iterator last) {
            // TODO return correct iterator
            const auto d = distance(begin(), first);
            auto count = distance(first, last);    
            if (count == 0) {
                return last;
            }
            shift_left(first, end(), count);
            length_ -= count;
            word_vector.resize(std::ceil(float(length_) / digits));
            return begin() + d;
        }
        constexpr void push_back(const value_type& value) {
            if (this->word_vector.size()*digits == length_) {
                word_vector.push_back(0U);
            }
            begin()[length_] = value;
            length_ += 1;
            return;
        };
        constexpr void pop_back() {
            length_ -= 1;
            if (length_ % digits == 0) {
                word_vector.pop_back();
            }
            return;
        };
        constexpr void resize(size_type count) {
            word_vector.resize(std::ceil(float(count) / digits));
            length_ = count;
            return;
        };
        constexpr void resize(size_type count, const value_type& value) {
            auto old_length = length_;
            word_vector.resize(std::ceil(float(count) / digits));
            length_ = count;
            if (length_ > old_length) {
                fill(begin() + length_, end(), value);
            }
            return;
        };


        /*
         * Helper functions
         */

        std::string debug_string(const_iterator first, const_iterator last) {
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
        };
        std::string debug_string() {
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
        };

        // TODO Make constexpr
        //friend std::ostream& operator<<(std::ostream& os, bit_vector bv) {
            //return os << bv.debug_string(bv.cbegin(), bv.cend());;
        //};
};
// -------------------------------------------------------------------------- //



// ========================================================================== //
} // namespace bit
#endif // _BIT_VECTOR_HPP_INCLUDED
// ========================================================================== //
