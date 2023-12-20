#include <functional>
#include <cstring>
#include <benchmark/benchmark.h>
#include "test_utils.hpp"
#include "bitlib/bitlib.hpp"
#include "sul/dynamic_bitset.hpp"
#include "bit_array.h"

auto BM_BitTransformUnaryAA = [](benchmark::State& state, auto input) {
    using container_type = typename std::tuple_element<0, decltype(input)>::type;
    using WordType = typename std::tuple_element<1, decltype(input)>::type;
    unsigned int total_bits = std::get<2>(input);
    auto digits = bit::binary_digits<WordType>::value;
    auto container_size = total_bits / digits;
    auto bitvec1 = get_random_vec<WordType>(container_size);
    auto first1 = bit::bit_iterator<decltype(std::begin(bitvec1))>(std::begin(bitvec1));
    auto last1 = bit::bit_iterator<decltype(std::end(bitvec1))>(std::end(bitvec1));

    constexpr auto unary_op = std::bit_not<WordType>();
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(bit::transform(
            first1,
            last1,
            first1,
            unary_op
        ));
        benchmark::ClobberMemory();
    }
};

auto BM_BitTransformUnaryUU = [](benchmark::State& state, auto input) {
    using container_type = typename std::tuple_element<0, decltype(input)>::type;
    using WordType = typename std::tuple_element<1, decltype(input)>::type;
    unsigned int total_bits = std::get<2>(input);
    auto digits = bit::binary_digits<WordType>::value;
    auto container_size = total_bits / digits + 1;
    auto bitvec1 = get_random_vec<WordType>(container_size);
    auto first1 = bit::bit_iterator<decltype(std::begin(bitvec1))>(std::begin(bitvec1));

    constexpr auto unary_op = std::bit_not<WordType>();
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(bit::transform(
            first1 + 2,
            first1 + total_bits - 4,
            first1 + 1,
            unary_op
        ));
        benchmark::ClobberMemory();
    }
};


auto BM_BitArrayTransformUnary = [](benchmark::State& state, auto input) {
    using container_type = typename std::tuple_element<0, decltype(input)>::type;
    using word_type = typename std::tuple_element<1, decltype(input)>::type;
    unsigned int total_bits = std::get<2>(input);
    BIT_ARRAY* bitarr = bit_array_create(total_bits);

    for (auto _ : state)
    {
        bit_array_not(bitarr, bitarr);
        benchmark::ClobberMemory();
    }
};

auto BM_DynamicBitsetTransformUnary = [](benchmark::State& state, auto input) {
    using container_type = typename std::tuple_element<0, decltype(input)>::type;
    using WordType = typename std::tuple_element<1, decltype(input)>::type;
    unsigned int total_bits = std::get<2>(input);
    auto digits = bit::binary_digits<WordType>::value;
    auto container_size = total_bits / digits + 1;
    auto bitvec1 = get_random_vec<WordType>(container_size);
    sul::dynamic_bitset<WordType> bitset1(total_bits, 1);
    std::memcpy((char*)bitset1.data(), static_cast<void*>((bitvec1.data())), total_bits / 8);
    for (auto _ : state) {
        benchmark::DoNotOptimize(bitset1.flip());
        benchmark::ClobberMemory();
    }
};

auto BM_BoolTransformUnary = [](benchmark::State& state, auto input) {
    using container_type = typename std::tuple_element<0, decltype(input)>::type;
    unsigned int total_bits = std::get<2>(input);
    auto container_size = total_bits;
    container_type boolvec1 = make_random_container<container_type> (container_size); 
    container_type boolvec2 = make_random_container<container_type> (container_size); 
    auto first1 = boolvec1.begin();
    auto first2 = boolvec2.begin();


    auto unary_op = [](bool b) {return !b;};
    for (auto _ : state)
    {
        std::transform(
            first1,
            first1 + total_bits,
            first2,
            unary_op
        );
        benchmark::ClobberMemory();
    }
};


