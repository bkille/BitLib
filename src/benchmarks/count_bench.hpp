#include <benchmark/benchmark.h>
#include <math.h>
#include "test_utils.hpp"
#include "bit_array.h"
#include "bitlib/bit-algorithms/count.hpp"

auto BM_BitCount = [](benchmark::State& state, auto input) {
    using container_type = typename std::tuple_element<0, decltype(input)>::type;
    using word_type = typename std::tuple_element<1, decltype(input)>::type;
    unsigned int total_bits = std::get<2>(input);
    auto digits = bit::binary_digits<word_type>::value;
    auto container_size = ceil(float(total_bits) / digits);
    container_type bitcont = make_random_container<container_type>(container_size); 
    auto first = bit::bit_iterator<decltype(std::begin(bitcont))>(std::begin(bitcont));
    auto last = bit::bit_iterator<decltype(std::end(bitcont))>(std::end(bitcont));
    for (auto _ : state) {
        benchmark::DoNotOptimize(bit::count(first, last, bit::bit1));
        benchmark::ClobberMemory();
    }
};


auto BM_CBitArrCount = [](benchmark::State& state, auto input) {
    using container_type = typename std::tuple_element<0, decltype(input)>::type;
    using word_type = typename std::tuple_element<1, decltype(input)>::type;
    unsigned int total_bits = std::get<2>(input);
    BIT_ARRAY* bitarr = bit_array_create(total_bits);
    for (auto _ : state) {
        benchmark::DoNotOptimize(bit_array_num_bits_set(bitarr));
        benchmark::ClobberMemory();
    }
};


auto BM_BoolCount = [](benchmark::State& state, auto input) {
    using container_type = std::vector<bool>;
    using num_type = typename container_type::value_type;
    unsigned int container_size = std::get<2>(input);
    container_type cont = make_random_container<container_type>(container_size); 
    auto first = cont.begin();
    auto last = cont.end();
    for (auto _ : state) {
        benchmark::DoNotOptimize(std::count(first, last, true));
        benchmark::ClobberMemory();
    }
};
