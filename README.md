# BitLib

![Actions](https://github.com/bkille/BitLib/actions/workflows/cmake.yml/badge.svg?branch=master)[![Coverage Status](https://coveralls.io/repos/github/bkille/BitLib/badge.svg?branch=master)](https://coveralls.io/github/bkille/BitLib?branch=master)

**This repository acts as an efficient replacement of `std::vector<bool>`. It provides implementations of many of the functions in [`<algorithms>`](https://en.cppreference.com/w/cpp/algorithm) optimized for containers of bits, in addition to providing a `bit_vector` class which has roughly the same interface as `std::vector<bool>`**.

This project is built on "[bit iterators](https://github.com/vreverdy/bit)" developed by Vincent Reverdy and many of the implementations in `include/bit-algorithms` come from some of my previous work with Vincent [here](https://github.com/vreverdy/bit-algorithms).

# Example
The code below is from `example/src/example1.cpp`. While the type of word that the bitvector is built off of is templated and you can use any unsigned type, it is likely that you'll want to use `uint64_t` or another 64 bit unsigned type, as that will leverage the most bit-parallelism.
```cpp
#include <iostream>
#include "bitlib/bitlib.hpp"

int main() {
    bit::bit_vector<unsigned char> bv1 ("011111010010");
    std::cout << "Original bitvec:  " << bv1.debug_string() << std::endl;
    // Original bitvec:  01111101 0010

    // Same behavior as std::reverse
    bit::reverse(bv1.begin(), bv1.end());
    std::cout << "Reversed bitvec:  " << bv1.debug_string() << std::endl;
    // Reversed bitvec:  01001011 1110

    // Same behavior as std::rotate
    bit::rotate(bv1.begin(), bv1.begin() + 3, bv1.end());
    std::cout << "Rotated bitvec:   " << bv1.debug_string() << std::endl;
    // Rotated bitvec:   01011111 0010

    // Same behavior as the corresponding std::vector::push_back and std::vector::insert
    bv1.push_back(bit::bit0);
    bv1.insert(bv1.end(), 10, bit::bit1);
    std::cout << "Extended bitvec:  " << bv1.debug_string() << std::endl;
    // Extended bitvec:  01011111 00100111 1111111

    return 0;
}
```

# Installation
BitLib is a header-only libarary. Currently, the BitLib library requires at least `-std=c++17`. 


## CMake
You can automatically fetch the library using Cmake's `FetchContent`. 

```cmake
include(FetchContent)
FetchContent_Declare(
    bitlib
    GIT_REPOSITORY https://github.com/bkille/bitlib.git
    GIT_TAG origin/master
)
FetchContent_MakeAvailable(bitlib)

add_executable(example example.cpp)
target_link_libraries(example bitlib::bitlib)
```

## Manual include
Alternatively, you can copy the `include/bitlib` directory to somewhere in your include path.



# Usage
The goal of BitLib is to be as similar to the C++ STL as possible. The interface of most functions and classes are the same as they are in the STL. Instead of the values being `bool`, we have `bit::bit_value`, which can take on either `bit::bit0` or `bit::bit1`. 

## Containers
 Right now, the only container I have implemented is the bitvector. `bit::bit_vector<WordType>` is essentially a wrapper around `std::vector<WordType>`. The interfaces are nearly identical. In addition to the normal `vector` constructors, you can also provide a string to construct your bitvector:
```cpp
using WordType = uint64_t;
bit::bit_vector<WordType> bvec1 ("011111010010");
```

While the type of word that the bitvector is built off of is templated and you can use any unsigned type, it is likely that you'll want to use `uint64_t` or another 64 bit unsigned type, as that will leverage the most bit-parallelism.

## Algorithms
The algorithms again work in the same manner as the STL. The functions provided here have the same interface as those in the STL, however under the hood, they take advantage of bit-parallelism. It should be noted that if there is an STL algorithm that is not supported yet by BitLib, you can still use the STL implementation. For example:
```cpp
using WordType = uint64_t;
bit::bit_vector<WordType> bvec1 ("011111010010");
bit::bit_vector<WordType> bvec2 = bvec1;
bit::equal(bvec1.begin(), bvec1.end(), bvec2.begin(), bvec1.end());
std::equal(bvec1.begin(), bvec1.end(), bvec2.begin(), bvec1.end()); // Also works, but much slower as it works bit-by-bit
```

For algorithms which take a function (i.e. `bit::transform`), the function should have `WordType` as the input types as well as the return type. For example, to compute the intersection of two bitvectors:
```cpp
using WordType = uint64_t;
auto binary_op = std::bit_and<WordType>();

// Store the AND of bitvec1 and bitvec2 in bitvec3
auto bitret = bit::transform(
        bitvec1.begin(),
        bitvec1.end(),
        bitvec2.begin(),
        bitvec3.begin()
        binary_op); 
```

## Iterators
The bit-iterators are the foundation of the library. In most cases, users will only need to work w/ the `bit::bit_vector::begin()` and `bit::bit_vector::end()` methods to obtain iterators. However, constructing a bit iterator from any address is also straightforward:
```cpp
using WordType = uint64_t;
std::array<WordType, 4> wordArr = {1,2,3,4};
bit::bit_iterator<WordType*>(&(wordArr[0])); // Constructs a bit iterator starting from the first bit from the first word of the vector
bit::bit_iterator<WordType*>(&(wordArr[0]), 1); // Constructs a bit iterator from the second bit (position 1) of the first word of the vector
```

In order to grab the underlying word that a bit pointed to by a bit_iterator comes from, you can use the `bit_iterator.base()` function. 

It is worth noting that the "position" of a bit always increases from LSB to MSB. For those looking to create their own algorithms from bit_iterators, this can be a common "gotcha". For example, shifting a word to the right by `k` will eliminate the first `k` bits of the container. This is only important to those implementing their own algorithms. `bit::shift_*` works as described in the documentation i.e. `shift_right` shifts the container towards `end()` and `shift_left` shifts the container towards `begin()`. 

```
       MSB|<-----|LSB
Position: 76543210
Value:    01010001 --> Sequence: 10001010


// bit::shift_right by 2
       MSB|<-----|LSB
Position: 76543210
Value:    01000100 --> Sequence: 00100010
```

# Documentation
Given that the majority of the library is focused on having the same interface as the C++ STL iterators, containers, and algorithms, users should use the official [STL documentation website](https://en.cppreference.com/). We do plan on adding our own documentation in the future, however. 


# Performance Benchmarks
I used Google's [benchmark](https://github.com/google/benchmark) library for computing benchmarks. Each benchmark is formatted as `{bit, BitArray, std}::function` (size) [(alignment-tags)]. 

    * `bit` is for this library, `BitArray` is for the popular C-based [BitArray library](https://github.com/noporpoise/BitArray), [dynamic_bitset](https://github.com/pinam45/dynamic_bitset) is a header-only library similar to Boost's dynamic_bitset, and`std` is the standard library operating on the infamous `vector<bool>`. 
* (size) denotes the size of the container in bits. `small = 1 << 8`, `medium= 1 << 16`, `large = 1 << 24`, `huge = 1 << 31`
* (alignment-tags) refers to the memory alignment of the bit-iterators. `U` means the iterator does not fall on a word boundary, `R` means the iterator is placed at random, and `A` means the iterator is aligned with a word boundary.

For example, `bit::rotate (large) (ARA)` refers to our library's implementation of the `rotate` algorithm operating on a container of 65536 bits, where `first` and `last` are aligned but `n_first` is selected at random.

```
2023-12-19T17:56:41-06:00
Running ./bin/bitlib-bench
Run on (64 X 1067.77 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x32)
  L1 Instruction 32 KiB (x32)
  L2 Unified 1024 KiB (x32)
  L3 Unified 22528 KiB (x2)
Load Average: 0.70, 0.87, 0.70
---------------------------------------------------------------------------------------
Benchmark                                             Time             CPU   Iterations
---------------------------------------------------------------------------------------
bit::set (large)                                   1.91 ns         1.91 ns    366779196
dynamic_bitset::set (large)                        2.36 ns         2.36 ns    296975883
bitarray::set (large)                              2.20 ns         2.20 ns    318761424
std::set (large)                                   2.39 ns         2.39 ns    293167404
bit::shift_left (small)                            19.8 ns         19.8 ns     35519917
bit::shift_left (small) (UU)                       30.5 ns         30.5 ns     22984689
dynamic_bitset::shift_left (small)                 13.1 ns         13.1 ns     53628854
bitarray::shift_left (small)                       38.4 ns         38.4 ns     18168867
std::shift_left (small)                             579 ns          578 ns      1209283
bit::shift_left (large)                          160869 ns       160845 ns         4353
bit::shift_left (large) (UU)                     280267 ns       280229 ns         2505
dynamic_bitset::shift_left (large)               143487 ns       143454 ns         4877
bitarray::shift_left (large)                     835992 ns       835930 ns          837
std::shift_left (large)                        40289125 ns     40287190 ns           17
bit::shift_right (small)                           27.8 ns         27.8 ns     25146901
bit::shift_right (small) (UU)                      31.1 ns         31.1 ns     22561913
dynamic_bitset::shift_right (small)                12.2 ns         12.2 ns     57443996
bitarray::shift_right (small)                      38.8 ns         38.8 ns     18155925
std::shift_right (small)                            504 ns          504 ns      1392311
bit::shift_right (large)                         164210 ns       164191 ns         4264
bit::shift_right (large) (UU)                    292115 ns       292087 ns         2404
dynamic_bitset::shift_right (large)              125191 ns       125160 ns         5591
bitarray::shift_right (large)                    836455 ns       836415 ns          837
std::shift_right (large)                       36904578 ns     36906143 ns           17
bit::reverse (small) (UU)                          30.4 ns         30.4 ns     23010493
bitarray::reverse (small) (UU)                     92.9 ns         92.9 ns      7566424
std::reverse (small)                                416 ns          416 ns      1709223
bit::reverse (large)                             302243 ns       302246 ns         2314
bit::reverse (large) (UU)                        396252 ns       396251 ns         1766
bitarray::reverse (large)                       4180555 ns      4180640 ns          168
bitarray::reverse (large) (UU)                  5565145 ns      5565237 ns          126
std::reverse (large)                           71610824 ns     71613462 ns           10
bit::transform(UnaryOp) (small)                    7.73 ns         7.73 ns     90799823
bit::transform(UnaryOp) (small) (UU)               16.7 ns         16.7 ns     41797825
dynamic_bitset::transform(UnaryOp) (small)         3.79 ns         3.79 ns    178099711
bitarray::transform(UnaryOp) (small)               8.15 ns         8.15 ns     86177059
std::transform(UnaryOp) (small)                     762 ns          762 ns       920469
bit::transform(UnaryOp) (large)                   89430 ns        89427 ns         7830
bit::transform(UnaryOp) (large) (UU)             513673 ns       513652 ns         1363
dynamic_bitset::transform(UnaryOp) (large)        90179 ns        90174 ns         7755
bitarray::transform(UnaryOp) (large)             182288 ns       182278 ns         3806
std::transform(UnaryOp) (large)                49393629 ns     49392276 ns           14
bit::transform(BinaryOp) (small)                   4.79 ns         4.79 ns    146268444
bit::transform(BinaryOp) (small) (UU)              40.1 ns         40.1 ns     17465510
dynamic_bitset::transform(BinaryOp) (small)        4.35 ns         4.35 ns    160471539
bitarray::transform(BinaryOp) (small)              10.5 ns         10.5 ns     66739191
std::transform(BinaryOp) (small)                    837 ns          837 ns       834684
bit::transform(BinaryOp) (large)                 184508 ns       184491 ns         3796
bit::transform(BinaryOp) (large) (UU)           2396570 ns      2396591 ns          292
dynamic_bitset::transform(BinaryOp) (large)      183006 ns       182980 ns         3813
bitarray::transform(BinaryOp) (large)            131178 ns       131171 ns         5348
std::transform(BinaryOp) (large)              195492307 ns    195488596 ns            4
bit::rotate (small)                                 121 ns          121 ns     10000000
std::rotate (small)                                1725 ns         1725 ns       467233
bit::rotate (large)                             1830057 ns      1830041 ns          377
std::rotate (large)                           149375227 ns    149373295 ns            5
bit::count (small)                                 6.28 ns         6.28 ns    111995013
dynamic_bitset::count (small)                      8.12 ns         8.12 ns     87716832
bitarray::count (small)                            6.11 ns         6.11 ns    114586171
std::count (small)                                  233 ns          233 ns      3000468
bit::count (large)                                86768 ns        86767 ns         8067
dynamic_bitset::count (large)                     86774 ns        86776 ns         8068
bitarray::count (large)                          228298 ns       228300 ns         3066
std::count (large)                             14717449 ns     14717517 ns           48
bit::swap_ranges (small)                           8.03 ns         8.03 ns     85409308
bit::swap_ranges (small) (UU)                      19.0 ns         19.0 ns     36799054
std::swap_ranges (small)                            753 ns          753 ns       932645
bit::swap_ranges (large)                         206087 ns       206069 ns         3400
bit::swap_ranges (large) (UU)                   1416540 ns      1416482 ns          495
std::swap_ranges (large)                      128732217 ns    128736753 ns            5
bit::copy (small) (UU)                             22.6 ns         22.6 ns     30977614
std::copy (small)                                   706 ns          706 ns       991971
bit::copy (large) (UU)                          1283021 ns      1282931 ns          546
std::copy (large)                              47291412 ns     47293406 ns           15
bit::equal (small) (UU)                            13.5 ns         13.5 ns     51665518
std::equal (small)                                  887 ns          887 ns       789443
bit::equal (large) (UU)                          684564 ns       684588 ns         1023
std::equal (large)                             58741336 ns     58740796 ns           12
bit::move (small) (UU)                             24.9 ns         24.9 ns     28152253
std::move (small)                                   705 ns          705 ns       993177
bit::move (large) (UU)                          1486436 ns      1486307 ns          471
std::move (large)                              47268916 ns     47269412 ns           15
bit::copy_backward (small) (UU)                    35.8 ns         35.8 ns     19180871
std::copy_backward (small)                          524 ns          524 ns      1336116
bit::copy_backward (large) (UU)                 1843335 ns      1843176 ns          381
std::copy_backward (large)                    110068625 ns    110069932 ns            6
bit::fill (small) (UU)                             6.81 ns         6.81 ns    103143199
dynamic_bitset::fill (small)                       3.55 ns         3.55 ns    198214175
bitarray::fill (small)                             13.9 ns         13.9 ns     50233774
std::fill (small)                                  9.57 ns         9.57 ns     73133048
bit::fill (large) (UU)                            95661 ns        95650 ns         7326
dynamic_bitset::fill (large)                     102146 ns       102146 ns         6851
bitarray::fill (large)                            72462 ns        72462 ns         9615
std::fill (large)                                 72955 ns        72955 ns         9741
bit::find (small) (UU)                             3.22 ns         3.22 ns    217967844
dynamic_bitset::find (small)                       3.05 ns         3.05 ns    229824606
bitarray::find (small)                             7.15 ns         7.15 ns     94973526
std::find (small)                                   100 ns          100 ns      6992893
bit::find (large) (UU)                            27810 ns        27808 ns        25202
dynamic_bitset::find (large)                      64434 ns        64437 ns        10870
bitarray::find (large)                            62305 ns        62298 ns        11220
std::find (large)                               6376779 ns      6376904 ns          110
```
  
