#include <benchmark/benchmark.h>
#include <random>
#include <math.h>
#include "fixtures.hpp"
#include "bit_array.h"
#include <boost/dynamic_bitset.hpp>
#include "bitlib/bitlib.hpp"
#include "sul/dynamic_bitset.hpp"


// BitLib
BENCHMARK_TEMPLATE_DEFINE_F(
        SingleDynamicContainerFixture, 
        BitLib_Not, 
        bit::bit_vector<uint64_t>) 
(benchmark::State& state) {
    auto total_bits = state.range(0); 
    auto first = this->random_vec1.begin();
    auto last = this->random_vec1.end();
    auto n = bit::distance(first, last) / 2 + 4;

    //TODO bit_not word_type should be templated
    for (auto _ : state) {
        benchmark::DoNotOptimize(
                bit::transform(
                    first, 
                    last, 
                    first,
                    std::bit_not<uint64_t>()));
        benchmark::ClobberMemory();
    }
};

BENCHMARK_REGISTER_F(
        SingleDynamicContainerFixture, 
        BitLib_Not
)->RangeMultiplier(8)->Range(1<<8, 1<<23);

// Boost
BENCHMARK_TEMPLATE_DEFINE_F(
        SingleDynamicContainerFixture, 
        Boost_Not, 
        boost::dynamic_bitset<uint64_t>) 
(benchmark::State& state) {
    auto total_bits = state.range(0); 
    auto n = total_bits / 2 + 4;
    for (auto _ : state) {
        benchmark::DoNotOptimize(this->random_vec1.flip());
        benchmark::ClobberMemory();
    }
};

BENCHMARK_REGISTER_F(
        SingleDynamicContainerFixture, 
        Boost_Not
)->RangeMultiplier(8)->Range(1<<8, 1<<23);


// Sul
BENCHMARK_TEMPLATE_DEFINE_F(
        SingleDynamicContainerFixture, 
        Sul_Not, 
        sul::dynamic_bitset<uint64_t>) 
(benchmark::State& state) {
    auto total_bits = state.range(0); 
    auto n = total_bits / 2 + 4;
    for (auto _ : state) {
        benchmark::DoNotOptimize(this->random_vec1.flip());
        benchmark::ClobberMemory();
    }
};

BENCHMARK_REGISTER_F(
        SingleDynamicContainerFixture, 
        Sul_Not
)->RangeMultiplier(8)->Range(1<<8, 1<<23);

