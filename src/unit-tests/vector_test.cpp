// =============================== FIXTURES ================================= //
// Project:         The Experimental Bit Algorithms Library
// Description:     Fixtures for testing 
// Creator:         Bryce Kille [2019]
// License:         BSD 3-Clause License
// ========================================================================== //


// ============================== PREAMBLE ================================== //
// C++ standard library
#include <math.h>
// Project sources
#include "bitlib/bit-containers/bit-containers.hpp"
#include "fixtures.hpp"
// Third-party libraries
#include "gtest/gtest.h"
// Miscellaneous
// ========================================================================== //

/*
 * Constructor tests
 */
// Tests the default c'tor.
TYPED_TEST(VectorTest, DefaultConstructor) {
    EXPECT_EQ(0u, this->empty_vec.size());
}

// Tests the size c'tor.
TYPED_TEST(VectorTest, SizeInitializerConstructor) {
    EXPECT_EQ(18, this->v2_.size());
}

// Tests the string c'tor.
TYPED_TEST(VectorTest, StringConstructor) {
    EXPECT_EQ(9, this->v3_.size());
    EXPECT_EQ(false, static_cast<bool>(this->v3_[0]));
    EXPECT_EQ(true, static_cast<bool>(this->v3_[8]));
}

// Test the copy c'tor
TYPED_TEST(VectorTest, CopyConstructor) {
    for (auto _ = 0; _ < 128; ++_) {
        this->empty_vec = this->v2_;
        EXPECT_NE(this->empty_vec.begin().base(), this->v2_.begin().base());
        EXPECT_TRUE(std::equal(this->empty_vec.begin(), this->empty_vec.end(), this->v2_.begin()));
        EXPECT_EQ(this->empty_vec.size(), this->v2_.size());
        this->v2_.insert(this->v2_.end(), bit::bit1);
        this->v2_.insert(this->v2_.end(), bit::bit0);
        this->v2_.insert(this->v2_.end(), bit::bit1);
    }
}

// Test the move c'tor
TYPED_TEST(VectorTest, MoveConstructor) {
    bit::bit_vector<typename TestFixture::base_type> v2_copy = this->v2_;
    auto v2_data = this->v2_.begin().base();
    this->empty_vec = std::move(this->v2_);
    EXPECT_EQ(this->empty_vec.begin().base(), v2_data);
    EXPECT_TRUE(std::equal(this->empty_vec.begin(), this->empty_vec.end(), v2_copy.begin()));
    EXPECT_EQ(this->empty_vec.size(), v2_copy.size());
}


/*
 * Element access
 */

// Test read
TYPED_TEST(VectorTest, BracketRead) {
    EXPECT_EQ(this->v3_[0], bit::bit0);
}

// Test write
TYPED_TEST(VectorTest, BracketWrite) {
    this->v3_[0] = bit::bit1;
    EXPECT_EQ(this->v3_[0], bit::bit1);
}

// Test at
TYPED_TEST(VectorTest, AtRead) {
    EXPECT_EQ(this->v3_.at(0), bit::bit0);
    EXPECT_EQ(this->v3_.at(8), bit::bit1);
}


/*
 * Iterators
 */
//TODO


/*
 * Capacity
 */

// Test empty
TYPED_TEST(VectorTest, Empty) {
    EXPECT_TRUE(this->empty_vec.empty());
    EXPECT_FALSE(this->v2_.empty());
}

// Test size
TYPED_TEST(VectorTest, Size) {
    EXPECT_EQ(this->empty_vec.size(), 0);
    EXPECT_EQ(this->v2_.size(), 18);
    EXPECT_EQ(this->v3_.size(), 9);
}

// Test reserve and capacity
TYPED_TEST(VectorTest, ReserveAndCapacity) {
    this->empty_vec.reserve(1);
    EXPECT_GE(this->empty_vec.capacity(), 1);
    this->empty_vec.reserve(9);
    EXPECT_GE(this->empty_vec.capacity(), 9);
    this->empty_vec.reserve(64);
    EXPECT_GE(this->empty_vec.capacity(), 64);
}

// Test shrink_to_fit
TYPED_TEST(VectorTest, ShrinkToFit) {
    this->empty_vec.shrink_to_fit();
    EXPECT_EQ(this->empty_vec.size(), 0);
    this->empty_vec.reserve(12345);
    this->empty_vec.shrink_to_fit();
    EXPECT_EQ(this->empty_vec.size(), 0);
}

/*
 * Modifiers
 */

// Test clear
TYPED_TEST(VectorTest, Clear) {
    this->empty_vec.clear();
    EXPECT_EQ(this->empty_vec.size(), 0);
    this->empty_vec.reserve(1);
    EXPECT_EQ(this->empty_vec.size(), 0);
    this->empty_vec.clear();
    EXPECT_EQ(this->empty_vec.size(), 0);
    this->v2_.clear();
    EXPECT_EQ(this->v2_.size(), 0);
    this->v3_.clear();
    EXPECT_EQ(this->v3_.size(), 0);
}

