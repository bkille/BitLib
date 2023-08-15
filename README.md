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

* `bit` is for this library, `BitArray` is for the popular C-based [BitArray library](https://github.com/noporpoise/BitArray), and`std` is the standard library operating on the infamous `vector<bool>`. 
* (size) denotes the size of the container in bits. `small = 1 << 4`, `large = 1 << 16`
* (alignment-tags) refers to the memory alignment of the bit-iterators. `U` means the iterator does not fall on a word boundary, `R` means the iterator is placed at random, and `A` means the iterator is aligned with a word boundary.

For example, `bit::rotate (large) (ARA)` refers to our library's implementation of the `rotate` algorithm operating on a container of 65536 bits, where `first` and `last` are aligned but `n_first` is selected at random.

```
2022-05-04T16:54:22-05:00
Running ./bin/bench
Run on (80 X 2899.73 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x40)
  L1 Instruction 32 KiB (x40)
  L2 Unified 1024 KiB (x40)
  L3 Unified 28160 KiB (x2)
Load Average: 1.12, 0.98, 0.54
--------------------------------------------------------------------------------
Benchmark                                      Time             CPU   Iterations
--------------------------------------------------------------------------------
bit::shift_left (small) (AA)                4.79 ns         4.79 ns    146028612
bit::shift_left (small) (UU)                3.72 ns         3.72 ns    187172020
std::shift_left (small)                     37.8 ns         37.8 ns     18507630
bit::shift_left (large) (AA)                78.9 ns         78.9 ns      8887302
bit::shift_left (large) (UU)                 243 ns          243 ns      2887952
std::shift_left (large)                   156867 ns       156869 ns         4463
bit::shift_right (small) (UU)               3.48 ns         3.48 ns    201058677
std::shift_right (small)                    35.7 ns         35.7 ns     19186367
bit::shift_right (large) (AA)               68.3 ns         68.3 ns     10249245
std::shift_right (large)                  132458 ns       132461 ns         5276
bit::reverse (small) (UU)                   8.73 ns         8.73 ns     80176090
std::reverse (small)                        39.9 ns         39.9 ns     17545669
bit::reverse (large) (AA)                    842 ns          842 ns       830385
bit::reverse (large) (UU)                   1157 ns         1157 ns       605963
std::reverse (large)                      285799 ns       285792 ns         2456
bit::transform(UnaryOp) (small) (AA)        5.22 ns         5.22 ns    134034538
bit::transform(UnaryOp) (small) (UU)        6.28 ns         6.28 ns    111084155
std::transform(UnaryOp) (small)             50.6 ns         50.6 ns     13837852
bit::transform(UnaryOp) (large) (AA)         238 ns          238 ns      2956037
bit::transform(UnaryOp) (large) (UU)        2005 ns         2005 ns       349160
std::transform(UnaryOp) (large)           192498 ns       192502 ns         3637
bit::transform(BinaryOp) (small) (AA)       7.50 ns         7.50 ns     93300797
bit::transform(BinaryOp) (small) (UU)       7.85 ns         7.85 ns     89176138
std::transform(BinaryOp) (small)            37.1 ns         37.1 ns     18848167
bit::transform(BinaryOp) (large) (AA)        345 ns          345 ns      2030257
bit::transform(BinaryOp) (large) (UU)      12924 ns        12925 ns        54165
std::transform(BinaryOp) (large)          619243 ns       619246 ns         1134
bit::rotate (small) (ARA)                   9.14 ns         9.14 ns    123732722
std::rotate (small)                         79.7 ns         79.7 ns      9138769
bit::rotate (large) (ARA)                   7617 ns         7617 ns        92147
std::rotate (large)                       582126 ns       582135 ns         1207
bit::count (small) (AA)                     2.29 ns         2.29 ns    299434270
std::count (small)                          15.2 ns         15.2 ns     45934612
bit::count (large) (AA)                      457 ns          457 ns      1533128
std::count (large)                         57501 ns        57501 ns        12174
bit::swap_ranges (small) (AA)               6.76 ns         6.76 ns    103735181
bit::swap_ranges (small) (UU)               5.43 ns         5.43 ns    128688535
std::swap_ranges (small)                    27.8 ns         27.8 ns     25309938
bit::swap_ranges (large) (AA)                446 ns          446 ns      1570781
bit::swap_ranges (large) (UU)               5496 ns         5496 ns       127033
std::swap_ranges (large)                  507092 ns       507093 ns         1380
bit::copy (small) (UU)                      6.22 ns         6.22 ns    110731355
std::copy (small)                           27.7 ns         27.7 ns     25261667
bit::copy (large) (UU)                      5367 ns         5367 ns       130292
std::copy (large)                         184520 ns       184523 ns         3794
bit::equal (small) (UU)                     3.64 ns         3.64 ns    193325012
std::equal (small)                          32.2 ns         32.2 ns     21650629
bit::equal (large) (UU)                     1799 ns         1799 ns       389158
std::equal (large)                        200078 ns       200080 ns         3499
bit::move (small) (UU)                      6.31 ns         6.31 ns    110834953
std::move (small)                           27.7 ns         27.7 ns     25270665
bit::move (large) (UU)                      5372 ns         5372 ns       130464
std::move (large)                         184090 ns       184094 ns         3803
bit::copy_backward (small) (UU)             9.60 ns         9.60 ns     72952203
std::copy_backward (small)                  19.9 ns         19.9 ns     35227170
bit::copy_backward (large) (UU)             7602 ns         7602 ns        92137
std::copy_backward (large)                431622 ns       431616 ns         1619
bit::fill (small) (UU)                      4.35 ns         4.35 ns    160834380
std::fill (small)                           2.35 ns         2.35 ns    297524146
bit::fill (huge) (UU)                      17138 ns        17137 ns        40748
std::fill (huge)                           11840 ns        11839 ns        59666
