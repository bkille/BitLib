#include <benchmark/benchmark.h>
#include <random>
#include <math.h>
#include "bit_array.h"
#include "bitlib/bit-algorithms/rotate.hpp"

auto BM_BitRotateAUA = [](benchmark::State& state, auto input) {
    using container_type = typename std::tuple_element<0, decltype(input)>::type;
    using word_type = typename std::tuple_element<1, decltype(input)>::type;
    unsigned int total_bits = std::get<2>(input);
    auto digits = bit::binary_digits<word_type>::value;
    auto container_size = ceil(float(total_bits) / digits);
    container_type bitcont = make_random_container<container_type>(container_size); 
    auto first = bit::bit_iterator<decltype(std::begin(bitcont))>(std::begin(bitcont));
    auto n_first = first + total_bits/2 - 3;
    auto last = bit::bit_iterator<decltype(std::end(bitcont))>(std::end(bitcont));
    for (auto _ : state) {
        benchmark::DoNotOptimize(bit::rotate(first, n_first, last));
    }
};

auto BM_CBitArrRotate = [](benchmark::State& state, auto input) {
    using container_type = typename std::tuple_element<0, decltype(input)>::type;
    using word_type = typename std::tuple_element<1, decltype(input)>::type;
    unsigned int total_bits = std::get<2>(input);
    const int range_from  = 0;
    const int range_to    = total_bits - 1;
    std::random_device                  rand_dev;
    std::mt19937                        generator(rand_dev());
    std::uniform_int_distribution<int>  distr(range_from, range_to);
    auto n  = total_bits/2 - 3;
    BIT_ARRAY* bitarr = bit_array_create(total_bits);
    for (auto _ : state) {
        bit_array_cycle_left( bitarr, n);
    }
};

auto BM_BoolRotate = [](benchmark::State& state, auto input) {
    using container_type = std::vector<bool>;
    using num_type = typename container_type::value_type;
    unsigned int container_size = std::get<2>(input);
    container_type cont = make_random_container<container_type>(container_size); 
    auto first = cont.begin();
    auto n_first = first + container_size/2 - 3;
    auto last = cont.end();
    for (auto _ : state) {
        benchmark::DoNotOptimize(std::rotate(first, n_first, last));
    }
};
