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
// Project sources
#include "bit-iterator/bit_iterator.hpp"
#include "bit-containers/bit-containers.hpp"
#include "../utils/test_utils.hpp"
// Third-party libraries
#include "gtest/gtest.h"
// Miscellaneous
// ========================================================================== //

using BaseTypes = ::testing::Types<unsigned char, unsigned short, unsigned int, unsigned long long>;

template<typename WordType>
class VectorTest : public testing::Test {
    protected:
    void SetUp() override {
        empty_vec = bit::bit_vector<WordType>(); 
    }

   using base_type = WordType;
   bit::bit_vector<WordType> empty_vec;
   bit::bit_vector<WordType> v2_ {256};
   bit::bit_vector<WordType> v3_ {"010111111"};
};

TYPED_TEST_SUITE(VectorTest, BaseTypes);
// ========================================================================== //
#endif // _FIXTURES_HPP_INCLUDED
// ========================================================================== //