// Test insert
TYPED_TEST(VectorTest, InsertAtEnd1) {
    // First signature
    for (auto _ = 64*this->digits; _--;) {
        bool to_insert_bool = generate_random_number(0, 1) > 0 ? true : false;
        bit::bit_value to_insert_bit = to_insert_bool ? bit::bit1 : bit::bit0;
        auto bitret = this->empty_vec.insert(
                this->empty_vec.end(), 
                to_insert_bit);
        auto boolret = this->empty_vec_bool.insert(
                this->empty_vec_bool.end(), 
                to_insert_bool);
        EXPECT_TRUE(std::equal(
                    this->empty_vec.begin(), 
                    this->empty_vec.end(), 
                    this->empty_vec_bool.begin(), 
                    this->empty_vec_bool.end(), 
                    comparator));
        EXPECT_EQ(
                std::distance(this->empty_vec.begin(), bitret), 
                std::distance(this->empty_vec_bool.begin(), boolret));
    }
}
TYPED_TEST(VectorTest, InsertAtBegin1) {
    // First signature
    for (auto _ = 64*this->digits; _--;) {
        bool to_insert_bool = generate_random_number(0, 1) > 0 ? true : false;
        bit::bit_value to_insert_bit = to_insert_bool ? bit::bit1 : bit::bit0;
        auto bitret = this->empty_vec.insert(
                this->empty_vec.begin(), 
                to_insert_bit);
        auto boolret = this->empty_vec_bool.insert(
                this->empty_vec_bool.begin(), 
                to_insert_bool);
        EXPECT_TRUE(std::equal(
                    this->empty_vec.begin(), 
                    this->empty_vec.end(), 
                    this->empty_vec_bool.begin(), 
                    this->empty_vec_bool.end(), 
                    comparator));
        EXPECT_EQ(
                std::distance(this->empty_vec.begin(), bitret), 
                std::distance(this->empty_vec_bool.begin(), boolret));
    }
}

TYPED_TEST(VectorTest, InsertAtRand1) {
    // First signature
    for (auto _ = 64*this->digits; _--;) {
        auto insert_location = generate_random_number(0, this->empty_vec.size());
        bool to_insert_bool = generate_random_number(0, 1) > 0 ? true : false;
        bit::bit_value to_insert_bit = to_insert_bool ? bit::bit1 : bit::bit0;
        auto bitret = this->empty_vec.insert(
                this->empty_vec.begin() + insert_location,
                to_insert_bit);
        auto boolret = this->empty_vec_bool.insert(
                this->empty_vec_bool.begin() + insert_location, 
                to_insert_bool);
        EXPECT_TRUE(std::equal(
                    this->empty_vec.begin(), 
                    this->empty_vec.end(), 
                    this->empty_vec_bool.begin(), 
                    this->empty_vec_bool.end(), 
                    comparator));
        EXPECT_EQ(
                std::distance(this->empty_vec.begin(), bitret), 
                std::distance(this->empty_vec_bool.begin(), boolret));
    }
}

TYPED_TEST(VectorTest, InsertAtBegin2) {
    // Second signature
    for (auto _ = 16; _--;) {
        auto to_insert = generate_random_number(0, 4*this->digits);
        auto bitret = this->empty_vec.insert(
                this->empty_vec.begin(), 
                to_insert, 
                bit::bit1);
        auto boolret = this->empty_vec_bool.insert(
                this->empty_vec_bool.begin(),
                to_insert, 
                true);
        EXPECT_TRUE(std::equal(
                    this->empty_vec.begin(), 
                    this->empty_vec.end(), 
                    this->empty_vec_bool.begin(), 
                    this->empty_vec_bool.end(), 
                    comparator));
        EXPECT_EQ(
                std::distance(this->empty_vec.begin(), bitret), 
                std::distance(this->empty_vec_bool.begin(), boolret));
    }
}

TYPED_TEST(VectorTest, InsertAtEnd2) {
    // Second signature
    for (auto _ = 16; _--;) {
        auto to_insert = generate_random_number(0, 444*this->digits);
        auto bitret = this->empty_vec.insert(
                this->empty_vec.end(), 
                to_insert, 
                bit::bit1);
        auto boolret = this->empty_vec_bool.insert(
                this->empty_vec_bool.end(), 
                to_insert, 
                true);
        EXPECT_TRUE(std::equal(
                    this->empty_vec.begin(), 
                    this->empty_vec.end(), 
                    this->empty_vec_bool.begin(), 
                    this->empty_vec_bool.end(), 
                    comparator));
        EXPECT_EQ(
                std::distance(this->empty_vec.begin(), bitret), 
                std::distance(this->empty_vec_bool.begin(), boolret));
    }
}

