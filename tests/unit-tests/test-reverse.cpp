// ============================= REVERSE TESTS ============================== //
// Project:         The Experimental Bit Algorithms Library
// Description:     Tests for count algorithms 
// Contributor(s):  Bryce Kille
// License:         BSD 3-Clause License
// ========================================================================== //


// ============================== PREAMBLE ================================== //
// C++ standard library
#include <math.h>
#include <algorithm>
// Project sources
#include "bit-algorithms/bit_algorithm.hpp"
#include "bit-containers/bit-containers.hpp"
#include "fixtures.hpp"
// Third-party libraries
#include "gtest/gtest.h"
// Miscellaneous
// ========================================================================== //

TYPED_TEST(SingleRangeTest, Reverse) {
    for (size_t idx = 0; idx < this->bit_size - 1; ++idx) {
        bit::bit_vector<typename TestFixture::base_type> bitvec = this->random_bitvecs[idx];
        std::vector<bool> boolvec = this->random_boolvecs[idx];
        size_t start_count = 16;
        while (start_count--) {
            unsigned long long start = generate_random_number(0, std::min<unsigned long long>(bitvec.size() - 1, 16));
            bit::reverse(bitvec.begin() + start, bitvec.end()); 
            std::reverse(boolvec.begin() + start, boolvec.end()); 
            EXPECT_TRUE(std::equal(
                        bitvec.begin(), bitvec.end(),
                        boolvec.begin(), boolvec.end(), comparator)
            );
        }  
    }
};


