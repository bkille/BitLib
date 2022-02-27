#include <benchmark/benchmark.h>
#include <random>
#include <math.h>

auto BM_BitRotate = [](benchmark::State& state, auto input) {
    using container_type = typename std::tuple_element<0, decltype(input)>::type;
    using word_type = typename std::tuple_element<1, decltype(input)>::type;
    unsigned int total_bits = std::get<2>(input);
    auto digits = bit::binary_digits<word_type>::value;
    auto container_size = ceil(float(total_bits) / digits);
    container_type bitcont = make_random_container<container_type>(container_size); 
    const int range_from  = 0;
    const int range_to    = total_bits - 1;
    std::random_device                  rand_dev;
    std::mt19937                        generator(rand_dev());
    std::uniform_int_distribution<int>  distr(range_from, range_to);
    auto first = bit::bit_iterator<decltype(std::begin(bitcont))>(std::begin(bitcont));
    auto n_first = first + distr(generator);
    auto last = bit::bit_iterator<decltype(std::end(bitcont))>(std::end(bitcont));
    for (auto _ : state) {
        benchmark::DoNotOptimize(bit::rotate(first, n_first, last));
        benchmark::ClobberMemory();
    }
};


auto BM_BoolRotate = [](benchmark::State& state, auto input) {
    using container_type = std::vector<bool>;
    using num_type = typename container_type::value_type;
    unsigned int container_size = std::get<2>(input);
    container_type cont = make_random_container<container_type>(container_size); 
    const int range_from  = 0;
    const int range_to    = container_size - 1;
    std::random_device                  rand_dev;
    std::mt19937                        generator(rand_dev());
    std::uniform_int_distribution<int>  distr(range_from, range_to);
    auto first = cont.begin();
    auto n_first = first + distr(generator);
    auto last = cont.end();
    for (auto _ : state) {
        benchmark::DoNotOptimize(std::rotate(first, n_first, last));
        benchmark::ClobberMemory();
    }
};
