#include <benchmark/benchmark.h>
#include "test_utils.hpp"
#include "sul/dynamic_bitset.hpp"
#include "bitlib/bitlib.hpp"

auto BM_BitSet = [](benchmark::State& state, auto input) {
    using container_type = typename std::tuple_element<0, decltype(input)>::type;
    using WordType = typename std::tuple_element<1, decltype(input)>::type;
    unsigned int total_bits = std::get<2>(input);
    auto digits = bit::binary_digits<WordType>::value;
    auto container_size = total_bits / digits + 1;
    auto bitvec1 = get_random_vec<WordType>(container_size);
    auto first1 = bit::bit_iterator<decltype(std::begin(bitvec1))>(std::begin(bitvec1));

    for (auto _ : state) {
        benchmark::DoNotOptimize(first1[total_bits/2] = bit::bit1);
        benchmark::ClobberMemory();
    }
};

auto BM_BitArraySet = [](benchmark::State& state, auto input) {
    using container_type = typename std::tuple_element<0, decltype(input)>::type;
    using WordType = typename std::tuple_element<1, decltype(input)>::type;
    unsigned int total_bits = std::get<2>(input);
    auto digits = bit::binary_digits<WordType>::value;
    BIT_ARRAY* bitarr = bit_array_create(total_bits);

    for (auto _ : state)
    {
        bit_array_set_bit(bitarr, total_bits/2);
        benchmark::ClobberMemory();
    }
};


auto BM_DynamicBitsetSet = [](benchmark::State& state, auto input) {
    using container_type = typename std::tuple_element<0, decltype(input)>::type;
    using WordType = typename std::tuple_element<1, decltype(input)>::type;
    unsigned int total_bits = std::get<2>(input);
    sul::dynamic_bitset<uint64_t> x(total_bits);
    container_type boolvec1 = make_random_container<container_type> (total_bits); 
    for (auto i = 0; i < total_bits; ++i) {
        x[i] = boolvec1[i];
    }

    for (auto _ : state) {
        (x[total_bits/2] = true);
        benchmark::ClobberMemory();
    }
};

auto BM_BoolSet = [](benchmark::State& state, auto input) {
    using container_type = typename std::tuple_element<0, decltype(input)>::type;
    unsigned int total_bits = std::get<2>(input);
    auto container_size = total_bits;
    container_type boolvec1 = make_random_container<container_type> (container_size); 

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(boolvec1[container_size/2] = true);
        benchmark::ClobberMemory();
    }
};


auto BM_BitGet = [](benchmark::State& state, auto input) {
    using container_type = typename std::tuple_element<0, decltype(input)>::type;
    using WordType = typename std::tuple_element<1, decltype(input)>::type;
    unsigned int total_bits = std::get<2>(input);
    auto digits = bit::binary_digits<WordType>::value;
    auto container_size = total_bits / digits + 1;
    auto bitvec1 = get_random_vec<WordType>(container_size);
    auto first1 = bit::bit_iterator<decltype(std::begin(bitvec1))>(std::begin(bitvec1));

    for (auto _ : state) {
        benchmark::DoNotOptimize(first1[total_bits/2]);
        benchmark::ClobberMemory();
    }
};

auto BM_BitArrayGet = [](benchmark::State& state, auto input) {
    using container_type = typename std::tuple_element<0, decltype(input)>::type;
    using WordType = typename std::tuple_element<1, decltype(input)>::type;
    unsigned int total_bits = std::get<2>(input);
    auto digits = bit::binary_digits<WordType>::value;
    BIT_ARRAY* bitarr = bit_array_create(total_bits);

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(bit_array_get_bit(bitarr, total_bits/2));
        benchmark::ClobberMemory();
    }
};


auto BM_DynamicBitsetGet = [](benchmark::State& state, auto input) {
    using container_type = typename std::tuple_element<0, decltype(input)>::type;
    using WordType = typename std::tuple_element<1, decltype(input)>::type;
    unsigned int total_bits = std::get<2>(input);
    sul::dynamic_bitset<uint64_t> x(total_bits);
    container_type boolvec1 = make_random_container<container_type> (total_bits); 
    for (auto i = 0; i < total_bits; ++i) {
        x[i] = boolvec1[i];
    }

    for (auto _ : state) {
        benchmark::DoNotOptimize(x[total_bits/2]);
        benchmark::ClobberMemory();
    }
};

auto BM_BoolGet = [](benchmark::State& state, auto input) {
    using container_type = typename std::tuple_element<0, decltype(input)>::type;
    unsigned int total_bits = std::get<2>(input);
    auto container_size = total_bits;
    container_type boolvec1 = make_random_container<container_type> (container_size); 
    bool x;
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(x = boolvec1[container_size/2]);
        benchmark::ClobberMemory();
    }
};
