// ============================= FILL  TESTS =============================== //
// Project:         The Experimental Bit Algorithms Library
// Description:     Tests for fill algorithms 
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

TYPED_TEST(SingleRangeTest, Fill) {
    for (size_t idx = 0; idx < this->random_bitvecs.size(); ++idx) {
        using WordType = typename TestFixture::base_type;
        bit::bit_vector<WordType>& bitvec = this->random_bitvecs[idx];
        std::vector<bool>& boolvec = this->random_boolvecs[idx];
        ASSERT_EQ(boolvec.size(), bitvec.size());
        constexpr auto digits = bit::binary_digits<WordType>::value;
        unsigned long long start = generate_random_number(
                0,
                std::min<long long>(bitvec.size(), digits + 1));
        unsigned long long end = generate_random_number(
                0,
                std::min<long long>(digits + 1, bitvec.size() - start));
        auto fill_val_bit = bit::bit1;
        auto fill_val_bool = true;
        bit::fill(bitvec.begin() + start, bitvec.end() - end, fill_val_bit); 
        std::fill(boolvec.begin() + start, boolvec.end() - end, fill_val_bool); 
        EXPECT_TRUE(std::equal(
                    bitvec.begin(), bitvec.end(),
                    boolvec.begin(), boolvec.end(), comparator)
        );
        start = generate_random_number(
                0,
                std::min<long long>(bitvec.size(), digits + 1));
        end = generate_random_number(
                0,
                std::min<long long>(digits + 1, bitvec.size() - start));
        fill_val_bit = bit::bit0;
        fill_val_bool = false;
        bit::fill(bitvec.begin() + start, bitvec.end() - end, fill_val_bit); 
        std::fill(boolvec.begin() + start, boolvec.end() - end, fill_val_bool); 
        EXPECT_TRUE(std::equal(
                    bitvec.begin(), bitvec.end(),
                    boolvec.begin(), boolvec.end(), comparator)
        );
    }
}

