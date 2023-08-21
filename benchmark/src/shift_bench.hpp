#include <benchmark/benchmark.h>
#include <random>
#include <memory>
#include <math.h>
#include "bitlib/bitlib.hpp"
#include "bit_array.h"
#include <sul/dynamic_bitset.hpp>


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
    auto n = bit::distance(first, last) / 2 + 6;
    for (auto _ : state) {
        benchmark::DoNotOptimize(bit::shift_left(first, last, n));
        benchmark::ClobberMemory();
    }
};

auto BM_BitArrayShiftLeft = [](benchmark::State& state, auto input) {
    using container_type = typename std::tuple_element<0, decltype(input)>::type;
    using word_type = typename std::tuple_element<1, decltype(input)>::type;
    unsigned int total_bits = std::get<2>(input);
    BIT_ARRAY* bitarr = bit_array_create(total_bits);
    auto n = total_bits / 2;
    for (auto _ : state) {
        bit_array_shift_right(bitarr, n, 0);
        benchmark::ClobberMemory();
    }
    bit_array_free(bitarr);
};


auto BM_DynamicBitsetShiftLeft = [](benchmark::State& state, auto input) {
    using container_type = typename std::tuple_element<0, decltype(input)>::type;
    using word_type = typename std::tuple_element<1, decltype(input)>::type;
    using iterator_type = typename container_type::iterator;
    unsigned int total_bits = std::get<2>(input);
    sul::dynamic_bitset<> bitset1(total_bits, 1);
    auto n = total_bits / 2;
    for (auto _ : state) {
        bitset1 <<= n;
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
    auto n = std::distance(first, last) / 2 + 6;
    for (auto _ : state) {
        benchmark::DoNotOptimize(bit::word_shift_left(first, last, n));
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
    auto n = bit::distance(first, last) / 2 + 6;
    for (auto _ : state) {
        benchmark::DoNotOptimize(bit::shift_right(first, last, n));
        benchmark::ClobberMemory();
    }
};

auto BM_DynamicBitsetShiftRight = [](benchmark::State& state, auto input) {
    using container_type = typename std::tuple_element<0, decltype(input)>::type;
    using word_type = typename std::tuple_element<1, decltype(input)>::type;
    using iterator_type = typename container_type::iterator;
    unsigned int total_bits = std::get<2>(input);
    sul::dynamic_bitset<> bitset1(total_bits, 1);
    auto n = total_bits / 2;
    for (auto _ : state) {
        bitset1 >>= n;
        benchmark::ClobberMemory();
    }
};

auto BM_BitArrayShiftRight = [](benchmark::State& state, auto input) {
    using container_type = typename std::tuple_element<0, decltype(input)>::type;
    using word_type = typename std::tuple_element<1, decltype(input)>::type;
    unsigned int total_bits = std::get<2>(input);
    BIT_ARRAY* bitarr = bit_array_create(total_bits);
    auto n = total_bits / 2;
    for (auto _ : state) {
        bit_array_shift_right(bitarr, n, 0);
        benchmark::ClobberMemory();
    }
    bit_array_free(bitarr);
};

auto BM_BoolShiftRight = [](benchmark::State& state, auto input) {
    using container_type = std::vector<bool>;
    using num_type = typename container_type::value_type;
    unsigned int container_size = std::get<2>(input);
    container_type cont = make_random_container<container_type>(container_size); 
    auto first = cont.begin();
    auto last = cont.end();
    auto n = std::distance(first, last) / 2 + 6;
    for (auto _ : state) {
        benchmark::DoNotOptimize(bit::word_shift_right(first, last, n));
        benchmark::ClobberMemory();
    }
};
