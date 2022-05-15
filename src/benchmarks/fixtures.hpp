// =============================== FIXTURES ================================= //
// Project:         The Experimental Bit Algorithms Library
// Description:     Fixtures for benchmarking 
// Creator:         Bryce Kille [2019]
// License:         BSD 3-Clause License
// ========================================================================== //
#ifndef _BENCH_FIXTURES_HPP_INCLUDED
#define _BENCH_FIXTURES_HPP_INCLUDED
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
#include <benchmark/benchmark.h>
#include "test_utils.hpp"
// Third-party libraries
// Miscellaneous
// ========================================================================== //


// This fixture should work for any bitcontainer type that supports:
// BitContainer<uint64_t>(num_bits)
// BitContainer<uint64_t>::push_back(bool)
template<class BitContainer>
class SingleDynamicContainerFixture: public benchmark::Fixture {

    protected:
    BitContainer random_vec1;
    BitContainer random_vec_sparse1;

    public:
    void SetUp(const ::benchmark::State& state) {
        auto num_bits = state.range(0);
        random_vec1 = make_random_binary_container<BitContainer>(num_bits);
        random_vec_sparse1 = make_random_binary_container<BitContainer>(num_bits, 0.05);
    }

    void TearDown(const ::benchmark::State& state) {}
};

template<class BitContainer>
class DoubleDynamicContainerFixture: public SingleDynamicContainerFixture<BitContainer> {

    protected:
    using bit_container_type = BitContainer;
    bit_container_type random_vec2;
    bit_container_type random_vec_sparse2;

    public:
    void SetUp(const ::benchmark::State& state) {
        SingleDynamicContainerFixture<BitContainer>::SetUp(state);
        auto num_bits = state.range(0);
        random_vec2 = make_random_binary_container<bit_container_type>(num_bits);
        random_vec_sparse2 = make_random_binary_container<bit_container_type>(num_bits, 0.05);
    }

    void TearDown(const ::benchmark::State& state) {}
};

// ========================================================================== //
#endif // _BENCH_FIXTURES_HPP_INCLUDED
// ========================================================================== //
