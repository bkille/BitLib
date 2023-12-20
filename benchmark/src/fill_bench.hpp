#include <benchmark/benchmark.h>
#include <math.h>
#include "bitlib/bit-algorithms/fill.hpp"
#include "bit_array.h"
#include "sul/dynamic_bitset.hpp"

auto BM_BitFill = [](benchmark::State& state, auto input) {
    using container_type = typename std::tuple_element<0, decltype(input)>::type;
    using word_type = typename std::tuple_element<1, decltype(input)>::type;
    unsigned int total_bits = std::get<2>(input);
    auto digits = bit::binary_digits<word_type>::value;
    auto container_size = ceil(float(total_bits) / digits);
    container_type bitcont = make_random_container<container_type>(container_size); 
    auto first = bit::bit_iterator<decltype(std::begin(bitcont))>(std::begin(bitcont));
    auto last = bit::bit_iterator<decltype(std::end(bitcont))>(std::end(bitcont));
    for (auto _ : state) {
        bit::fill(first + 2, last - 3, bit::bit1);
        benchmark::ClobberMemory();
    }
};

auto BM_BitArrayFill = [](benchmark::State& state, auto input) {
    using container_type = typename std::tuple_element<0, decltype(input)>::type;
    using word_type = typename std::tuple_element<1, decltype(input)>::type;
    unsigned int total_bits = std::get<2>(input);
    BIT_ARRAY* bitarr = bit_array_create(total_bits);
    for (auto _ : state) {
        bit_array_set_region(bitarr, 2, total_bits - 5);
        benchmark::ClobberMemory();
    }
    bit_array_free(bitarr);
};

auto BM_DynamicBitsetFill = [](benchmark::State& state, auto input) {
    using container_type = typename std::tuple_element<0, decltype(input)>::type;
    using word_type = typename std::tuple_element<1, decltype(input)>::type;
    using iterator_type = typename container_type::iterator;
    unsigned int total_bits = std::get<2>(input);
    sul::dynamic_bitset<> bitset1(total_bits, 0);
    for (auto _ : state) {
        bitset1.set(2, total_bits - 5, true);
        benchmark::ClobberMemory();
    }
};

auto BM_BoolFill = [](benchmark::State& state, auto input) {
    using container_type = std::vector<bool>;
    using num_type = typename container_type::value_type;
    unsigned int container_size = std::get<2>(input);
    container_type cont = make_random_container<container_type>(container_size); 
    auto first = cont.begin();
    auto last = cont.end();
    for (auto _ : state) {
        std::fill(first + 2, last - 3, true);
        benchmark::ClobberMemory();
    }
};
