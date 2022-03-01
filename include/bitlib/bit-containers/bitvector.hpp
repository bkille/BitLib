#include <vector>
#include <cmath>
#include <string>
#include <iostream>
#include <algorithm>
#include <vector>

#include "bitlib/bit-iterator/bit.hpp"
#include "bitlib/bit-algorithms/bit_algorithm.hpp"

namespace bit {

template<class WordType, class Allocator = std::allocator<WordType>>
class bit_vector {
    private:
        size_t length_;
        size_t digits = binary_digits<WordType>::value;
        std::vector<WordType, Allocator> word_vector;
        bit_iterator<decltype(std::begin(word_vector))> head;


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
        constexpr bit_vector() : word_vector{}, length_(0) {};
        constexpr bit_vector(const bit_vector<WordType>& other)
            : word_vector{other.word_vector.begin(), other.word_vector.end()}, length_(other.length_) {};
        constexpr bit_vector(size_t N) : word_vector(std::ceil(float(N) / digits)), length_(N) {};
        constexpr bit_vector(std::string_view s) : word_vector(std::ceil(float(s.length()) / digits)), length_(s.length()) {
            for (size_t i = 0; i < s.length(); ++i) {
                begin()[i] = bit_value(static_cast<WordType>(s[i] & static_cast<WordType>(1)));
            }
        }; 

        constexpr ~bit_vector() {};

        bit_vector& operator=(const bit_vector& other) {
             length_ = other.length_;
             digits = other.digits;
             word_vector = other.word_vector;
             return *this;
        };

        bit_vector& operator=(bit_vector&& other) {
             length_ = other.length_;
             digits = other.digits;
             word_vector = std::move(other.word_vector);
             return *this;
        };


        /* 
         * Element Access
         */
        reference operator[](size_type pos) {return begin()[pos];};
        constexpr reference operator[](size_type pos) const {return begin()[pos];};
       
        constexpr reference at(size_type pos) {
            if (pos < length_ && pos >= 0) {
                return begin()[pos];
            } else {
                throw std::out_of_range("Position is out of range");
            }
        }

        constexpr const_reference at(size_type pos) const {
            if (pos < length_ && pos > 0) {
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
        constexpr const WordType* data() const noexcept {return length_ ? &(word_vector[0]) : 0;};

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
            if (this->capacity() <= length_ - 1) {
                word_vector.push_back(0U);
            }
            shift_right(begin() + d, begin() + length_, 1);
            begin()[d] = value;
            length_ += 1;
            return begin() + d;
        };
        constexpr iterator insert(const_iterator pos, size_type count, const value_type& value) {
            const auto d = distance(cbegin(), pos);
            while (capacity() <= (length_ > count ? length_ - count : 0)) {
                word_vector.push_back(0U);
            }
            shift_right(begin() + d, begin() + length_, count);
            fill(begin() + d, begin() + d + count, value);
            length_ += count;
            return begin() + d;
        };
        constexpr iterator insert(const_iterator pos, iterator first, iterator last) {
            const auto d = distance(cbegin(), pos);
            const size_t count = distance(first, last);    
            while (capacity() <= (length_ > count ? length_ - count : 0)) {
                word_vector.push_back(0U);
            }
            shift_right(begin() + d, begin() + length_, count);
            copy(first, last, begin() + d);
            length_ += count;
            return begin() + d;
        };
        constexpr iterator erase(iterator pos) {
            const auto d = distance(begin(), pos);
            shift_left(pos, begin() + length_, 1);    
            length_ -= 1;
            return pos;
        };
        constexpr iterator erase(iterator first, iterator last) {
            // TODO return correct iterator
            auto count = distance(first, last);    
            shift_left(first, first + length_, count);
            length_ -= count;
            return last;
        }
        constexpr void push_back(const value_type& value) {
            if (this->capacity() <= length_ - 1) {
                word_vector.push_back(0U);
            }
            begin()[length_] = value;
            length_ += 1;
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


}

