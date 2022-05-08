# BitLib

![Actions](https://github.com/bkille/BitLib/actions/workflows/cmake.yml/badge.svg?branch=master)[![Coverage Status](https://coveralls.io/repos/github/bkille/BitLib/badge.svg?branch=master)](https://coveralls.io/github/bkille/BitLib?branch=master)

**This repository acts as an efficient replacement of `std::vector<bool>`. It provides implementations of many of the functions in [`<algorithms>`](https://en.cppreference.com/w/cpp/algorithm) optimized for containers of bits, in addition to providing a `bit_vector` class which has roughly the same interface as `std::vector<bool>`**.

This project is built on "[bit iterators](https://github.com/vreverdy/bit)" developed by Vincent Reverdy and many of the implementations in `include/bit-algorithms` come from some of my previous work with Vincent [here](https://github.com/vreverdy/bit-algorithms).

# Example
The code below is from `example/print.cpp`. While the type of word that the bitvector is built off of is templated and you can use any unsigned type, it is likely that you'll want to use `uint64_t` or another 64 bit unsigned type, as that will leverage the most bit-parallelism.
```cpp
#include <iostream>
#include "bitlib/bitlib.hpp"

int main() {
    bit::bit_vector<unsigned char> bv1 {"011111010010"};
    std::cout << "Original bitvec:  " << bv1.debug_string() << std::endl;
    // Original bitvec:  01111101 0010

    // Same behavior as std::reverse
    bit::reverse(bv1.begin(), bv1.end());
    std::cout << "Reversed bitvec:  " << bv1.debug_string() << std::endl;
    // Reversed bitvec:  01001011 1110

    // Same behavior as std::rotate
    bit::rotate(bv1.begin(), bv.begin() + 3, bv1.end());
    std::cout << "Rotated bitvec:   " << bv1.debug_string() << std::endl;
    // Rotated bitvec:   01011111 0010

    // Same behavior as the corresponding std::vector::push_back and std::vector::insert
    bv1.push_back(bit::bit0);
    bv1.insert(bv.end(), 10, bit::bit1);
    std::cout << "Extended bitvec:  " << bv1.debug_string() << std::endl;
    // Extended bitvec:  01011111 00100111 1111111

    return 0;
}
```

# Speedy highlights

Here are some of the highlighted benchmarking results. Algorithms were ran on containers of `1 << 16 = 65536` bits. The "Alignment" column denotes the memory alignment of the input bit-iterators. `U` means the iterators do not fall on a word boundary and `A` means the iterators are aligned with a word boundary. More succinctly, `alignment = (iterator.position() == 0) ? 'A'  'U'`. 

## Compared to `std::vector<bool>`
| Function | Alignment | Speedup |
|--|--|--|
|shift_left | U | 734.24x|
|shift_left | A | 1970.49x|
|shift_right |A | 1933.49x|
|reverse |U | 228.25x|
|reverse |A | 309.60x|
|transformUnaryOp |U | 105.86x|
|transformUnaryOp |A | 1209.39x|
|transformBinaryOp |U | 55.50x|
|transformBinaryOp |A | 2436.48x|
|rotate | AUA | 171.43x|
|count |A | 79.89x|
|swap_ranges | U | 99.64x|
|swap_ranges | A | 1462.80x|
|copy | U | 39.85x|
|equal |U | 111.15x|
|move | U | 31.73x|
|copy_backward | U | 77.21x|

## Compared to [noporpoise/BitArray]((https://github.com/noporpoise/BitArray))
I also benchmarked against the [BitArray C library](https://github.com/noporpoise/BitArray). In most cases, the C BitArray library only supports operating on the entire array, so the baseline here is the analagous operation performed on aligned pointers in the BitArray library.

| Function | Alignment | Speedup |
|-|-|-|
|shift_left | A | 34.16x|
|shift_left | U | 12.73x|
|reverse | A | 20.04x|
|reverse | U | 14.78x|
|transform(UnaryOp) | A | 5.76x|
|transform(BinaryOp) | A | 2.68x|
|rotate | AUA | 8.11x|
|count | A | 1.45x|

For more details, see the [benchmarking section](https://github.com/bkille/BitLib#performance-benchmarks).

# Installation
BitLib is a header-only libarary, however it does rely on the simdpp library in `ext/` for simd operations. Currently, the BitLib library requires at least `-std=c++17`. 


## Cmake
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
Alternatively, just make sure the `include/` and `ext/simdpp` folders are added compiler's search path. 



# Usage
The goal of BitLib is to be as similar to the C++ STL as possible. The interface of most functions and classes are the same as they are in the STL. Instead of the values being `bool`, we have `bit::bit_value`, which can take on either `bit::bit0` or `bit::bit1`. 

## Containers
 Right now, the only container I have implemented is the bitvector. `bit::bit_vector<WordType>` is essentially a wrapper around `std::vector<WordType>`. The interfaces are nearly identical. In addition to the normal `vector` constructors, you can also provide a string to construct your bitvector:
```cpp
using WordType = uint64_t;
bit::bit_vector<WordType> bvec1 {"011111010010"};
```

While the type of word that the bitvector is built off of is templated and you can use any unsigned type, it is likely that you'll want to use `uint64_t` or another 64 bit unsigned type, as that will leverage the most bit-parallelism.

## Algorithms
The algorithms again work in the same manner as the STL. The functions provided here have the same interface as those in the STL, however under the hood, they take advantage of bit-parallelism. It should be noted that if there is an STL algorithm that is not supported yet by BitLib, you can still use the STL implementation. For example:
```cpp
using WordType = uint64_t;
bit::bit_vector<WordType> bvec1 {"011111010010"};
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
-------------------------------------------------------------------------------------
Benchmark                                           Time             CPU   Iterations
-------------------------------------------------------------------------------------
bit::shift_left (small) (AA)                     2.71 ns         2.71 ns    258211388
bit::shift_left (small) (UU)                     2.61 ns         2.61 ns    269302138
std::shift_left (small)                          23.5 ns         23.5 ns     29373131
bit::shift_left (large) (AA)                     49.1 ns         49.1 ns     14221586
bit::shift_left (large) (UU)                      132 ns          132 ns      5323746
BitArray::shift_left (large) (AA)                1676 ns         1676 ns       417446
std::shift_left (large)                         96668 ns        96662 ns         7238
-------------------------------------------------------------------------------------
bit::shift_right (small) (UU)                    2.47 ns         2.47 ns    286330458
std::shift_right (small)                         22.2 ns         22.2 ns     31524914
bit::shift_right (large) (AA)                    42.6 ns         42.6 ns     16505937
std::shift_right (large)                        82397 ns        82393 ns         8505
-------------------------------------------------------------------------------------
bit::reverse (small) (UU)                        5.46 ns         5.46 ns    128243169
std::reverse (small)                             24.9 ns         24.9 ns     28032329
bit::reverse (large) (AA)                         515 ns          515 ns      1346635
bit::reverse (large) (UU)                         698 ns          698 ns      1010933
BitArray::reverse (large) (AA)                  10320 ns        10320 ns        66633
std::reverse (large)                           159426 ns       159417 ns         4392
-------------------------------------------------------------------------------------
bit::transform(UnaryOp) (small) (AA)             3.34 ns         3.34 ns    209550489
bit::transform(UnaryOp) (small) (UU)             4.63 ns         4.63 ns    151344589
std::transform(UnaryOp) (small)                  31.6 ns         31.6 ns     22120482
bit::transform(UnaryOp) (large) (AA)             98.6 ns         98.6 ns      7071918
BitArray::transform(UnaryOp) (large) (AA)         568 ns          568 ns      1236126
bit::transform(UnaryOp) (large) (UU)             1126 ns         1126 ns       621683
std::transform(UnaryOp) (large)                119215 ns       119211 ns         5863
-------------------------------------------------------------------------------------
bit::transform(BinaryOp) (small) (AA)            5.70 ns         5.70 ns    122674050
bit::transform(BinaryOp) (small) (UU)            5.44 ns         5.44 ns    128673649
std::transform(BinaryOp) (small)                 23.3 ns         23.3 ns     31074047
bit::transform(BinaryOp) (large) (AA)             159 ns          159 ns      4254743
BitArray::transform(BinaryOp) (large) (AA)        428 ns          428 ns      1612610
bit::transform(BinaryOp) (large) (UU)            6992 ns         6991 ns       100430
std::transform(BinaryOp) (large)               388040 ns       388017 ns         1805
-------------------------------------------------------------------------------------
bit::rotate (small) (AUA)                        5.43 ns         5.43 ns    128914954
std::rotate (small)                              52.8 ns         52.8 ns     13262485
bit::rotate (large) (AUA)                        2581 ns         2580 ns       271229
BitArray::rotate (large) (AUA)                  20932 ns        20931 ns        33469
std::rotate (large)                            442400 ns       442382 ns         1578
-------------------------------------------------------------------------------------
bit::count (small) (AA)                          1.37 ns         1.37 ns    515163169
std::count (small)                               9.68 ns         9.68 ns     73184866
bit::count (large) (AA)                           449 ns          449 ns      1548186
BitArray::count (large) (AA)                      651 ns          651 ns      1072912
std::count (large)                              35865 ns        35863 ns        19501
-------------------------------------------------------------------------------------
bit::swap_ranges (small) (AA)                    4.07 ns         4.07 ns    171889010
bit::swap_ranges (small) (UU)                    3.39 ns         3.39 ns    206244002
std::swap_ranges (small)                         18.0 ns         18.0 ns     38556899
bit::swap_ranges (large) (AA)                     234 ns          234 ns      2995324
bit::swap_ranges (large) (UU)                    3432 ns         3432 ns       203790
std::swap_ranges (large)                       341970 ns       341957 ns         2045
-------------------------------------------------------------------------------------
bit::copy (small) (UU)                           4.08 ns         4.08 ns    170777009
std::copy (small)                                17.2 ns         17.2 ns     40512412
bit::copy (large) (UU)                           2901 ns         2900 ns       241640
std::copy (large)                              115582 ns       115577 ns         6068
-------------------------------------------------------------------------------------
bit::equal (small) (UU)                          3.00 ns         3.00 ns    234132358
std::equal (small)                               20.1 ns         20.1 ns     34820885
bit::equal (large) (UU)                          1124 ns         1123 ns       623400
std::equal (large)                             124873 ns       124866 ns         5592
-------------------------------------------------------------------------------------
bit::move (small) (UU)                           4.09 ns         4.09 ns    169329810
std::move (small)                                17.3 ns         17.3 ns     40541053
bit::move (large) (UU)                           3688 ns         3688 ns       191427
std::move (large)                              117027 ns       117018 ns         5918
-------------------------------------------------------------------------------------
bit::copy_backward (small) (UU)                  6.33 ns         6.33 ns    112511417
std::copy_backward (small)                       12.5 ns         12.5 ns     54679813
bit::copy_backward (large) (UU)                  4651 ns         4650 ns       152354
std::copy_backward (large)                     359083 ns       359059 ns         1993
-------------------------------------------------------------------------------------
bit::fill (small) (UU)                           3.05 ns         3.05 ns    228479324
std::fill (small)                                1.48 ns         1.48 ns    446344808
bit::fill (huge) (UU)                            7502 ns         7502 ns        94398
std::fill (huge)                                 7415 ns         7415 ns        91575
```

| Function | Size  | Alignment | Speedup |
|-----------|-------|----|-----|
|shift_left | small | AA | 8.68x|
|shift_left | small | UU | 9.02x|
|shift_left | large | UU | 734.24x|
|shift_left | large | AA | 1970.49x|
|shift_right | small | UU | 8.98x|
|shift_right | large | AA | 1933.49x|
|reverse | small | UU | 4.56x|
|reverse | large | UU | 228.25x|
|reverse | large | AA | 309.60x|
|transformUnaryOp | small | UU | 6.82x|
|transformUnaryOp | small | AA | 9.47x|
|transformUnaryOp | large | UU | 105.86x|
|transformUnaryOp | large | AA | 1209.39x|
|transformBinaryOp | small | AA | 4.09x|
|transformBinaryOp | small | UU | 4.28x|
|transformBinaryOp | large | UU | 55.50x|
|transformBinaryOp | large | AA | 2436.48x|
|rotate | small | AUA | 9.72x|
|rotate | large | AUA | 171.43x|
|count | small | AA | 7.06x|
|count | large | AA | 79.89x|
|swap_ranges | small | AA | 4.42x|
|swap_ranges | small | UU | 5.30x|
|swap_ranges | large | UU | 99.64x|
|swap_ranges | large | AA | 1462.80x|
|copy | small | UU | 4.22x|
|copy | large | UU | 39.85x|
|equal | small | UU | 6.71x|
|equal | large | UU | 111.15x|
|move | small | UU | 4.22x|
|move | large | UU | 31.73x|
|copy_backward | small | UU | 1.98x|
|copy_backward | large | UU | 77.21x|
|fill | small | UU | 0.49x|
|fill | huge | UU | 0.99x|


