#include "gtest/gtest.h"
#include <math.h>

#include "bitvector.hpp"

class VectorTest : public testing::Test {
    protected:
        void SetUp() override {
            v1_ = bit::bit_vector<unsigned char>(); 
        }

   bit::bit_vector<unsigned char> v1_;
   bit::bit_vector<unsigned char> v2_ {256};
   bit::bit_vector<unsigned char> v3_ {"010111111"};

};

/*
 * Constructor tests
 */
// Tests the default c'tor.
TEST_F(VectorTest, DefaultConstructor) {
    EXPECT_EQ(0u, v1_.size());
}

// Tests the size c'tor.
TEST_F(VectorTest, SizeInitializerConstructor) {
    EXPECT_EQ(256, v2_.size());
}

// Tests the string c'tor.
TEST_F(VectorTest, StringConstructor) {
    EXPECT_EQ(9, v3_.size());
    EXPECT_EQ(false, static_cast<bool>(v3_[0]));
    EXPECT_EQ(true, static_cast<bool>(v3_[8]));
}

// Test the copy c'tor
TEST_F(VectorTest, CopyConstructor) {
    v1_ = v2_;
    EXPECT_NE(v1_.begin().base(), v2_.begin().base());
    EXPECT_TRUE(std::equal(v1_.begin(), v1_.end(), v2_.begin()));
    EXPECT_EQ(v1_.size(), v2_.size());
}

// Test the move c'tor
TEST_F(VectorTest, MoveConstructor) {
    bit::bit_vector<unsigned char> v2_copy = v2_;
    auto v2_data = v2_.begin().base();
    v1_ = std::move(v2_);
    EXPECT_EQ(v1_.begin().base(), v2_data);
    EXPECT_TRUE(std::equal(v1_.begin(), v1_.end(), v2_copy.begin()));
    EXPECT_EQ(v1_.size(), v2_copy.size());
}


/*
 * Element access
 */

// Test read
TEST_F(VectorTest, BracketRead) {
    EXPECT_EQ(v3_[0], bit::bit0);
}

// Test write
TEST_F(VectorTest, BracketWrite) {
    v3_[0] = bit::bit1;
    EXPECT_EQ(v3_[0], bit::bit1);
}

// Test at
TEST_F(VectorTest, AtRead) {
    EXPECT_EQ(v3_.at(0), bit::bit0);
    EXPECT_EQ(v3_.at(8), bit::bit1);
}


/*
 * Iterators
 */
//TODO


/*
 * Capacity
 */

// Test empty
TEST_F(VectorTest, Empty) {
    EXPECT_TRUE(v1_.empty());
    EXPECT_FALSE(v2_.empty());
}

// Test size
TEST_F(VectorTest, Size) {
    EXPECT_EQ(v1_.size(), 0);
    EXPECT_EQ(v2_.size(), 256);
    EXPECT_EQ(v3_.size(), 9);
}

// Test reserve and capacity
TEST_F(VectorTest, ReserveAndCapacity) {
    v1_.reserve(1);
    EXPECT_GE(v1_.capacity(), 1);
    v1_.reserve(9);
    EXPECT_GE(v1_.capacity(), 9);
    v1_.reserve(64);
    EXPECT_GE(v1_.capacity(), 64);
}

// Test shrink_to_fit
TEST_F(VectorTest, ShrinkToFit) {
    v1_.shrink_to_fit();
    EXPECT_EQ(v1_.size(), 0);
    v1_.reserve(12345);
    v1_.shrink_to_fit();
    EXPECT_EQ(v1_.size(), 0);
}

/*
 * Modifiers
 */

// Test clear
TEST_F(VectorTest, Clear) {
    v1_.clear();
    EXPECT_EQ(v1_.size(), 0);
    v1_.reserve(1);
    EXPECT_EQ(v1_.size(), 0);
    v1_.clear();
    EXPECT_EQ(v1_.size(), 0);
    v2_.clear();
    EXPECT_EQ(v2_.size(), 0);
    v3_.clear();
    EXPECT_EQ(v3_.size(), 0);
}

// Test insert
TEST_F(VectorTest, Insert1) {
    // First signature
    auto bitret = v1_.insert(v1_.begin(), bit::bit1);
    EXPECT_EQ(std::distance(v1_.begin(), bitret), 0);
    EXPECT_EQ(v1_[0], bit::bit1);
    bitret = v1_.insert(v1_.begin(), bit::bit0);
    EXPECT_EQ(std::distance(v1_.begin(), bitret), 0);
    EXPECT_EQ(v1_[0], bit::bit0);
    bitret = v1_.insert(v1_.end(), bit::bit1);
    EXPECT_EQ(std::distance(v1_.begin(), bitret), 2);
    EXPECT_EQ(*(v1_.end()-1), bit::bit1);
}

TEST_F(VectorTest, Insert2) {
    // Second signature
    auto bitret = v1_.insert(v1_.begin(), 9, bit::bit1);
    EXPECT_EQ(std::distance(v1_.begin(), bitret), 0);
    EXPECT_EQ(v1_[0], bit::bit1);
    EXPECT_EQ(v1_[8], bit::bit1);
    bitret = v1_.insert(v1_.begin() + 2, 4, bit::bit0);
    EXPECT_EQ(std::distance(v1_.begin(), bitret), 2);
    EXPECT_EQ(v1_[1], bit::bit1);
    EXPECT_EQ(v1_[2], bit::bit0);
    EXPECT_EQ(v1_[5], bit::bit0);
    EXPECT_EQ(v1_[6], bit::bit1);
    bitret = v1_.insert(v1_.end(), 10, bit::bit1);
    EXPECT_EQ(std::distance(v1_.begin(), bitret), 13);
    EXPECT_EQ(*(v1_.end()-1), bit::bit1);
}

TEST_F(VectorTest, Insert3) {
    // Second signature
    v1_.insert(v1_.begin(), 9, bit::bit1);
    auto bitret = v1_.insert(v1_.begin() + 4, v3_.begin(), v3_.end());
    EXPECT_TRUE(std::equal(v3_.begin(), v3_.end(), v1_.begin() + 4));
    EXPECT_EQ(std::distance(v1_.begin(), bitret), 4);
}

// Test erase
TEST_F(VectorTest, Erase1) {
    auto v3_copy = v3_;
    auto bitret = v3_.erase(v3_.begin());
    EXPECT_TRUE(std::equal(v3_.begin(), v3_.end(), v3_copy.begin() + 1));
    EXPECT_EQ(std::distance(v3_.begin(), bitret), 0);
    bitret = v3_.erase(v3_.end() - 1);
    EXPECT_TRUE(std::equal(v3_.begin(), v3_.end(), v3_copy.begin() + 1));
    EXPECT_EQ(std::distance(v3_.begin(), bitret), v3_copy.size() - 2);
}






