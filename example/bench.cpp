#include <iostream>
#include <chrono>
#include <string>
#include <string_view>
#include <vector>
#include <random>
#include "bitlib/bitlib.hpp"
#include "test_utils.hpp"

int main() {
    using bv_type = bit::bit_vector<uint64_t>;
    uint64_t size = 1ULL << 16;
    bv_type bv1 = make_random_binary_container<bv_type>(size);
    bv_type bv2 = make_random_binary_container<bv_type>(size);
    auto count = 0;
    for (auto i = 0; i < 100000000; ++i) {
        bit::transform(bv1.begin(), bv1.end(), bv2.begin(), bv1.begin(), std::bit_and<uint64_t>());
        count +=  i;
    }
    std::cout << count << std::endl;

}
