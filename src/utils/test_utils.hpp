// ============================= TEST_UTILS ================================ //
// Project:         The Experimental Bit Algorithms Library
// Name:            test_utils.hpp
// Description:     General utilities for testing
// Creator:         Bryce Kille
// Contributor(s):  Bryce Kille [2019], 
//                  Collin Gress [2019]
// License: BSD 3-Clause License
// ========================================================================== //
#ifndef _TEST_UTILS_HPP_INCLUDED
#define _TEST_UTILS_HPP_INCLUDED
// ========================================================================== //


// ================================ PREAMBLE ================================ //
// C++ standard library
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <list>
#include <forward_list>
// Project sources
#include "bitlib/bitlib.hpp"
// Third-party libraries
// Miscellaneous


// =============================== DEBUGGING ================================ //

// ================================ TESTING ================================- //

// Comparator for bit_iterator to other iterators
constexpr auto comparator = [](auto b1, auto b2){
    return static_cast<bool>(b1) == static_cast<bool>(b2);
};

// Helper struct for rebind_container
template <class Container, class NewType> struct rebind_container;

// Helper struct for rebind_container
template <class T, 
          class Alloc, 
          template <class, class, class...> class Container, 
          class NewType, 
          class... Parameters
         >
struct rebind_container<Container<T, Alloc, Parameters...>, NewType>
{
     typedef Container<NewType, typename Alloc::template rebind<NewType>::other > type;
};


// Takes in Container<T> and returns Container<bool> of the same underlying bits
template <class Container, typename T = typename Container::value_type>
auto bitcont_to_boolcont(const Container bitcont){
    auto bfirst = bit::bit_iterator<decltype(std::begin(bitcont))>(std::begin(bitcont));
    auto blast = bit::bit_iterator<decltype(std::end(bitcont))>(std::end(bitcont));
    typename rebind_container<Container, bool>::type c(std::distance(bfirst, 
                                                                     blast)
    );
    auto benchmark_it = std::begin(c);
    auto it = bfirst;
    for (; it != blast; ++it, ++benchmark_it) {
        *benchmark_it = static_cast<bool>(*it);
    }
    return c;
}

// Produces container of random numbers from min to max
template <class Container, typename T = typename Container::value_type>
Container make_random_container(
    std::size_t size, 
    T min = std::numeric_limits<T>::min(), 
    T max = std::numeric_limits<T>::max(), 
    const T& seed = T()
)
{
    Container c(size);
    std::random_device device;
    std::mt19937 engine(seed == T() ? device() : seed);
    std::uniform_int_distribution<std::uintmax_t> distribution(min, max);
    auto it = std::begin(c); 
    for (std::size_t i = 0; i < size; ++i) {
        *it = distribution(engine);
        ++it;
    }
    return c;
}


inline unsigned long long generate_random_number(size_t min, size_t max) {
    // First create an instance of an engine.
    std::random_device rnd_device;
    // Specify the engine and distribution.
    std::mt19937 mersenne_engine {rnd_device()};  // Generates random integers
    std::uniform_int_distribution<unsigned long long> dist {min, max};
    
    return dist(mersenne_engine);
}


template <typename WordType>
std::vector<WordType> get_random_vec(
        unsigned long long int size,
        WordType min = std::numeric_limits<WordType>::min(), 
        WordType max = std::numeric_limits<WordType>::max()
) {
    // First create an instance of an engine.
    std::random_device rnd_device;
    // Specify the engine and distribution.
    std::mt19937 mersenne_engine {rnd_device()};  // Generates random integers
    std::uniform_int_distribution<WordType> dist {min, max};
    
    auto gen = [&dist, &mersenne_engine](){
                   return dist(mersenne_engine);
   };
    std::vector<WordType> vec(size);
    generate(begin(vec), end(vec), gen);
    return vec;
}

template <typename WordType>
bit::bit_vector<WordType> get_random_bvec(
        unsigned long long int size
) {
    constexpr auto digits = bit::binary_digits<WordType>::value;
    auto words = (size + digits - 1) / digits;
    auto word_vec = get_random_vec(words);
    bit::bit_vector<WordType> ret(word_vec.begin(), word_vec.end());
    ret.resize(size);
    return ret;
}

template <typename WordType>
std::vector<bool> boolvec_from_bitvec(bit::bit_vector<WordType> bv) {
    std::vector<bool> ret_vec{};
    for (bit::bit_value value : bv) {
       ret_vec.push_back(value == bit::bit1 ? true : false); 
    }
    return ret_vec;
}

// ========================================================================== //
#endif // _TEST_UTILS_HPP_INCLUDED
// ========================================================================== //
