// ============================= TRANSFORM  TESTS =============================== //
// Project:         The Experimental Bit Algorithms Library
// Description:     Tests for transform algorithms 
// Contributor(s):  Bryce Kille
// License:         BSD 3-Clause License
// ========================================================================== //


// ============================== PREAMBLE ================================== //
// C++ standard library
#include <math.h>
#include <algorithm>
#include <functional>
// Project sources
#include "bitlib/bit-algorithms/bit_algorithm.hpp"
#include "bitlib/bit-containers/bit-containers.hpp"
#include "fixtures.hpp"
// Third-party libraries
#include "gtest/gtest.h"
// Miscellaneous
// ========================================================================== //

TYPED_TEST(DoubleRangeTest, TransformUnaryOp) {
    for (size_t idx = 0; idx < this->bit_size - 1; ++idx) {
        bit::bit_vector<typename TestFixture::base_type> bitvec1 = this->random_bitvecs1[idx];
        bit::bit_vector<typename TestFixture::base_type> bitvec2 = this->random_bitvecs2[idx];
        std::vector<bool> boolvec1 = this->random_boolvecs1[idx];
        std::vector<bool> boolvec2 = this->random_boolvecs2[idx];
        long long start1 = generate_random_number(0, std::min<long long>(bitvec1.size() - 1, 16));
        long long start2 = generate_random_number(0, std::min<long long>(bitvec2.size() - 1, 16));
        long long end1 = generate_random_number(std::max<long long>(0, start2 - start1), bitvec1.size() - start1);
        auto unary_op_bit = std::bit_not<typename TestFixture::base_type>();
        auto unary_op_bool = [](bool b) {return !b;};
        auto bitret = bit::transform(bitvec1.begin() + start1, bitvec1.end() - end1, bitvec2.begin() + start2, unary_op_bit); 
        auto boolret = std::transform(boolvec1.begin() + start1, boolvec1.end() - end1, boolvec2.begin() + start2, unary_op_bool); 
        EXPECT_EQ(bit::distance(bitvec2.begin(), bitret), std::distance(boolvec2.begin(), boolret));
        EXPECT_TRUE(std::equal(
                    bitvec2.begin(), bitvec2.end(),
                    boolvec2.begin(), boolvec2.end(), comparator)
        );
    }
}

TYPED_TEST(DoubleRangeTest, TransformBinaryOp) {
    for (size_t idx = 0; idx < this->bit_size - 1; ++idx) {
        bit::bit_vector<typename TestFixture::base_type> bitvec1 = this->random_bitvecs1[idx];
        bit::bit_vector<typename TestFixture::base_type> bitvec2 = this->random_bitvecs2[idx];
        std::vector<bool> boolvec1 = this->random_boolvecs1[idx];
        std::vector<bool> boolvec2 = this->random_boolvecs2[idx];
        long long start1 = generate_random_number(0, std::min<long long>(bitvec1.size() - 1, 16));
        long long start2 = generate_random_number(0, std::min<long long>(bitvec2.size() - 1, 16));
        long long end1 = generate_random_number(std::max<long long>(0, start2 - start1), bitvec1.size() - start1);
        auto binary_op_bit = std::bit_and<typename TestFixture::base_type>();
        auto binary_op_bool = [](bool a, bool b) {return a && b;};
        auto bitret = bit::transform(
                bitvec1.begin() + start1, 
                bitvec1.end() - end1, 
                bitvec2.begin() + start2, 
                bitvec2.begin() + start2, 
                binary_op_bit); 
        auto boolret = std::transform(
                boolvec1.begin() + start1, 
                boolvec1.end() - end1, 
                boolvec2.begin() + start2, 
                boolvec2.begin() + start2, 
                binary_op_bool); 
        EXPECT_EQ(
                bit::distance(bitvec2.begin(), bitret), 
                std::distance(boolvec2.begin(), boolret));
        EXPECT_TRUE(std::equal(
                    bitvec2.begin(), bitvec2.end(),
                    boolvec2.begin(), boolvec2.end(), comparator)
        );
    }
}

