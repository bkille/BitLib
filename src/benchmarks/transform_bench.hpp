#include <functional>
#include <benchmark/benchmark.h>
#include "test_utils.hpp"
#include "bit_array.h"
#include <boost/dynamic_bitset.hpp>
#include "bitlib/bitlib.hpp"

auto BM_BitTransformUnaryAA = [](benchmark::State& state, auto input) {
    using container_type = typename std::tuple_element<0, decltype(input)>::type;
    using WordType = typename std::tuple_element<1, decltype(input)>::type;
    unsigned int total_bits = std::get<2>(input);
    auto digits = bit::binary_digits<WordType>::value;
    auto container_size = total_bits / digits + 1;
    auto bitvec1 = get_random_vec<WordType>(container_size);
    auto first1 = bit::bit_iterator<decltype(std::begin(bitvec1))>(std::begin(bitvec1));
    auto bitvec2 = get_random_vec<WordType>(container_size);
    auto first2 = bit::bit_iterator<decltype(std::begin(bitvec2))>(std::begin(bitvec2));

    auto unary_op = std::bit_not<WordType>();
    for (auto _ : state)
        benchmark::DoNotOptimize(bit::transform(
            first1,
            first1 + total_bits,
            first2,
            unary_op
        ));
};


auto BM_BitTransformUnaryUU = [](benchmark::State& state, auto input) {
    using container_type = typename std::tuple_element<0, decltype(input)>::type;
    using WordType = typename std::tuple_element<1, decltype(input)>::type;
    unsigned int total_bits = std::get<2>(input);
    auto digits = bit::binary_digits<WordType>::value;
    auto container_size = total_bits / digits + 1;
    auto bitvec1 = get_random_vec<WordType>(container_size);
    auto first1 = bit::bit_iterator<decltype(std::begin(bitvec1))>(std::begin(bitvec1));
    auto bitvec2 = get_random_vec<WordType>(container_size);
    auto first2 = bit::bit_iterator<decltype(std::begin(bitvec2))>(std::begin(bitvec2));

    auto unary_op = std::bit_not<WordType>();
    for (auto _ : state)
        benchmark::DoNotOptimize(bit::transform(
            first1 + 2,
            first1 + total_bits - 4,
            first2 + 3,
            unary_op
        ));
};

auto BM_CBitArrTransformUnary = [](benchmark::State& state, auto input) {
    using container_type = typename std::tuple_element<0, decltype(input)>::type;
    unsigned int total_bits = std::get<2>(input);

    BIT_ARRAY* bitarr1 = bit_array_create(total_bits);
    BIT_ARRAY* bitarr2 = bit_array_create(total_bits);

    for (auto _ : state)
         bit_array_not(bitarr1, bitarr2);
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
        benchmark::DoNotOptimize(std::transform(
            first1,
            first1 + total_bits,
            first2,
            unary_op
        ));
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
    auto bitvec3 = get_random_vec<WordType>(container_size);
    auto first3 = bit::bit_iterator<decltype(std::begin(bitvec3))>(std::begin(bitvec3));

    auto binary_op = std::bit_and<WordType>();
    for (auto _ : state)
        benchmark::DoNotOptimize(bit::transform(
            first1,
            first1 + total_bits,
            first2,
            first3,
            binary_op
        ));
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
    auto bitvec3 = get_random_vec<WordType>(container_size);
    auto first3 = bit::bit_iterator<decltype(std::begin(bitvec3))>(std::begin(bitvec3));

    auto binary_op = std::bit_and<WordType>();
    for (auto _ : state)
        benchmark::DoNotOptimize(bit::transform(
            first1 + 2,
            first1 + total_bits - 4,
            first2 + 3,
            first3 + 1,
            binary_op
        ));
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
    auto first3 = boolvec3.begin();

    auto binary_op = [](bool a, bool b) {return a && b;};
    for (auto _ : state)
        benchmark::DoNotOptimize(std::transform(
            first1,
            first1 + total_bits,
            first2,
            first3,
            binary_op
        ));
};


auto BM_CBitArrTransformBinary = [](benchmark::State& state, auto input) {
    using container_type = typename std::tuple_element<0, decltype(input)>::type;
    unsigned int total_bits = std::get<2>(input);

    BIT_ARRAY* bitarr1 = bit_array_create(total_bits);
    BIT_ARRAY* bitarr2 = bit_array_create(total_bits);
    BIT_ARRAY* bitarr3 = bit_array_create(total_bits);

    for (auto _ : state)
         bit_array_and(bitarr1, bitarr2, bitarr3);
};

auto BM_BoostTransformBinaryAA = [](benchmark::State& state, auto input) {
    using container_type = typename std::tuple_element<0, decltype(input)>::type;
    using WordType = typename std::tuple_element<1, decltype(input)>::type;
    unsigned int total_bits = std::get<2>(input);
    auto digits = bit::binary_digits<WordType>::value;
    auto unary_op = std::bit_not<WordType>();
    boost::dynamic_bitset<uint64_t> x(total_bits);
    boost::dynamic_bitset<uint64_t> y(total_bits);
    boost::dynamic_bitset<uint64_t> z;
    container_type boolvec1 = make_random_container<container_type> (total_bits); 
    container_type boolvec2 = make_random_container<container_type> (total_bits); 
    for (auto i = 0; i < total_bits; ++i) {
        x[i] = boolvec1[i];
        y[i] = boolvec2[i];
    }
    for (auto _ : state)
        benchmark::DoNotOptimize(z = x&y);
};

