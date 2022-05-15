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
        BitLib_ShiftLeft, 
        bit::bit_vector<uint64_t>) 
(benchmark::State& state) {
    auto total_bits = state.range(0); 
    auto first = this->random_vec1.begin();
    auto last = this->random_vec1.end();
    auto n = bit::distance(first, last) / 2 + 4;

    for (auto _ : state) {
        benchmark::DoNotOptimize(bit::shift_left(first, last, n));
        benchmark::ClobberMemory();
    }
};

BENCHMARK_REGISTER_F(
        SingleDynamicContainerFixture, 
        BitLib_ShiftLeft
)->RangeMultiplier(8)->Range(1<<8, 1<<23);

// Boost
BENCHMARK_TEMPLATE_DEFINE_F(
        SingleDynamicContainerFixture, 
        Boost_ShiftLeft, 
        boost::dynamic_bitset<uint64_t>) 
(benchmark::State& state) {
    auto total_bits = state.range(0); 
    auto n = total_bits / 2 + 4;
    for (auto _ : state) {
        benchmark::DoNotOptimize(this->random_vec1 <<= n);
        benchmark::ClobberMemory();
    }
};

BENCHMARK_REGISTER_F(
        SingleDynamicContainerFixture, 
        Boost_ShiftLeft
)->RangeMultiplier(8)->Range(1<<8, 1<<23);


// Sul
BENCHMARK_TEMPLATE_DEFINE_F(
        SingleDynamicContainerFixture, 
        Sul_ShiftLeft, 
        sul::dynamic_bitset<uint64_t>) 
(benchmark::State& state) {
    auto total_bits = state.range(0); 
    auto n = total_bits / 2 + 4;
    for (auto _ : state) {
        benchmark::DoNotOptimize(this->random_vec1 <<= n);
        benchmark::ClobberMemory();
    }
};

BENCHMARK_REGISTER_F(
        SingleDynamicContainerFixture, 
        Sul_ShiftLeft
)->RangeMultiplier(8)->Range(1<<8, 1<<23);

