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
        DoubleDynamicContainerFixture, 
        BitLib_Copy, 
        bit::bit_vector<uint64_t>) 
(benchmark::State& state) {
    auto& x = this->random_vec1;
    auto& y = this->random_vec2;

    //TODO bit_and word_type should be templated
    for (auto _ : state) {
        benchmark::DoNotOptimize(x = y);
        benchmark::ClobberMemory();
    }
};

BENCHMARK_REGISTER_F(
        DoubleDynamicContainerFixture, 
        BitLib_Copy
)->RangeMultiplier(8)->Range(1<<8, 1<<23);

// Boost
BENCHMARK_TEMPLATE_DEFINE_F(
        DoubleDynamicContainerFixture, 
        Boost_Copy, 
        boost::dynamic_bitset<uint64_t>) 
(benchmark::State& state) {
    auto& x = this->random_vec1;
    auto& y = this->random_vec2;
    for (auto _ : state) {
        benchmark::DoNotOptimize(x = y);
        benchmark::ClobberMemory();
    }
};

BENCHMARK_REGISTER_F(
        DoubleDynamicContainerFixture, 
        Boost_Copy
)->RangeMultiplier(8)->Range(1<<8, 1<<23);


// Sul
BENCHMARK_TEMPLATE_DEFINE_F(
        DoubleDynamicContainerFixture, 
        Sul_Copy, 
        sul::dynamic_bitset<uint64_t>) 
(benchmark::State& state) {
    auto& x = this->random_vec1;
    auto& y = this->random_vec2;
    for (auto _ : state) {
        benchmark::DoNotOptimize(x = y);
        benchmark::ClobberMemory();
    }
};

BENCHMARK_REGISTER_F(
        DoubleDynamicContainerFixture, 
        Sul_Copy
)->RangeMultiplier(8)->Range(1<<8, 1<<23);

