#include <iostream>
#include <chrono>
#include <string>
#include <string_view>
#include <vector>
#include <random>
#include <boost/dynamic_bitset.hpp>
#include "bitlib/bitlib.hpp"

int main() {

    uint64_t len = 1 << 16;
    uint64_t num_elements = 1 << 14;
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(0, len); // define the range

    std::vector<uint64_t> rand_ints;
    for (int i = 0; i < len; ++i) {
        rand_ints.push_back(distr(gen));
    }
    std::vector<uint64_t> rand_reads;
    for (int i = 0; i < len; ++i) {
        rand_reads.push_back(distr(gen));
    }
    bit::bit_vector<uint64_t> bit_vec(len);
    boost::dynamic_bitset<uint64_t> boost_db(len);
    std::vector<bool> bool_vec(len);

    auto t1 = std::chrono::high_resolution_clock::now();
    for (auto index : rand_ints) {
        bool_vec[index] = true;
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "std::vector<bool>:    Writing " << len << " bits in random order took " <<  std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << "[µs]" << std::endl;

    t1 = std::chrono::high_resolution_clock::now();
    for (auto index : rand_ints) {
        boost_db[index] = true;
    }
    t2 = std::chrono::high_resolution_clock::now();
    std::cout << "boost::dynamic_bitset   Writing " << len << " bits in random order took " <<  std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << "[µs]" << std::endl;

     t1 = std::chrono::high_resolution_clock::now();
    for (auto index : rand_ints) {
        bit_vec[index] = bit::bit1;
    }
     t2 = std::chrono::high_resolution_clock::now();
    std::cout << "bit::bitvector:    Writing " << len << " bits in random order took " <<  std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << "[µs]" << std::endl;


    uint64_t count = 0;
    t1 = std::chrono::high_resolution_clock::now();
    for (auto index : rand_reads) {
        volatile auto b = bool_vec[index];
    }
    t2 = std::chrono::high_resolution_clock::now();
    std::cout << "std::vector<bool>:    Reading " << len << " bits in random order took " <<  std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << "[µs]" << std::endl;

    t1 = std::chrono::high_resolution_clock::now();
    for (auto index : rand_reads) {
        volatile auto b = boost_db[index];
    }
    t2 = std::chrono::high_resolution_clock::now();
    std::cout << "boost::dynamic_bitset   Reading " << len << " bits in random order took " <<  std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << "[µs]" << std::endl;

     t1 = std::chrono::high_resolution_clock::now();
    for (auto index : rand_reads) {
        volatile bit::bit_value b = bit_vec[index];
    }
     t2 = std::chrono::high_resolution_clock::now();
    std::cout << "bit::bitvector:    Reading " << len << " bits in random order took " <<  std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << "[µs]" << std::endl;
    std::cout << count << std::endl;
    return 0;
}
