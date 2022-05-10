#include <benchmark/benchmark.h>
#include <random>
#include <math.h>
#include "bit_array.h"
#include <boost/dynamic_bitset.hpp>
#include "bitlib/bitlib.hpp"


auto BM_BitShiftLeft = [](benchmark::State& state, auto input) {
    using container_type = typename std::tuple_element<0, decltype(input)>::type;
    using word_type = typename std::tuple_element<1, decltype(input)>::type;
    unsigned int total_bits = std::get<2>(input);
    auto digits = bit::binary_digits<word_type>::value;
    auto container_size = ceil(float(total_bits) / digits);
    container_type bitcont = make_random_container<container_type>(container_size); 
    auto first = bit::bit_iterator<decltype(std::begin(bitcont))>(std::begin(bitcont));
    auto last = bit::bit_iterator<decltype(std::end(bitcont))>(std::end(bitcont));
    auto n = bit::distance(first, last) / 2;
    for (auto _ : state) {
        benchmark::DoNotOptimize(bit::shift_left(first, last, n));
        benchmark::ClobberMemory();
    }
};

auto BM_BoostShiftLeft = [](benchmark::State& state, auto input) {
    using container_type = typename std::tuple_element<0, decltype(input)>::type;
    using word_type = typename std::tuple_element<1, decltype(input)>::type;
    unsigned int total_bits = std::get<2>(input);
    boost::dynamic_bitset<> x(total_bits);
    for (auto i = 0; i < total_bits; ++i) {
        if (i % 3 == 0) {
            x[i] = 1;
        }
    }
    auto n = total_bits / 2;
    for (auto _ : state) {
        benchmark::DoNotOptimize(x <<= n);
        benchmark::ClobberMemory();
    }
};

auto BM_CBitArrShiftLeft = [](benchmark::State& state, auto input) {
    using container_type = typename std::tuple_element<0, decltype(input)>::type;
    using word_type = typename std::tuple_element<1, decltype(input)>::type;
    unsigned int total_bits = std::get<2>(input);
    BIT_ARRAY* bitarr = bit_array_create(total_bits);

    for (auto _ : state) {
        bit_array_shift_left(bitarr, total_bits/2, '0');
        benchmark::ClobberMemory();
    }
};

auto BM_BitShiftLeft_UU = [](benchmark::State& state, auto input) {
    using container_type = typename std::tuple_element<0, decltype(input)>::type;
    using word_type = typename std::tuple_element<1, decltype(input)>::type;
    using iterator_type = typename container_type::iterator;
    unsigned int total_bits = std::get<2>(input);
    auto digits = bit::binary_digits<word_type>::value;
    auto container_size = ceil(float(total_bits) / digits);
    container_type bitcont = make_random_container<container_type>(container_size); 
    bit::bit_iterator<iterator_type> first = bit::bit_iterator<iterator_type>(bitcont.begin()) + 1;
    bit::bit_iterator<iterator_type> last = bit::bit_iterator<iterator_type>(bitcont.end()) - 1;
    auto n = bit::distance(first, last) / 2 + 3;
    for (auto _ : state) {
        benchmark::DoNotOptimize(bit::shift_left(first, last, n));
        benchmark::ClobberMemory();
    }
};

auto BM_BoolShiftLeft = [](benchmark::State& state, auto input) {
    using container_type = std::vector<bool>;
    using num_type = typename container_type::value_type;
    unsigned int container_size = std::get<2>(input);
    container_type cont = make_random_container<container_type>(container_size); 
    auto first = cont.begin();
    auto last = cont.end();
    auto n = std::distance(first, last) / 2 + 3;
    for (auto _ : state) {
        benchmark::DoNotOptimize(std::shift_left(first, last, n));
        benchmark::ClobberMemory();
    }
};

auto BM_BitShiftRight = [](benchmark::State& state, auto input) {
    using container_type = typename std::tuple_element<0, decltype(input)>::type;
    using word_type = typename std::tuple_element<1, decltype(input)>::type;
    unsigned int total_bits = std::get<2>(input);
    auto digits = bit::binary_digits<word_type>::value;
    auto container_size = ceil(float(total_bits) / digits);
    container_type bitcont = make_random_container<container_type>(container_size); 
    auto first = bit::bit_iterator<decltype(std::begin(bitcont))>(std::begin(bitcont));
    auto last = bit::bit_iterator<decltype(std::end(bitcont))>(std::end(bitcont));
    auto n = bit::distance(first, last) / 2;
    for (auto _ : state) {
        benchmark::DoNotOptimize(bit::shift_right(first, last, n));
        benchmark::ClobberMemory();
    }
};

auto BM_BitShiftRight_UU = [](benchmark::State& state, auto input) {
    using container_type = typename std::tuple_element<0, decltype(input)>::type;
    using word_type = typename std::tuple_element<1, decltype(input)>::type;
    unsigned int total_bits = std::get<2>(input);
    auto digits = bit::binary_digits<word_type>::value;
    auto container_size = ceil(float(total_bits) / digits);
    container_type bitcont = make_random_container<container_type>(container_size); 
    auto first = bit::bit_iterator<decltype(std::begin(bitcont))>(std::begin(bitcont)) + 2;
    auto last = bit::bit_iterator<decltype(std::end(bitcont))>(std::end(bitcont)) - 3;
    auto n = bit::distance(first, last) / 2 + 3;
    for (auto _ : state) {
        benchmark::DoNotOptimize(bit::shift_right(first, last, n));
        benchmark::ClobberMemory();
    }
};

auto BM_BoolShiftRight = [](benchmark::State& state, auto input) {
    using container_type = std::vector<bool>;
    using num_type = typename container_type::value_type;
    unsigned int container_size = std::get<2>(input);
    container_type cont = make_random_container<container_type>(container_size); 
    auto first = cont.begin();
    auto last = cont.end();
    auto n = std::distance(first, last) / 2 + 3;
    for (auto _ : state) {
        benchmark::DoNotOptimize(std::shift_right(first, last, n));
        benchmark::ClobberMemory();
    }
};