TYPED_TEST(VectorTest, InsertAtRand2) {
    // Second signature
    for (auto _ = 16; _--;) {
        auto to_insert = generate_random_number(0, 4*this->digits);
        auto insert_location = generate_random_number(0, this->empty_vec.size());
        auto bitret = this->empty_vec.insert(
                this->empty_vec.begin() + insert_location, 
                to_insert, 
                bit::bit1);
        auto boolret = this->empty_vec_bool.insert(
                this->empty_vec_bool.begin() + insert_location, 
                to_insert, 
                true);
        EXPECT_TRUE(std::equal(
                    this->empty_vec.begin(), 
                    this->empty_vec.end(), 
                    this->empty_vec_bool.begin(), 
                    this->empty_vec_bool.end(), 
                    comparator));
        EXPECT_EQ(
                std::distance(this->empty_vec.begin(), bitret), 
                std::distance(this->empty_vec_bool.begin(), boolret));
    }
}

TYPED_TEST(VectorTest, InsertAtBegin3) {
    // Third signature
    for (auto _ = 16; _--;) {
        auto to_insert = generate_random_number(0, this->random_bitvecs.size() - 1);
        auto insert_bitvec = this->random_bitvecs[to_insert];
        auto insert_boolvec = this->random_boolvecs[to_insert];
        auto bitret = this->empty_vec.insert(
                this->empty_vec.begin(), 
                insert_bitvec.begin(), 
                insert_bitvec.end());
        auto boolret = this->empty_vec_bool.insert(
                this->empty_vec_bool.begin(), 
                insert_boolvec.begin(), 
                insert_boolvec.end());
        EXPECT_TRUE(std::equal(
                    this->empty_vec.begin(), 
                    this->empty_vec.end(), 
                    this->empty_vec_bool.begin(), 
                    this->empty_vec_bool.end(), 
                    comparator));
        EXPECT_EQ(
                std::distance(this->empty_vec.begin(), bitret), 
                std::distance(this->empty_vec_bool.begin(), boolret));
    }
}

TYPED_TEST(VectorTest, InsertAtEnd3) {
    // Third signature
    for (auto _ = 16; _--;) {
        auto to_insert = generate_random_number(0, this->random_bitvecs.size() - 1);
        auto insert_bitvec = this->random_bitvecs[to_insert];
        auto insert_boolvec = this->random_boolvecs[to_insert];
        auto bitret = this->empty_vec.insert(
                this->empty_vec.end(), 
                insert_bitvec.begin(), 
                insert_bitvec.end());
        auto boolret = this->empty_vec_bool.insert(
                this->empty_vec_bool.end(), 
                insert_boolvec.begin(), 
                insert_boolvec.end());
        EXPECT_TRUE(std::equal(
                    this->empty_vec.begin(), 
                    this->empty_vec.end(), 
                    this->empty_vec_bool.begin(), 
                    this->empty_vec_bool.end(), 
                    comparator));
        EXPECT_EQ(
                std::distance(this->empty_vec.begin(), bitret), 
                std::distance(this->empty_vec_bool.begin(), boolret));
    }
}

TYPED_TEST(VectorTest, InsertAtRand3) {
    // Third signature
    for (auto _ = 16; _--;) {
        auto to_insert = generate_random_number(0, this->random_bitvecs.size() - 1);
        auto insert_bitvec = this->random_bitvecs[to_insert];
        auto insert_boolvec = this->random_boolvecs[to_insert];
        auto insert_location = generate_random_number(0, this->empty_vec.size());
        auto bitret = this->empty_vec.insert(
                this->empty_vec.begin() + insert_location, 
                insert_bitvec.begin(), 
                insert_bitvec.end());
        auto boolret = this->empty_vec_bool.insert(
                this->empty_vec_bool.begin() + insert_location, 
                insert_boolvec.begin(), 
                insert_boolvec.end());
        EXPECT_TRUE(std::equal(
                    this->empty_vec.begin(), 
                    this->empty_vec.end(), 
                    this->empty_vec_bool.begin(), 
                    this->empty_vec_bool.end(), 
                    comparator));
        EXPECT_EQ(
                std::distance(this->empty_vec.begin(), bitret), 
                std::distance(this->empty_vec_bool.begin(), boolret));
    }
}

// Test erase
TYPED_TEST(VectorTest, Erase1) {
    auto v3_copy = this->v3_;
    auto bitret = this->v3_.erase(this->v3_.begin());
    EXPECT_TRUE(std::equal(this->v3_.begin(), this->v3_.end(), v3_copy.begin() + 1));
    EXPECT_EQ(std::distance(this->v3_.begin(), bitret), 0);
    bitret = this->v3_.erase(this->v3_.end() - 1);
    EXPECT_TRUE(std::equal(this->v3_.begin(), this->v3_.end(), v3_copy.begin() + 1));
    EXPECT_EQ(std::distance(this->v3_.begin(), bitret), v3_copy.size() - 2);
}

//TYPED_TEST(VectorTest, Print) {
    //std::cout << this->v3_ << std::endl;
//}
