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
    unsigned long long start1 = generate_random_number(0, std::min<unsigned long long>(total_bits - 1, 16));
    unsigned long long start2 = generate_random_number(0, std::min<unsigned long long>(total_bits - 1, 16));
    auto end1 = generate_random_number(std::max<long long>(0, start2 - start1), total_bits - start1);
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

    unsigned long long start1 = generate_random_number(0, std::min<unsigned long long>(boolvec1.size() - 1, 16));
    unsigned long long start2 = generate_random_number(0, std::min<unsigned long long>(boolvec2.size() - 1, 16));
    auto end1 = generate_random_number(std::max<long long>(0, start2 - start1), total_bits - start1);
    long long n = total_bits - start1 - end1;

    for (auto _ : state)
        std::copy_backward(
            first1 + start1,
            first1 + total_bits - end1,
            first2 + start2 + n
        );
};

