#include <benchmark/benchmark.h>
#include <math.h>
#include "bitlib/bit-algorithms/find.hpp"
#include "bit_array.h"
#include "sul/dynamic_bitset.hpp"

auto BM_BitFind = [](benchmark::State& state, auto input) {
    using container_type = typename std::tuple_element<0, decltype(input)>::type;
    using word_type = typename std::tuple_element<1, decltype(input)>::type;
    unsigned int total_bits = std::get<2>(input);
    auto digits = bit::binary_digits<word_type>::value;
    auto container_size = ceil(float(total_bits) / digits);
    container_type bitcont(container_size); 
    auto first = bit::bit_iterator<decltype(std::begin(bitcont))>(std::begin(bitcont));
    auto last = bit::bit_iterator<decltype(std::end(bitcont))>(std::end(bitcont));
    *(first + total_bits / 2 + 4) = bit::bit1;
    for (auto _ : state) {
        benchmark::DoNotOptimize(bit::find(first + 2, last - 3, bit::bit1));
        benchmark::ClobberMemory();
    }
};

auto BM_BitArrayFind = [](benchmark::State& state, auto input) {
    using container_type = typename std::tuple_element<0, decltype(input)>::type;
    using word_type = typename std::tuple_element<1, decltype(input)>::type;
    unsigned int total_bits = std::get<2>(input);
    BIT_ARRAY* bitarr = bit_array_create(total_bits);
    bit_array_set_bit(bitarr, total_bits/2 + 4);
    bit_index_t result;
    for (auto _ : state) {
        benchmark::DoNotOptimize(bit_array_find_first_set_bit(bitarr, &result));
        benchmark::ClobberMemory();
    }
    bit_array_free(bitarr);
};

auto BM_DynamicBitsetFind = [](benchmark::State& state, auto input) {
    using container_type = typename std::tuple_element<0, decltype(input)>::type;
    using word_type = typename std::tuple_element<1, decltype(input)>::type;
    using iterator_type = typename container_type::iterator;
    unsigned int total_bits = std::get<2>(input);
    sul::dynamic_bitset<> bitset1(total_bits, 0);
    bitset1[total_bits / 2 + 4] = 1;
    for (auto _ : state) {
        benchmark::DoNotOptimize(bitset1.find_first()); 
        benchmark::ClobberMemory();
    }
};

auto BM_BoolFind = [](benchmark::State& state, auto input) {
    using container_type = std::vector<bool>;
    using num_type = typename container_type::value_type;
    unsigned int container_size = std::get<2>(input);
    container_type cont(container_size); 
    cont[(cont.size() / 2) + 4] = true;
    auto first = cont.begin();
    auto last = cont.end();
    for (auto _ : state) {
        benchmark::DoNotOptimize(std::find(first + 2, last - 3, true));
        benchmark::ClobberMemory();
    }
};
