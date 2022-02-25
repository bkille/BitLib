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

// Tests the default c'tor.
TEST_F(VectorTest, DefaultConstructor) {
    EXPECT_EQ(0u, v1_.size());
}

// Tests the size c'tor.
TEST_F(VectorTest, SizeInitializerConstructor) {
    EXPECT_EQ(256, v2_.size());
}

// Tests the size c'tor.
TEST_F(VectorTest, SringConstructor) {
    EXPECT_EQ(9, v3_.size());
    EXPECT_EQ(false, static_cast<bool>(v3_[0]));
    EXPECT_EQ(true, static_cast<bool>(v3_[8]));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
