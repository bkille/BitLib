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
        BitLib_Reverse, 
        bit::bit_vector<uint64_t>) 
(benchmark::State& state) {
    auto total_bits = state.range(0); 
    int first_offset = state.range(1);
    int last_offset = state.range(2);
    auto first = this->random_vec1.begin() + first_offset;
    auto last = this->random_vec1.end() - last_offset;


    for (auto _ : state) {
        bit::reverse(first, last);
        benchmark::ClobberMemory();
    }
};

BENCHMARK_REGISTER_F(
        SingleDynamicContainerFixture, 
        BitLib_Reverse
)->ArgsProduct({
    benchmark::CreateRange(1 << 4, 1 << 24, /*multi=*/16),
    {0, 2}, 
    {0, 3}
});

//std::vector<bool>
BENCHMARK_TEMPLATE_DEFINE_F(
        SingleDynamicContainerFixture, 
        VectorBool_Reverse, 
        std::vector<bool>) 
(benchmark::State& state) {
    auto total_bits = state.range(0); 
    int first_offset = state.range(1);
    int last_offset = state.range(2);
    auto first = this->random_vec1.begin() + first_offset;
    auto last = this->random_vec1.end() - last_offset;


    for (auto _ : state) {
        std::reverse(first, last);
        benchmark::ClobberMemory();
    }
};

BENCHMARK_REGISTER_F(
        SingleDynamicContainerFixture, 
        VectorBool_Reverse
)->ArgsProduct({
    benchmark::CreateRange(1 << 4, 1 << 24, /*multi=*/16),
    {0, 2}, 
    {0, 3}
});

