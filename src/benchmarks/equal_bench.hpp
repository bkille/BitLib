#include <benchmark/benchmark.h>
#include "test_utils.hpp"
#include <boost/dynamic_bitset.hpp>
#include "bitlib/bitlib.hpp"

auto BM_BitEqual = [](benchmark::State& state, auto input) {
    using container_type = typename std::tuple_element<0, decltype(input)>::type;
    using WordType = typename std::tuple_element<1, decltype(input)>::type;
    unsigned int total_bits = std::get<2>(input);
    auto digits = bit::binary_digits<WordType>::value;
    auto container_size = total_bits / digits + 1;
    auto bitvec1 = get_random_vec<WordType>(container_size);
    auto first1 = bit::bit_iterator<decltype(std::begin(bitvec1))>(std::begin(bitvec1));
    auto bitvec2 = get_random_vec<WordType>(container_size);
    auto first2 = bit::bit_iterator<decltype(std::begin(bitvec2))>(std::begin(bitvec2));

    // Only really care about benchmarking when equal, since it is worst case scenario
    std::copy(
        first1,
        first1 + total_bits,
        first2
    );
    for (auto _ : state)
        benchmark::DoNotOptimize(bit::equal(
            first1,
            first1 + total_bits,
            first2
        ));
};

auto BM_BitEqualUU = [](benchmark::State& state, auto input) {
    using container_type = typename std::tuple_element<0, decltype(input)>::type;
    using WordType = typename std::tuple_element<1, decltype(input)>::type;
    unsigned int total_bits = std::get<2>(input);
    auto digits = bit::binary_digits<WordType>::value;
    auto container_size = total_bits / digits + 1;
    auto bitvec1 = get_random_vec<WordType>(container_size);
    auto first1 = bit::bit_iterator<decltype(std::begin(bitvec1))>(std::begin(bitvec1));
    auto bitvec2 = get_random_vec<WordType>(container_size);
    auto first2 = bit::bit_iterator<decltype(std::begin(bitvec2))>(std::begin(bitvec2));

    // Only really care about benchmarking when equal, since it is worst case scenario
    std::copy(
        first1 + 1,
        first1 + total_bits - 4,
        first2 + 2
    );
    for (auto _ : state)
        benchmark::DoNotOptimize(bit::equal(
        first1 + 1,
        first1 + total_bits - 4,
        first2 + 2
        ));
};


auto BM_BoostEqual = [](benchmark::State& state, auto input) {
    using container_type = typename std::tuple_element<0, decltype(input)>::type;
    using WordType = typename std::tuple_element<1, decltype(input)>::type;
    unsigned int total_bits = std::get<2>(input);
    boost::dynamic_bitset<uint64_t> x(total_bits);
    boost::dynamic_bitset<uint64_t> y(total_bits);
    container_type boolvec1 = make_random_container<container_type> (total_bits); 
    for (auto i = 0; i < total_bits; ++i) {
        x[i] = boolvec1[i];
        y[i] = boolvec1[i];
    }
    // Only really care about benchmarking when equal, since it is worst case scenario
    for (auto _ : state) {
        benchmark::DoNotOptimize(x == y);
    }
};

auto BM_BoolEqual = [](benchmark::State& state, auto input) {
    using container_type = typename std::tuple_element<0, decltype(input)>::type;
    unsigned int total_bits = std::get<2>(input);
    auto container_size = total_bits;
    container_type boolvec1 = make_random_container<container_type> (container_size); 
    container_type boolvec2 = make_random_container<container_type> (container_size); 
    auto first1 = boolvec1.begin();
    auto first2 = boolvec2.begin();


    // Only really care about benchmarking when equal, since it is worst case scenario
    std::copy(
        first1 + 1,
        first1 + total_bits - 4,
        first2 + 2
    );
    for (auto _ : state)
        benchmark::DoNotOptimize(std::equal(
                first1 + 1,
                first1 + total_bits - 4,
                first2 + 2
        ));
};

