// =============================== FIXTURES ================================= //
// Project:         The Experimental Bit Algorithms Library
// Description:     Fixtures for testing 
// Creator:         Bryce Kille [2019]
// License:         BSD 3-Clause License
// ========================================================================== //
#ifndef _FIXTURES_HPP_INCLUDED
#define _FIXTURES_HPP_INCLUDED
// ========================================================================== //


// ============================== PREAMBLE ================================== //
// C++ standard library
#include <vector>
#include <list>
#include <forward_list>
#include <set>
#include <random>
#include <limits>
#include <cstring>
// Project sources
#include "bit-iterator/bit_iterator.hpp"
#include "bit-containers/bit-containers.hpp"
// Third-party libraries
#include "gtest/gtest.h"
// Miscellaneous
// ========================================================================== //

// Comparator for bit_iterator to other iterators
constexpr auto comparator = [](auto b1, auto b2){
    return static_cast<bool>(b1) == static_cast<bool>(b2);
};

using BaseTypes = ::testing::Types<unsigned char, unsigned short, unsigned int, unsigned long long>;
//using BaseTypes = ::testing::Types<unsigned char>;

inline unsigned long long generate_random_number(size_t min, size_t max) {
    // First create an instance of an engine.
    std::random_device rnd_device;
    // Specify the engine and distribution.
    std::mt19937 mersenne_engine {rnd_device()};  // Generates random integers
    std::uniform_int_distribution<unsigned long long> dist {min, max};
    
    return dist(mersenne_engine);
}

template <typename WordType>
std::vector<WordType> get_random_vec(unsigned long long int size) {
    // First create an instance of an engine.
    std::random_device rnd_device;
    // Specify the engine and distribution.
    std::mt19937 mersenne_engine {rnd_device()};  // Generates random integers
    std::uniform_int_distribution<WordType> dist {std::numeric_limits<WordType>::min(), std::numeric_limits<WordType>::max()};
    
    auto gen = [&dist, &mersenne_engine](){
                   return dist(mersenne_engine);
   };
    std::vector<WordType> vec(size);
    generate(begin(vec), end(vec), gen);
    return vec;
}

template <typename WordType>
std::vector<bool> boolvec_from_bitvec(bit::bit_vector<WordType> bv) {
    std::vector<bool> ret_vec{};
    for (auto value : bv) {
       ret_vec.push_back(value == bit::bit1 ? true : false); 
    }
    return ret_vec;
}


template<typename WordType>
class VectorTest : public testing::Test {
    protected:

    using base_type = WordType;
    bit::bit_vector<WordType> empty_vec;
    bit::bit_vector<WordType> v2_ {256};
    bit::bit_vector<WordType> v3_ {"010111111"};

    std::vector<bit::bit_vector<WordType>> random_bitvecs;
    std::vector<std::vector<bool>> random_boolvecs;
    std::vector<WordType> random_vec;
    size_t word_size = 4;
    size_t bit_size = word_size*bit::binary_digits<WordType>::value;

    void SetUp() override {
        empty_vec = bit::bit_vector<WordType>(); 
        random_vec = get_random_vec<WordType>(word_size);
        for (size_t cont_size = 1; cont_size < bit_size; ++cont_size) {
            auto bitvec = bit::bit_vector<WordType>(bit_size);
            std::memcpy(&(*bitvec.begin().base()), &(random_vec[0]), word_size);
            bitvec.resize(cont_size);

            auto boolvec = boolvec_from_bitvec(bitvec);
            random_bitvecs.push_back(bitvec);
            random_boolvecs.push_back(boolvec);
        }
    }
};
TYPED_TEST_SUITE(VectorTest, BaseTypes);

template<typename WordType>
class SingleRangeTest : public testing::Test {
    protected:

    using base_type = WordType;

    std::vector<bit::bit_vector<WordType>> random_bitvecs;
    std::vector<std::vector<bool>> random_boolvecs;
    std::vector<WordType> random_vec;
    size_t word_size = 4;
    size_t bit_size = word_size*bit::binary_digits<WordType>::value;

    void SetUp() override {
        random_vec = get_random_vec<WordType>(word_size);
        for (size_t cont_size = 1; cont_size < bit_size; ++cont_size) {
            auto bitvec = bit::bit_vector<WordType>(bit_size);
            std::memcpy(&(*bitvec.begin().base()), &(random_vec[0]), word_size);
            bitvec.resize(cont_size);

            auto boolvec = boolvec_from_bitvec(bitvec);
            random_bitvecs.push_back(bitvec);
            random_boolvecs.push_back(boolvec);
        }
    }
};
TYPED_TEST_SUITE(SingleRangeTest, BaseTypes);
//TYPED_TEST_SUITE_P(SingleRangeTest);
// ========================================================================== //
#endif // _FIXTURES_HPP_INCLUDED
// ========================================================================== //
