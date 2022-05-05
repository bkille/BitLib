// ============================= ROTATE TESTS =============================== //
// Project:         The Experimental Bit Algorithms Library
// Name:            rotate.hpp
// Description:     Tests for rotate algorithms 
// Creator:         Vincent Reverdy
// Contributor(s):  Bryce Kille [2019]
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
        bit::bit_vector<WordType>& bitvec1 = this->random_bitvecs1[idx];
        bit::bit_vector<WordType>& bitvec2 = this->random_bitvecs2[idx];
        constexpr auto digits = bit::binary_digits<typename TestFixture::base_type>::value;
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


TYPED_TEST(SingleRangeTest, Rotate) {
    using WordType = typename TestFixture::base_type;
    for (size_t idx = 0; idx < this->bit_size - 1; ++idx) {
        bit::bit_vector<WordType>& bitvec = this->random_bitvecs[idx];
        std::vector<bool> boolvec = this->random_boolvecs[idx];
        size_t start_count = 16;
        while (start_count--) {
            unsigned long long start = generate_random_number(0, std::min<unsigned long long>(bitvec.size() - 1, 16));
            size_t middle_count = 16;
            while (middle_count--) {
                unsigned long long middle = generate_random_number(start, bitvec.size() - 1);
                auto bitret = bit::rotate(bitvec.begin() + start, bitvec.begin() + middle, bitvec.end()); 
                auto boolret = std::rotate(boolvec.begin() + start, boolvec.begin() + middle, boolvec.end()); 
                EXPECT_TRUE(std::equal(
                            bitvec.begin(), bitvec.end(),
                            boolvec.begin(), boolvec.end(), comparator)
                );
                EXPECT_EQ(std::distance(bitvec.begin(), bitret), std::distance(boolvec.begin(), boolret));
            }
        }  
    }
}


