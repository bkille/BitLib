// ============================= SHIFT  TESTS =============================== //
// Project:         The Experimental Bit Algorithms Library
// Description:     Tests for shift algorithms 
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

TYPED_TEST(SingleRangeTest, ShiftLeft) {
    for (size_t idx = 0; idx < this->bit_size - 1; ++idx) {
        bit::bit_vector<typename TestFixture::base_type> bitvec = this->random_bitvecs[idx];
        std::vector<bool> boolvec = this->random_boolvecs[idx];
        ASSERT_EQ(boolvec.size(), bitvec.size());
        unsigned long long start = generate_random_number(0, std::min<unsigned long long>(bitvec.size() - 1, 16));
        unsigned long long shift_val = generate_random_number(0, bitvec.size() + 1);
        auto bitret = bit::shift_left(bitvec.begin() + start, bitvec.end(), shift_val); 
        auto boolret = bit::word_shift_left(boolvec.begin() + start, boolvec.end(), shift_val); 
        EXPECT_EQ(std::distance(bitvec.begin(), bitret), std::distance(boolvec.begin(), boolret));
        EXPECT_TRUE(std::equal(
                    bitvec.begin(), bitret,
                    boolvec.begin(), boolret, comparator)
        );
    }
}


TYPED_TEST(SingleRangeTest, ShiftRight) {
    for (size_t idx = 0; idx < this->bit_size - 1; ++idx) {
        bit::bit_vector<typename TestFixture::base_type> bitvec = this->random_bitvecs[idx];
        std::vector<bool> boolvec = this->random_boolvecs[idx];
        unsigned long long start = generate_random_number(0, std::min<unsigned long long>(bitvec.size() - 1, 16));
        unsigned long long shift_val = generate_random_number(0, bitvec.size() + 1);
        auto bitret = bit::shift_right(bitvec.begin() + start, bitvec.end(), shift_val); 
        auto boolret = bit::word_shift_right(boolvec.begin() + start, boolvec.end(), shift_val); 
        EXPECT_EQ(std::distance(bitvec.begin(), bitret), std::distance(boolvec.begin(), boolret));
        EXPECT_TRUE(std::equal(
                    bitret, bitvec.end(),
                    boolret, boolvec.end(), comparator)
        );
    }
}

//REGISTER_TYPED_TEST_SUITE_P(, DoesBlah, HasPropertyA);
//INSTANTIATE_TYPED_TEST_SUITE_P(Instantiation, SingleRangeTest, BaseTypes);
