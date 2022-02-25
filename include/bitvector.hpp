#include <vector>
#include <cmath>
#include <string>
#include <iostream>

#include "bit.hpp"

namespace bit {

template<class WordType, class Allocator = std::allocator<WordType>>
class bit_vector {
    private:
        size_t length_;
        size_t digits = bit::binary_digits<WordType>::value;
        std::vector<WordType, Allocator> word_vector;
        bit::bit_iterator<decltype(std::begin(word_vector))> head;


    public:
        /*
         * Types and typedefs
         */
        using value_type = bit::bit_value;
        using allocator_type = Allocator;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using reference = bit::bit_reference<WordType>;
        using const_reference = const reference;
        using pointer = bit::bit_pointer<WordType>;
        using iterator = bit::bit_iterator<decltype(std::begin(word_vector))>;
        using const_iterator = const iterator;
        
        /*
         * Constructors, copies and moves...
         */
        constexpr bit_vector() : word_vector{}, head(word_vector.begin()), length_(0) {};
        constexpr bit_vector(size_t N) : word_vector(std::ceil(N / digits)), head(word_vector.begin()), length_(N) {};
        constexpr bit_vector(std::string_view s) : word_vector(std::ceil(s.length() / digits)), head(word_vector.begin()), length_(s.length()) {
            for (size_t i = 0; i < s.length(); ++i) {
                word_vector[i] = s[i] & static_cast<WordType>(1);
            }
        }; 

        constexpr ~bit_vector() {};

        bit_vector& operator=(const bit_vector& other) {
             length_ = other.length_;
             digits = other.digits;
             word_vector = other.word_vector;
             head = iterator(word_vector.begin());
             return *this;
        };

        bit_vector& operator=(bit_vector&& other) {
             length_ = other.length_;
             digits = other.digits;
             word_vector = std::move(other.word_vector);
             head = iterator(word_vector.begin());
             return *this;
        };


        /* 
         * Element Access
         */
        reference operator[](size_type pos) {return head[pos];};
        constexpr reference operator[](size_type pos) const {return head[pos];};
       
        constexpr reference at(size_type pos) {
            if (pos < length_ && pos > 0) {
                return head[pos];
            } else {
                throw std::out_of_range("Position is out of range");
            }
        }

        constexpr const_reference at(size_type pos) const {
            if (pos < length_ && pos > 0) {
                return head[pos];
            } else {
                throw std::out_of_range("Position is out of range");
            }
        }

        constexpr reference front() {return head[0];};
        constexpr const_reference front() const {return head[0];};
        constexpr reference back() {return head[length_ - 1];};
        constexpr const_reference back() const {return head[length_ - 1];};

        constexpr WordType* data() noexcept {return length_ ? &(word_vector[0]) : 0;};
        constexpr const WordType* data() const noexcept {return length_ ? &(word_vector[0]) : 0;};


        /* 
         * Iterators
         */
        constexpr iterator begin() noexcept {return head;}
        constexpr iterator end() noexcept {return head + length_;}
        constexpr const_iterator begin() const noexcept {return head;}
        constexpr const_iterator end() const noexcept {return head + length_;}
        constexpr const_iterator cbegin() const noexcept {return head;}
        constexpr const_iterator cend() const noexcept {return head + length_;}
        

        /* 
         * Capacity
         */
        constexpr bool empty() const noexcept {return length_ == 0;};
        constexpr size_type size() const noexcept {return length_;}; 
        constexpr size_type max_size() const noexcept {return word_vector.max_size() * digits;};
        constexpr void reserve(size_type new_cap) {word_vector.reserve(std::ceil(new_cap / digits));};
        constexpr size_type capacity() const noexcept {return word_vector.capacity() * digits;};
        constexpr void shrink_to_fit() {word_vector.shrink_to_fit();};

        /*
         * Modifiers
         */
        constexpr void clear() noexcept {word_vector.clear(); length_ = 0;};
        constexpr iterator insert(const_iterator pos, const value_type& value) {
                
        };
};

}

