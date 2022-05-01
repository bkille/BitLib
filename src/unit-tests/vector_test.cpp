// =============================== FIXTURES ================================= //
// Project:         The Experimental Bit Algorithms Library
// Description:     Fixtures for testing 
// Creator:         Bryce Kille [2019]
// License:         BSD 3-Clause License
// ========================================================================== //


// ============================== PREAMBLE ================================== //
// C++ standard library
#include <math.h>
#include <type_traits>
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
    for (auto bv: this->v2_) {
        EXPECT_FALSE(bv);
    }
    // hack to get the word type, since for some reason I'm not allowed to use this->base_type
    using base_type = typename std::remove_reference<decltype(*(this->empty_vec.begin().base()))>::type;
    for (unsigned int veclen = 0; veclen < 4*this->digits; veclen++) {
        this->empty_vec = decltype(this->empty_vec)(veclen);
        for (unsigned int i = 0; i < veclen; ++i) {
            EXPECT_FALSE(this->empty_vec[i]);
        }
        this->empty_vec = bit::bit_vector<base_type>(veclen);
        unsigned int i = 0;
        // TODO misplaced test for range-based for loop
        for (auto bv: this->empty_vec) {
            EXPECT_FALSE(bv);
            i++; 
        }
        EXPECT_EQ(i, veclen);
    }
}

