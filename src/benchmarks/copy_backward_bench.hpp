#include <benchmark/benchmark.h>
#include "test_utils.hpp"
#include "bitlib/bitlib.hpp"

auto BM_BitCopyBackward = [](benchmark::State& state, auto input) {
    using container_type = typename std::tuple_element<0, decltype(input)>::type;
    using WordType = typename std::tuple_element<1, decltype(input)>::type;
    unsigned int total_bits = std::get<2>(input);
    auto digits = bit::binary_digits<WordType>::value;
    auto container_size = total_bits / digits + 1;
    auto bitvec1 = get_random_vec<WordType>(container_size);
    auto first1 = bit::bit_iterator<decltype(std::begin(bitvec1))>(std::begin(bitvec1));
    auto bitvec2 = get_random_vec<WordType>(container_size);
    auto first2 = bit::bit_iterator<decltype(std::begin(bitvec2))>(std::begin(bitvec2));
    auto start1 = 3;
    auto start2 = 1;
    auto end1 = 4;
    long long n = total_bits - start1 - end1;

    for (auto _ : state)
        bit::copy_backward(
            first1 + start1,
            first1 + total_bits - end1,
            first2 + start2 + n
        );
};


auto BM_BoolCopyBackward = [](benchmark::State& state, auto input) {
    using container_type = typename std::tuple_element<0, decltype(input)>::type;
    unsigned int total_bits = std::get<2>(input);
    auto container_size = total_bits;
    container_type boolvec1 = make_random_container<container_type> (container_size); 
    container_type boolvec2 = make_random_container<container_type> (container_size); 
    auto first1 = boolvec1.begin();
    auto first2 = boolvec2.begin();

    auto start1 = 3;
    auto start2 = 1;
    auto end1 = 4;
    long long n = total_bits - start1 - end1;

    for (auto _ : state)
        std::copy_backward(
            first1 + start1,
            first1 + total_bits - end1,
            first2 + start2 + n
        );
};

