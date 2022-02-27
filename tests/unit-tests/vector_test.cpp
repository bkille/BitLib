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
#include "bit-containers/bit-containers.hpp"
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
    EXPECT_EQ(256, this->v2_.size());
}

// Tests the string c'tor.
TYPED_TEST(VectorTest, StringConstructor) {
    EXPECT_EQ(9, this->v3_.size());
    EXPECT_EQ(false, static_cast<bool>(this->v3_[0]));
    EXPECT_EQ(true, static_cast<bool>(this->v3_[8]));
}

// Test the copy c'tor
TYPED_TEST(VectorTest, CopyConstructor) {
    this->empty_vec = this->v2_;
    EXPECT_NE(this->empty_vec.begin().base(), this->v2_.begin().base());
    EXPECT_TRUE(std::equal(this->empty_vec.begin(), this->empty_vec.end(), this->v2_.begin()));
    EXPECT_EQ(this->empty_vec.size(), this->v2_.size());
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
    EXPECT_EQ(this->v2_.size(), 256);
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
TYPED_TEST(VectorTest, Insert1) {
    // First signature
    auto bitret = this->empty_vec.insert(this->empty_vec.begin(), bit::bit1);
    EXPECT_EQ(std::distance(this->empty_vec.begin(), bitret), 0);
    EXPECT_EQ(this->empty_vec[0], bit::bit1);
    bitret = this->empty_vec.insert(this->empty_vec.begin(), bit::bit0);
    EXPECT_EQ(std::distance(this->empty_vec.begin(), bitret), 0);
    EXPECT_EQ(this->empty_vec[0], bit::bit0);
    bitret = this->empty_vec.insert(this->empty_vec.end(), bit::bit1);
    EXPECT_EQ(std::distance(this->empty_vec.begin(), bitret), 2);
    EXPECT_EQ(*(this->empty_vec.end()-1), bit::bit1);
}

TYPED_TEST(VectorTest, Insert2) {
    // Second signature
    auto bitret = this->empty_vec.insert(this->empty_vec.begin(), 9, bit::bit1);
    EXPECT_EQ(std::distance(this->empty_vec.begin(), bitret), 0);
    EXPECT_EQ(this->empty_vec[0], bit::bit1);
    EXPECT_EQ(this->empty_vec[8], bit::bit1);
    bitret = this->empty_vec.insert(this->empty_vec.begin() + 2, 4, bit::bit0);
    EXPECT_EQ(std::distance(this->empty_vec.begin(), bitret), 2);
    EXPECT_EQ(this->empty_vec[1], bit::bit1);
    EXPECT_EQ(this->empty_vec[2], bit::bit0);
    EXPECT_EQ(this->empty_vec[5], bit::bit0);
    EXPECT_EQ(this->empty_vec[6], bit::bit1);
    bitret = this->empty_vec.insert(this->empty_vec.end(), 10, bit::bit1);
    EXPECT_EQ(std::distance(this->empty_vec.begin(), bitret), 13);
    EXPECT_EQ(*(this->empty_vec.end()-1), bit::bit1);
}

TYPED_TEST(VectorTest, Insert3) {
    // Second signature
    this->empty_vec.insert(this->empty_vec.begin(), 9, bit::bit1);
    auto bitret = this->empty_vec.insert(this->empty_vec.begin() + 4, this->v3_.begin(), this->v3_.end());
    EXPECT_TRUE(std::equal(this->v3_.begin(), this->v3_.end(), this->empty_vec.begin() + 4));
    EXPECT_EQ(std::distance(this->empty_vec.begin(), bitret), 4);
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
