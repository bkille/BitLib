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
#include "bit-algorithms/bit_algorithm.hpp"
#include "bit-containers/bit-containers.hpp"
#include "fixtures.hpp"
// Third-party libraries
#include "gtest/gtest.h"
// Miscellaneous
// ========================================================================== //

//TYPED_TEST(SingleRangeTest, ShiftLeft) {
    //for (size_t idx = 0; idx < this->bit_size - 1; ++idx) {
        //bit::bit_vector<typename TestFixture::base_type> bitvec = this->random_bitvecs[idx];
        //std::vector<bool> boolvec = this->random_boolvecs[idx];
        //ASSERT_EQ(boolvec.size(), bitvec.size());
        //unsigned long long start = generate_random_number(0, std::min<unsigned long long>(bitvec.size() - 1, 16));
        //unsigned long long shift_val = generate_random_number(0, bitvec.size() + 1);
        ////std::cout << bitvec.debug_string(bitvec.begin(), bitvec.end()) << std::endl;
        ////std::cout << bitvec.size() << "->" << start <<":"<< shift_val <<":" <<bitvec.size() << std::endl;
        //auto bitret = bit::shift_left(bitvec.begin() + start, bitvec.end(), shift_val); 
        ////std::cout << bitvec.debug_string(bitvec.begin(), bitvec.end()) << std::endl;
        //auto boolret = bit::word_shift_left(boolvec.begin() + start, boolvec.end(), shift_val); 
        //bit::bit_vector<typename TestFixture::base_type> boolbit{};
        ////std::cout << boolvec.size() << std::endl;
        //for (auto b : boolvec) {
            //boolbit.push_back(b ? bit::bit1 : bit::bit0);
        //}
        ////std::cout << boolbit.debug_string(boolbit.begin(), boolbit.end()) << std::endl;
        
        //EXPECT_TRUE(std::equal(
                    //bitvec.begin(), bitvec.end(),
                    //boolvec.begin(), boolvec.end(), comparator)
        //);
        //EXPECT_EQ(std::distance(bitvec.begin(), bitret), std::distance(boolvec.begin(), boolret));
    //}
//};


TYPED_TEST(SingleRangeTest, ShiftRight) {
    for (size_t idx = 0; idx < this->bit_size - 1; ++idx) {
        bit::bit_vector<typename TestFixture::base_type> bitvec = this->random_bitvecs[idx];
        std::vector<bool> boolvec = this->random_boolvecs[idx];
        unsigned long long start = generate_random_number(0, std::min<unsigned long long>(bitvec.size() - 1, 16));
        unsigned long long shift_val = generate_random_number(0, bitvec.size() + 1);
        auto bitret = bit::shift_right(bitvec.begin() + start, bitvec.end(), shift_val); 
        auto boolret = bit::word_shift_right(boolvec.begin() + start, boolvec.end(), shift_val); 
        EXPECT_TRUE(std::equal(
                    bitvec.begin(), bitvec.end(),
                    boolvec.begin(), boolvec.end(), comparator)
        );
        EXPECT_EQ(std::distance(bitvec.begin(), bitret), std::distance(boolvec.begin(), boolret));
    }
};

//REGISTER_TYPED_TEST_SUITE_P(, DoesBlah, HasPropertyA);
//INSTANTIATE_TYPED_TEST_SUITE_P(Instantiation, SingleRangeTest, BaseTypes);
