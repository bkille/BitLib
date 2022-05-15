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
        BitLib_Count, 
        bit::bit_vector<uint64_t>) 
(benchmark::State& state) {
    auto total_bits = state.range(0); 
    int first_offset = state.range(1);
    int last_offset = state.range(2);
    auto first = this->random_vec1.begin() + first_offset;
    auto last = this->random_vec1.end() - last_offset;
    auto n_first = this->random_vec1.begin() + bit::distance(first, last) / 2;

    for (auto _ : state) {
        benchmark::DoNotOptimize(bit::count(first, last, bit::bit1));
        benchmark::ClobberMemory();
    }
};

BENCHMARK_REGISTER_F(
        SingleDynamicContainerFixture, 
        BitLib_Count
)->ArgsProduct({
    benchmark::CreateRange(1 << 4, 1 << 24, /*multi=*/16),
    {0, 2}, 
    {0, 3}
});

// sul
BENCHMARK_TEMPLATE_DEFINE_F(
        SingleDynamicContainerFixture, 
        Sul_Count, 
        sul::dynamic_bitset<uint64_t>) 
(benchmark::State& state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(this->random_vec1.count());
        benchmark::ClobberMemory();
    }
};

BENCHMARK_REGISTER_F(
        SingleDynamicContainerFixture, 
        Sul_Count
)->ArgsProduct({
    benchmark::CreateRange(1 << 4, 1 << 24, /*multi=*/16)
});


// boost
BENCHMARK_TEMPLATE_DEFINE_F(
        SingleDynamicContainerFixture, 
        Boost_Count, 
        boost::dynamic_bitset<uint64_t>) 
(benchmark::State& state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(this->random_vec1.count());
        benchmark::ClobberMemory();
    }
};

BENCHMARK_REGISTER_F(
        SingleDynamicContainerFixture, 
        Boost_Count
)->ArgsProduct({
    benchmark::CreateRange(1 << 4, 1 << 24, /*multi=*/16)
});

//std::vector<bool>
BENCHMARK_TEMPLATE_DEFINE_F(
        SingleDynamicContainerFixture, 
        VectorBool_Count, 
        std::vector<bool>) 
(benchmark::State& state) {
    auto total_bits = state.range(0); 
    int first_offset = state.range(1);
    int last_offset = state.range(2);
    auto first = this->random_vec1.begin() + first_offset;
    auto last = this->random_vec1.end() - last_offset;
    auto n_first = this->random_vec1.begin() + std::distance(first, last) / 2;

    for (auto _ : state) {
        benchmark::DoNotOptimize(std::count(first, last, true));
        benchmark::ClobberMemory();
    }
};

BENCHMARK_REGISTER_F(
        SingleDynamicContainerFixture, 
        VectorBool_Count
)->ArgsProduct({
    benchmark::CreateRange(1 << 4, 1 << 24, /*multi=*/16)
});
