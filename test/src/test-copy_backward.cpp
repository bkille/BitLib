// ============================= COPY_BACKWARD  TESTS =============================== //
// Project:         The Experimental Bit Algorithms Library
// Description:     Tests for copy_backward algorithms 
// Contributor(s):  Bryce Kille
// License:         BSD 3-Clause License
// ========================================================================== //


// ============================== PREAMBLE ================================== //
// C++ standard library
#include <math.h>
#include <algorithm>
// Project sources
#include "bitlib/bit-algorithms/bit_algorithm.hpp"
#include "bitlib/bit-containers/bit-containers.hpp"
#include "fixtures.hpp"
// Third-party libraries
#include "gtest/gtest.h"
// Miscellaneous
// ========================================================================== //

TYPED_TEST(DoubleRangeTest, CopyBackward) {
    for (size_t idx = 0; idx < this->random_bitvecs1.size(); ++idx) {
        using WordType = typename TestFixture::base_type;
        bit::bit_vector<WordType>& bitvec1 = this->random_bitvecs1[idx];
        bit::bit_vector<WordType>& bitvec2 = this->random_bitvecs2[idx];
        constexpr auto digits = bit::binary_digits<WordType>::value;
        std::vector<bool>& boolvec1 = this->random_boolvecs1[idx];
        std::vector<bool>& boolvec2 = this->random_boolvecs2[idx];
        long long start1 = generate_random_number(
                0, 
                std::min<long long>(bitvec1.size() - 1, digits + 1));
        long long start2 = generate_random_number(
                0, 
                std::min<long long>(bitvec2.size() - 1, digits + 1));
        const auto min_range = (start2 > start1) ? start2 - start1 : 0;
        const auto max_range = std::max<long long>(
                min_range,
                std::min<long long>(digits, bitvec1.size() - start1)); 
        long long end1 = generate_random_number(min_range, max_range);
        auto d = (bitvec1.size() - end1) - start1;

        auto bitret = bit::copy_backward(
                bitvec1.begin() + start1, 
                bitvec1.end() - end1,
                bitvec2.begin() + start2 + d); 
        auto boolret = std::copy_backward(
                boolvec1.begin() + start1, 
                boolvec1.end() - end1, 
                boolvec2.begin() + start2 + d); 
        EXPECT_EQ(
                bit::distance(bitvec2.begin(), bitret),
                std::distance(boolvec2.begin(), boolret));
        EXPECT_TRUE(std::equal(
                    bitvec2.begin(), bitvec2.end(),
                    boolvec2.begin(), boolvec2.end(), comparator)
        );
        start2 = generate_random_number(start1, start1 + end1);
        bitret = bit::copy_backward(
                bitvec1.begin() + start1, 
                bitvec1.end() - end1,
                bitvec1.begin() + start2 + d); 
        boolret = std::copy_backward(
                boolvec1.begin() + start1, 
                boolvec1.end() - end1, 
                boolvec1.begin() + start2 + d); 
        EXPECT_EQ(
                bit::distance(bitvec1.begin(), bitret),
                std::distance(boolvec1.begin(), boolret));
        EXPECT_TRUE(std::equal(
                    bitvec1.begin(), bitvec1.end(),
                    boolvec1.begin(), boolvec1.end(), comparator)
        );
    }
}

