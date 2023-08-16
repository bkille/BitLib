#include <benchmark/benchmark.h>
#include <math.h>
#include "bitlib/bit-algorithms/find.hpp"

auto BM_BitFind = [](benchmark::State& state, auto input) {
    using container_type = typename std::tuple_element<0, decltype(input)>::type;
    using word_type = typename std::tuple_element<1, decltype(input)>::type;
    unsigned int total_bits = std::get<2>(input);
    auto digits = bit::binary_digits<word_type>::value;
    auto container_size = ceil(float(total_bits) / digits);
    container_type bitcont(container_size); 
    auto first = bit::bit_iterator<decltype(std::begin(bitcont))>(std::begin(bitcont));
    auto last = bit::bit_iterator<decltype(std::end(bitcont))>(std::end(bitcont));
    *(first + (bitcont.size() / 2) + 4) = bit::bit1;
    for (auto _ : state) {
        benchmark::DoNotOptimize(bit::find(first + 2, last - 3, bit::bit1));
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
