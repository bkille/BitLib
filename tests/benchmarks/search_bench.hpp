#include <benchmark/benchmark.h>
#include "search.hpp"
#include "bit.hpp"

auto BM_BitSearch = [](benchmark::State& state, auto input) {
    using container_type = typename std::tuple_element<0, decltype(input)>::type;
    using word_type = typename std::tuple_element<1, decltype(input)>::type;
    unsigned int total_bits = std::get<2>(input);
    auto digits = bit::binary_digits<word_type>::value;
    auto container_size = total_bits / digits;
    container_type cont1 = make_random_container<container_type>(container_size); 
    auto first1 = bit::bit_iterator<decltype(std::begin(cont1))>(std::begin(cont1));
    auto last1 = bit::bit_iterator<decltype(std::end(cont1))>(std::end(cont1));

    for (auto _ : state)
        bit::search_shift_or(
            first1, 
            last1, 
            last1-5120,
            last1
        );
};

auto BM_BitSearch_WorstCase = [](benchmark::State& state, auto input) {
    using container_type = typename std::tuple_element<0, decltype(input)>::type;
    using word_type = typename std::tuple_element<1, decltype(input)>::type;
    unsigned int total_bits = std::get<2>(input);
    auto digits = bit::binary_digits<word_type>::value;
    auto container_size = total_bits / digits;
    container_type cont1 (container_size, 0);

    auto first1 = bit::bit_iterator<decltype(std::begin(cont1))>(std::begin(cont1));
    auto last1 = bit::bit_iterator<decltype(std::end(cont1))>(std::end(cont1));

    for (int i = 0; i < total_bits - 2048; i += 2048) {
        first1[i] = bit::bit1;
    }

    for (auto _ : state)
        bit::search_shift_or(
            first1, 
            last1, 
            last1-5120,
            last1
        );
};

auto BM_BoolSearch = [](benchmark::State& state, auto input) {
    using container_type = typename std::tuple_element<0, decltype(input)>::type;
    unsigned int total_bits = std::get<2>(input);
    auto container_size = total_bits;
    container_type cont1 = make_random_container<container_type>
                                     (container_size); 
    auto first1 = cont1.begin();
    auto last1 = cont1.end();

    for (auto _ : state)
        std::search(
            first1, 
            last1, 
            last1-5120,
            last1
        );
};

auto BM_BoolSearch_WorstCase = [](benchmark::State& state, auto input) {
    using container_type = typename std::tuple_element<0, decltype(input)>::type;
    unsigned int total_bits = std::get<2>(input);
    auto container_size = total_bits;
    container_type cont1 (container_size, 0);
    for (int i = 0; i < container_size - 2048; i += 2048) 
        cont1[i] = 1;
    auto first1 = cont1.begin();
    auto last1 = cont1.end();

    for (auto _ : state){
        auto ret = std::search(
            first1, 
            last1, 
            last1-5120,
            last1
        );
    }
};