auto BM_BitTransformBinaryAA = [](benchmark::State& state, auto input) {
    using container_type = typename std::tuple_element<0, decltype(input)>::type;
    using WordType = typename std::tuple_element<1, decltype(input)>::type;
    unsigned int total_bits = std::get<2>(input);
    auto digits = bit::binary_digits<WordType>::value;
    auto container_size = total_bits / digits + 1;
    auto bitvec1 = get_random_vec<WordType>(container_size);
    auto first1 = bit::bit_iterator<decltype(std::begin(bitvec1))>(std::begin(bitvec1));
    auto bitvec2 = get_random_vec<WordType>(container_size);
    auto first2 = bit::bit_iterator<decltype(std::begin(bitvec2))>(std::begin(bitvec2));

    constexpr auto binary_op = std::bit_and<WordType>();
    for (auto _ : state)
    {
        bit::transform(
            first1,
            first1 + total_bits,
            first2,
            first2,
            binary_op
        );
        benchmark::ClobberMemory();
    }
};

auto BM_BitTransformBinaryUU = [](benchmark::State& state, auto input) {
    using container_type = typename std::tuple_element<0, decltype(input)>::type;
    using WordType = typename std::tuple_element<1, decltype(input)>::type;
    unsigned int total_bits = std::get<2>(input);
    auto digits = bit::binary_digits<WordType>::value;
    auto container_size = total_bits / digits + 1;
    auto bitvec1 = get_random_vec<WordType>(container_size);
    auto first1 = bit::bit_iterator<decltype(std::begin(bitvec1))>(std::begin(bitvec1));
    auto bitvec2 = get_random_vec<WordType>(container_size);
    auto first2 = bit::bit_iterator<decltype(std::begin(bitvec2))>(std::begin(bitvec2));

    constexpr auto binary_op = std::bit_and<WordType>();
    for (auto _ : state)
    {
        bit::transform(
            first1 + 2,
            first1 + total_bits - 4,
            first2 + 3,
            first2 + 1,
            binary_op
        );
        benchmark::ClobberMemory();
    }
};

auto BM_DynamicBitsetTransformBinary = [](benchmark::State& state, auto input) {
    using container_type = typename std::tuple_element<0, decltype(input)>::type;
    using WordType = typename std::tuple_element<1, decltype(input)>::type;
    unsigned int total_bits = std::get<2>(input);
    auto digits = bit::binary_digits<WordType>::value;
    auto container_size = total_bits / digits + 1;
    auto bitvec1 = get_random_vec<WordType>(container_size);
    auto bitvec2 = get_random_vec<WordType>(container_size);
    sul::dynamic_bitset<WordType> bitset1(total_bits, 1);
    sul::dynamic_bitset<WordType> bitset2(total_bits, 1);
    std::memcpy((char*)bitset1.data(), static_cast<void*>((bitvec1.data())), total_bits / 8);
    std::memcpy((char*)bitset2.data(), static_cast<void*>((bitvec2.data())), total_bits / 8);
    for (auto _ : state) {
        benchmark::DoNotOptimize(bitset1 &= bitset2);
        benchmark::ClobberMemory();
    }
};

auto BM_BitArrayTransformBinary = [](benchmark::State& state, auto input) {
    using container_type = typename std::tuple_element<0, decltype(input)>::type;
    using word_type = typename std::tuple_element<1, decltype(input)>::type;
    unsigned int total_bits = std::get<2>(input);
    BIT_ARRAY* bitarr = bit_array_create(total_bits);
    BIT_ARRAY* bitarr2 = bit_array_create(total_bits);

    for (auto _ : state)
    {
        bit_array_and(bitarr, bitarr, bitarr2);
        benchmark::ClobberMemory();
    }
};


auto BM_BoolTransformBinary = [](benchmark::State& state, auto input) {
    using container_type = typename std::tuple_element<0, decltype(input)>::type;
    unsigned int total_bits = std::get<2>(input);
    auto container_size = total_bits;
    container_type boolvec1 = make_random_container<container_type> (container_size); 
    container_type boolvec2 = make_random_container<container_type> (container_size); 
    container_type boolvec3 = make_random_container<container_type> (container_size); 
    auto first1 = boolvec1.begin();
    auto first2 = boolvec2.begin();

    auto binary_op = [](bool a, bool b) {return a && b;};
    for (auto _ : state)
    {
        std::transform(
            first1,
            first1 + total_bits,
            first2,
            first2,
            binary_op
        );
        benchmark::ClobberMemory();
    }
};