// Tests the string c'tor.
TYPED_TEST(VectorTest, StringConstructor) {
    EXPECT_EQ(9, this->v3_.size());
    EXPECT_EQ(false, static_cast<bool>(this->v3_[0]));
    EXPECT_EQ(true, static_cast<bool>(this->v3_[8]));
    using base_type = typename std::remove_reference<decltype(*(this->empty_vec.begin().base()))>::type;
    for (unsigned int strlen = 0; strlen < 4*this->digits; strlen++) {
        std::string rand_bs(strlen, 0);
        this->empty_vec_bool.clear();
        for (auto& pos: rand_bs) {
            pos = generate_random_number('0', '1');
            this->empty_vec_bool.push_back(pos == '1');
        }
        this->empty_vec = bit::bit_vector<base_type>(rand_bs);
        EXPECT_TRUE(std::equal(
                    this->empty_vec.begin(), 
                    this->empty_vec.end(), 
                    this->empty_vec_bool.begin(), 
                    this->empty_vec_bool.end(), 
                    comparator));
    }
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
    for (unsigned int vec_idx = 0; vec_idx < this->random_bitvecs.size(); ++vec_idx) {
        auto& bitvec = this->random_bitvecs[vec_idx];
        auto& boolvec = this->random_boolvecs[vec_idx];
        this->empty_vec = bitvec;
        EXPECT_NE(this->empty_vec.begin().base(), bitvec.begin().base());
        EXPECT_TRUE(std::equal(
                    this->empty_vec.begin(), 
                    this->empty_vec.end(), 
                    boolvec.begin(), 
                    boolvec.end(), 
                    comparator));
        if (bitvec.size() > 0) {
            bitvec[0] = ~bitvec[0];
            EXPECT_NE(bitvec[0], this->empty_vec[0]);
        }
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
    for (unsigned int vec_idx = 0; vec_idx < this->random_bitvecs.size(); ++vec_idx) {
        auto& bitvec = this->random_bitvecs[vec_idx];
        auto& boolvec = this->random_boolvecs[vec_idx];
        auto bitvec_base = bitvec.begin().base();
        this->empty_vec = std::move(bitvec);
        EXPECT_EQ(this->empty_vec.begin().base(), bitvec_base);
        EXPECT_TRUE(std::equal(
                    this->empty_vec.begin(), 
                    this->empty_vec.end(), 
                    boolvec.begin(), 
                    boolvec.end(), 
                    comparator));
    }
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
TYPED_TEST(VectorTest, EraseAtBegin1) {
    // First signature
    for (auto _ = 16; _--;) {
        auto vec_idx = generate_random_number(0, this->random_bitvecs.size() - 1);
        auto& bitvec = this->random_bitvecs[vec_idx];
        auto& boolvec = this->random_boolvecs[vec_idx];
        if (boolvec.size() == 0) { continue; }
        auto bitret = bitvec.erase(bitvec.begin());
        auto boolret = boolvec.erase(boolvec.begin());
        EXPECT_TRUE(std::equal(
                    bitvec.begin(), 
                    bitvec.end(), 
                    boolvec.begin(), 
                    boolvec.end(), 
                    comparator));
        EXPECT_EQ(
                std::distance(bitvec.begin(), bitret), 
                std::distance(boolvec.begin(), boolret));
    }
}

TYPED_TEST(VectorTest, EraseAtEnd1) {
    // First signature
    for (auto _ = 16; _--;) {
        auto vec_idx = generate_random_number(0, this->random_bitvecs.size() - 1);
        auto& bitvec = this->random_bitvecs[vec_idx];
        auto& boolvec = this->random_boolvecs[vec_idx];
        if (boolvec.size() == 0) { continue; }
        auto bitret = bitvec.erase(bitvec.end() - 1);
        auto boolret = boolvec.erase(boolvec.end() - 1);
        EXPECT_TRUE(std::equal(
                    bitvec.begin(), 
                    bitvec.end(), 
                    boolvec.begin(), 
                    boolvec.end(), 
                    comparator));
        EXPECT_EQ(
                std::distance(bitvec.begin(), bitret), 
                std::distance(boolvec.begin(), boolret));
    }
}

TYPED_TEST(VectorTest, EraseAtRand1) {
    // First signature
    for (auto _ = 16; _--;) {
        auto vec_idx = generate_random_number(0, this->random_bitvecs.size() - 1);
        auto& bitvec = this->random_bitvecs[vec_idx];
        auto& boolvec = this->random_boolvecs[vec_idx];
        if (boolvec.size() == 0) { continue; }
        auto erase_location = generate_random_number(0, boolvec.size() - 1);
        auto bitret = bitvec.erase(bitvec.begin() + erase_location);
        auto boolret = boolvec.erase(boolvec.begin() + erase_location);
        EXPECT_TRUE(std::equal(
                    bitvec.begin(), 
                    bitvec.end(), 
                    boolvec.begin(), 
                    boolvec.end(), 
                    comparator));
        EXPECT_EQ(
                std::distance(bitvec.begin(), bitret), 
                std::distance(boolvec.begin(), boolret));
    }
}

TYPED_TEST(VectorTest, EraseAtBegin2) {
    // Second signature
    for (auto _ = 16; _--;) {
        auto vec_idx = generate_random_number(0, this->random_bitvecs.size() - 1);
        auto& bitvec = this->random_bitvecs[vec_idx];
        auto& boolvec = this->random_boolvecs[vec_idx];
        if (boolvec.size() == 0) { continue; }
        auto erase_size = generate_random_number(0, boolvec.size() - 1);
        auto bitret = bitvec.erase(bitvec.begin(), bitvec.begin() + erase_size);
        auto boolret = boolvec.erase(boolvec.begin(), boolvec.begin() + erase_size);
        EXPECT_TRUE(std::equal(
                    bitvec.begin(), 
                    bitvec.end(), 
                    boolvec.begin(), 
                    boolvec.end(), 
                    comparator));
        EXPECT_EQ(
                std::distance(bitvec.begin(), bitret), 
                std::distance(boolvec.begin(), boolret));
    }
}

TYPED_TEST(VectorTest, EraseAtEnd2) {
    // Second signature
    for (auto _ = 16; _--;) {
        auto vec_idx = generate_random_number(0, this->random_bitvecs.size() - 1);
        auto& bitvec = this->random_bitvecs[vec_idx];
        auto& boolvec = this->random_boolvecs[vec_idx];
        if (boolvec.size() == 0) { continue; }
        auto erase_start = generate_random_number(0, boolvec.size() - 1);
        auto bitret = bitvec.erase(bitvec.begin() + erase_start, bitvec.end());
        auto boolret = boolvec.erase(boolvec.begin() + erase_start, boolvec.end());
        EXPECT_TRUE(std::equal(
                    bitvec.begin(), 
                    bitvec.end(), 
                    boolvec.begin(), 
                    boolvec.end(), 
                    comparator));
        EXPECT_EQ(
                std::distance(bitvec.begin(), bitret), 
                std::distance(boolvec.begin(), boolret));
    }
}

TYPED_TEST(VectorTest, EraseAtRand2) {
    // Second signature
    for (auto _ = 16; _--;) {
        auto vec_idx = generate_random_number(0, this->random_bitvecs.size() - 1);
        auto& bitvec = this->random_bitvecs[vec_idx];
        auto& boolvec = this->random_boolvecs[vec_idx];
        if (boolvec.size() == 0) { continue; }
        auto erase_start = generate_random_number(0, boolvec.size() - 1);
        auto erase_end = generate_random_number(erase_start, boolvec.size());
        auto bitret = bitvec.erase(bitvec.begin() + erase_start, bitvec.begin() + erase_end);
        auto boolret = boolvec.erase(boolvec.begin() + erase_start, boolvec.begin() + erase_end);
        EXPECT_TRUE(std::equal(
                    bitvec.begin(), 
                    bitvec.end(), 
                    boolvec.begin(), 
                    boolvec.end(), 
                    comparator));
        EXPECT_EQ(
                std::distance(bitvec.begin(), bitret), 
                std::distance(boolvec.begin(), boolret));
    }
}

// Test push_back
TYPED_TEST(VectorTest, PushBack) {
    // First signature
    for (unsigned int vec_idx = 0; vec_idx < this->random_bitvecs.size(); ++vec_idx) {
        for (auto _ = 4*this->digits; _ > 0; _--) {
            auto& bitvec = this->random_bitvecs[vec_idx];
            auto& boolvec = this->random_boolvecs[vec_idx];
            bool to_insert_bool = generate_random_number(0, 1) > 0 ? true : false;
            bit::bit_value to_insert_bit = to_insert_bool ? bit::bit1 : bit::bit0;
            bitvec.push_back(to_insert_bit);
            boolvec.push_back(to_insert_bool);
            EXPECT_TRUE(std::equal(
                        bitvec.begin(), 
                        bitvec.end(), 
                        boolvec.begin(), 
                        boolvec.end(), 
                        comparator));
        }
    }
}

// Test pop_back
TYPED_TEST(VectorTest, PopBack) {
    // First signature
    for (unsigned int vec_idx = 0; vec_idx < this->random_bitvecs.size(); ++vec_idx) {
        auto& bitvec = this->random_bitvecs[vec_idx];
        auto& boolvec = this->random_boolvecs[vec_idx];
        for (auto _ = std::min(boolvec.size(), 4*this->digits); _>0; _--) {
            bitvec.pop_back();
            boolvec.pop_back();
            EXPECT_TRUE(std::equal(
                        bitvec.begin(), 
                        bitvec.end(), 
                        boolvec.begin(), 
                        boolvec.end(), 
                        comparator));
        }
    }
}


//TYPED_TEST(VectorTest, Print) {
    //std::cout << this->v3_ << std::endl;
//}
