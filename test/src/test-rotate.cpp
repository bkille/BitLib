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

TYPED_TEST(SingleRangeTest, Rotate) {
    using WordType = typename TestFixture::base_type;
    for (size_t idx = 0; idx < this->random_bitvecs.size(); ++idx) {
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


