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
        BitLib_And, 
        bit::bit_vector<uint64_t>) 
(benchmark::State& state) {
    auto first = this->random_vec1.begin();
    auto last = this->random_vec1.end();
    auto first2 = this->random_vec2.begin();

    //TODO bit_and word_type should be templated
    for (auto _ : state) {
        benchmark::DoNotOptimize(this->random_vec1 &= this->random_vec2);
        //benchmark::DoNotOptimize(
                //bit::transform(
                    //first, 
                    //last, 
                    //first2,
                    //first,
                    //std::bit_and<uint64_t>()));
        benchmark::ClobberMemory();
    }
};

BENCHMARK_REGISTER_F(
        DoubleDynamicContainerFixture, 
        BitLib_And
)->RangeMultiplier(8)->Range(1<<8, 1<<23);

// Boost
BENCHMARK_TEMPLATE_DEFINE_F(
        DoubleDynamicContainerFixture, 
        Boost_And, 
        boost::dynamic_bitset<uint64_t>) 
(benchmark::State& state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(this->random_vec1 &= this->random_vec2);
        benchmark::ClobberMemory();
    }
};

BENCHMARK_REGISTER_F(
        DoubleDynamicContainerFixture, 
        Boost_And
)->RangeMultiplier(8)->Range(1<<8, 1<<23);


// Sul
BENCHMARK_TEMPLATE_DEFINE_F(
        DoubleDynamicContainerFixture, 
        Sul_And, 
        sul::dynamic_bitset<uint64_t>) 
(benchmark::State& state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(this->random_vec1 &= this->random_vec2);
        benchmark::ClobberMemory();
    }
};

BENCHMARK_REGISTER_F(
        DoubleDynamicContainerFixture, 
        Sul_And
)->RangeMultiplier(8)->Range(1<<8, 1<<23);

