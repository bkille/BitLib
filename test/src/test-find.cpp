// ============================== FIND  TESTS =============================== //
// Project:         The Experimental Bit Algorithms Library
// Description:     Tests for find algorithms 
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

TYPED_TEST(SingleRangeTest, Find) {
    for (size_t idx = 0; idx < this->random_bitvecs.size(); ++idx) {
        bit::bit_vector<typename TestFixture::base_type> bitvec = this->random_bitvecs[idx];
        std::vector<bool> boolvec = this->random_boolvecs[idx];
        size_t start_count = 16;
        while (start_count--) {
            unsigned long long start = generate_random_number(0, std::min<unsigned long long>(bitvec.size() - 1, 16));
            auto bitret = bit::find(bitvec.begin() + start, bitvec.end(), bit::bit1); 
            auto boolret = std::find(boolvec.begin() + start, boolvec.end(), true); 
            EXPECT_EQ(std::distance(bitvec.begin(), bitret), std::distance(boolvec.begin(), boolret));
            bitret = bit::find(bitvec.begin() + start, bitvec.end(), bit::bit0); 
            boolret = std::find(boolvec.begin() + start, boolvec.end(), false); 
            EXPECT_EQ(std::distance(bitvec.begin(), bitret), std::distance(boolvec.begin(), boolret));
        }  
    }
}


