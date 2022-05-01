// ========================== SWAP_RANGES  TESTS ============================ //
// Project:         The Experimental Bit Algorithms Library
// Description:     Tests for swap_ranges algorithms 
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

TYPED_TEST(DoubleRangeTest, SwapRanges) {
    for (size_t idx = 0; idx < this->bit_size - 1; ++idx) {
        using WordType = typename TestFixture::base_type;
        auto digits = bit::binary_digits<WordType>::value;
        bit::bit_vector<typename TestFixture::base_type> bitvec1 = this->random_bitvecs1[idx];
        bit::bit_vector<typename TestFixture::base_type> bitvec2 = this->random_bitvecs2[idx];
        std::vector<bool> boolvec1 = this->random_boolvecs1[idx];
        std::vector<bool> boolvec2 = this->random_boolvecs2[idx];
        long long start1 = generate_random_number(0, std::min<long long>(bitvec1.size() - 1, digits + 1));
        long long start2 = generate_random_number(0, std::min<long long>(bitvec2.size() - 1, digits + 1));
        long long end1 = generate_random_number(std::max<long long>(0, start2 - start1), bitvec1.size() - start1);
        auto bitret = bit::swap_ranges(bitvec1.begin() + start1, bitvec1.end() - end1, bitvec2.begin() + start2); 
        auto boolret = std::swap_ranges(boolvec1.begin() + start1, boolvec1.end() - end1, boolvec2.begin() + start2); 
        EXPECT_EQ(bit::distance(bitvec2.begin(), bitret), std::distance(boolvec2.begin(), boolret));
        EXPECT_TRUE(std::equal(
                    bitvec2.begin(), bitvec2.end(),
                    boolvec2.begin(), boolvec2.end(), comparator)
        );
    }
}


