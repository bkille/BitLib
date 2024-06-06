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

## SIMD support, testing and benchmarking

SIMD support (enabled via Google's [highway](https://github.com/google/highway) library) can be enabled by defining `BITLIB_HWY`. For example, with `cmake`, you can run `cmake -DBITLIB_HWY=1`. Other options can be found in the `CMakeLists.txt` file:

```cmake
option(BITLIB_HWY "Build with google highway SIMD extensions" OFF)
option(BITLIB_BENCHMARK "Build bitlib benchmarks" OFF)
option(BITLIB_EXAMPLE "Build bitlib examples" OFF)
option(BITLIB_TEST "Build bitlib tests" OFF)
option(BITLIB_PROFILE "Buid simple example for profiling" OFF)
option(BITLIB_COVERAGE "Compute test coverage" OFF)
```

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
---------------------------------------------------------------------------------------
Benchmark                                             Time             CPU   Iterations
---------------------------------------------------------------------------------------
bit::set (large)                                   1.90 ns         1.90 ns    367974893
dynamic_bitset::set (large)                        2.37 ns         2.37 ns    296837879
bitarray::set (large)                              2.19 ns         2.19 ns    319133940
std::set (large)                                   2.39 ns         2.39 ns    293135332
bit::shift_left (small)                            26.8 ns         26.8 ns     25929070
bit::shift_left (small) (UU)                       22.4 ns         22.4 ns     31233265
dynamic_bitset::shift_left (small)                 13.1 ns         13.1 ns     53627207
bitarray::shift_left (small)                       38.2 ns         38.2 ns     18339126
std::shift_left (small)                             345 ns          345 ns      2029283
bit::shift_left (large)                          371224 ns       371211 ns         1886
bit::shift_left (large) (UU)                     371536 ns       371530 ns         1880
dynamic_bitset::shift_left (large)               638896 ns       638880 ns         1097
bitarray::shift_left (large)                    3156273 ns      3156003 ns          222
std::shift_left (large)                       105227752 ns    105223527 ns            7
bit::shift_right (small)                           26.9 ns         26.9 ns     25976563
bit::shift_right (small) (UU)                      39.3 ns         39.3 ns     17962533
dynamic_bitset::shift_right (small)                12.2 ns         12.2 ns     57419526
bitarray::shift_right (small)                      38.1 ns         38.1 ns     18325350
std::shift_right (small)                            504 ns          504 ns      1386280
bit::shift_right (large)                         413297 ns       413269 ns         1693
bit::shift_right (large) (UU)                    413692 ns       413655 ns         1682
dynamic_bitset::shift_right (large)              557287 ns       557305 ns         1257
bitarray::shift_right (large)                   3156463 ns      3156516 ns          222
std::shift_right (large)                      210100788 ns    210083631 ns            3
bit::reverse (small) (UU)                          43.4 ns         43.4 ns     16112098
bitarray::reverse (small) (UU)                     95.1 ns         95.1 ns      7387177
std::reverse (small)                                419 ns          419 ns      1677069
bit::reverse (large)                            1245260 ns      1245160 ns          563
bit::reverse (large) (UU)                       1800771 ns      1800680 ns          389
bitarray::reverse (large)                      16899481 ns     16898587 ns           41
bitarray::reverse (large) (UU)                 22719408 ns     22720393 ns           31
std::reverse (large)                          293563397 ns    293542850 ns            2
bit::transform(UnaryOp) (small)                    8.75 ns         8.75 ns     80079214
bit::transform(UnaryOp) (small) (UU)               16.6 ns         16.6 ns     42254961
dynamic_bitset::transform(UnaryOp) (small)         4.00 ns         4.00 ns    169219246
bitarray::transform(UnaryOp) (small)               8.39 ns         8.39 ns     83877004
std::transform(UnaryOp) (small)                     763 ns          763 ns       917975
bit::transform(UnaryOp) (large)                  373982 ns       373950 ns         1853
bit::transform(UnaryOp) (large) (UU)            2059234 ns      2059268 ns          339
dynamic_bitset::transform(UnaryOp) (large)       379368 ns       379368 ns         1805
bitarray::transform(UnaryOp) (large)             739552 ns       739544 ns          881
std::transform(UnaryOp) (large)               197977698 ns    197969224 ns            4
bit::transform(BinaryOp) (small)                   4.38 ns         4.38 ns    160002060
bit::transform(BinaryOp) (small) (UU)              42.1 ns         42.1 ns     16549758
dynamic_bitset::transform(BinaryOp) (small)        4.36 ns         4.36 ns    160692979
bitarray::transform(BinaryOp) (small)              10.7 ns         10.7 ns     66178974
std::transform(BinaryOp) (small)                    855 ns          855 ns       832115
bit::transform(BinaryOp) (large)                 763642 ns       763574 ns          912
bit::transform(BinaryOp) (large) (UU)          10966202 ns     10966406 ns           64
dynamic_bitset::transform(BinaryOp) (large)      758617 ns       758574 ns          906
bitarray::transform(BinaryOp) (large)            518286 ns       518267 ns         1177
std::transform(BinaryOp) (large)              802270688 ns    802303941 ns            1
bit::rotate (small)                                 131 ns          131 ns     16525922
std::rotate (small)                                1782 ns         1782 ns       417293
bit::rotate (large)                             7333284 ns      7333170 ns           96
std::rotate (large)                           514697313 ns    514718779 ns            1
bit::count (small)                                 8.14 ns         8.14 ns     86522765
dynamic_bitset::count (small)                      6.29 ns         6.29 ns    108878018
bitarray::count (small)                            5.47 ns         5.47 ns    133692569
std::count (small)                                  234 ns          234 ns      2997782
bit::count (large)                               365194 ns       365159 ns         1919
dynamic_bitset::count (large)                    365279 ns       365269 ns         1919
bitarray::count (large)                          917302 ns       917185 ns          764
std::count (large)                             58934071 ns     58931785 ns           12
bit::swap_ranges (small)                           9.58 ns         9.57 ns     73128377
bit::swap_ranges (small) (UU)                      19.7 ns         19.7 ns     35498474
std::swap_ranges (small)                            756 ns          756 ns       912041
bit::swap_ranges (large)                         852205 ns       852241 ns          821
bit::swap_ranges (large) (UU)                   5691899 ns      5692145 ns          123
std::swap_ranges (large)                      522198664 ns    522161939 ns            1
bit::copy (small) (UU)                             25.0 ns         25.0 ns     28200772
std::copy (small)                                   707 ns          707 ns       990757
bit::copy (large) (UU)                          5952278 ns      5951729 ns          116
std::copy (large)                             189551338 ns    189554366 ns            4
bit::equal (small) (UU)                            13.1 ns         13.1 ns     53616228
std::equal (small)                                  886 ns          886 ns       790035
bit::equal (large) (UU)                         1960399 ns      1960375 ns          357
std::equal (large)                            234389098 ns    234398907 ns            3
bit::move (small) (UU)                             23.5 ns         23.5 ns     29764745
std::move (small)                                   706 ns          706 ns       992054
bit::move (large) (UU)                          5135837 ns      5135619 ns          136
std::move (large)                             188961979 ns    188953500 ns            4
bit::copy_backward (small) (UU)                    39.0 ns         39.0 ns     17977387
std::copy_backward (small)                          527 ns          527 ns      1313265
bit::copy_backward (large) (UU)                 9163333 ns      9163038 ns           76
std::copy_backward (large)                    444362971 ns    444350668 ns            2
bit::fill (small) (UU)                             6.48 ns         6.48 ns    108934237
dynamic_bitset::fill (small)                       4.79 ns         4.79 ns    146205764
bitarray::fill (small)                             14.5 ns         14.5 ns     48030428
std::fill (small)                                  9.15 ns         9.15 ns     76612702
bit::fill (large) (UU)                           440400 ns       440396 ns         1590
dynamic_bitset::fill (large)                     429375 ns       429359 ns         1631
bitarray::fill (large)                           369732 ns       369736 ns         1964
std::fill (large)                                356517 ns       356488 ns         1894
bit::find (small) (UU)                             3.10 ns         3.10 ns    228714994
dynamic_bitset::find (small)                       3.05 ns         3.05 ns    229830138
bitarray::find (small)                             7.38 ns         7.38 ns     99039746
std::find (small)                                   110 ns          110 ns      6311725
bit::find (large) (UU)                           182002 ns       182006 ns         3850
dynamic_bitset::find (large)                     259896 ns       259908 ns         2696
bitarray::find (large)                           252434 ns       252445 ns         2774
std::find (large)                              28570723 ns     28567762 ns           25
```
  
