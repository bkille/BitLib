// =============================== FIXTURES ================================= //
// Project:         The Experimental Bit Algorithms Library
// Description:     Fixtures for benchmarking 
// Creator:         Bryce Kille [2019]
// License:         BSD 3-Clause License
// ========================================================================== //
#ifndef _FIXTURES_HPP_INCLUDED
#define _FIXTURES_HPP_INCLUDED
// ========================================================================== //


// ============================== PREAMBLE ================================== //
// C++ standard library
#include <vector>
#include <list>
#include <forward_list>
#include <set>
#include <random>
#include <limits>
#include <cstring>
// Project sources
#include "bitlib/bit-iterator/bit_iterator.hpp"
#include "bitlib/bit-containers/bit-containers.hpp"
#include "test_utils.hpp"
// Third-party libraries
// Miscellaneous
// ========================================================================== //


// This fixture should work for any bitcontainer type that supports:
// BitContainer<uint64_t>(num_bits)
// BitContainer<uint64_t>::push_back(bool)
template<template <typename, typename> BitContainer>
class DynamicContainerFixture: public benchmark::Fixture {

    protected:
    using word_type = uint64_t;
    using bit_container_type = BitContainer;
    using word_container_type = std::vector<word_type>;
    auto num_containers = 3;

    public:
    void SetUp(const ::benchmark::State& state) {
        auto rand_float_01_gen = std::bind(
                std::uniform_real_distribution<>(0,1),
                std::default_random_engine());
        auto num_bits = state.range(0);
        std::vector<bit_container_type> random_vecs;
        std::vector<bit_container_type> random_vecs_sparse;
        for (auto _ = 0; _ < num_containers; ++_) {
            bit_container_type bc(num_bits);
            random_vecs.push_back(bc);
            bit_container_type bc_sparse(num_bits);
            random_vecs.push_back(bc_sparse);
            for (auto i = 0; i < num_bits; ++i) {
                random_vecs.back()[i] = gen() > 0.5;
                random_vecs_sparse.back()[i] = gen() > 0.05;
            }
        }

    void TearDown(const ::benchmark::State& state) {
    }
};

// ========================================================================== //
#endif // _FIXTURES_HPP_INCLUDED
// ========================================================================== //
