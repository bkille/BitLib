# BitLib

![Actions](https://github.com/bkille/BitLib/actions/workflows/cmake.yml/badge.svg?branch=master)[![Coverage Status](https://coveralls.io/repos/github/bkille/BitLib/badge.svg?branch=master)](https://coveralls.io/github/bkille/BitLib?branch=master)


## Description
While [bit iterators](https://github.com/vreverdy/bit) are currently being proposed and reviewed by the ISO C++ Standards Committee, this repository is intended for practical use of bit containers and algorithms based on bit iterators. Primarily, this repository acts as an efficient replacement of `std::vector<bool>`. It provides implementations of many of the functions in `<algorithms>` optimized for containers of bits, in addition to providing a `bit_vector` class which has roughly the same interface as `std::vector<bool>`.

Many of the implementations in `include/bit-algorithms` come from some of my previous work [here](https://github.com/vreverdy/bit-algorithms), however that repository is also somewhat frozen, as it is tied to the ISO C++ Standards Committee proposal as well. In addition, it contains many overloads that are likely less practical (like forward lists of bits). 

## Installation
BitLib is a header-only libarary, however it does rely on the simdpp library in `ext/` for simd operations. In order to use BitLib, just make sure the `include/` and `ext/simdpp` folders are added compiler's search path. Currently, the BitLib library requires at least `-std=c++17`. 


## Example
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

Another example can be seen which showcases some of the capabilities of the `bitlib/bit_iterators` library:

```cpp
#include <iostream>
#include <cstdint>
#include "bitlib/bitlib.hpp"

// Here are a couple examples of what BitLib can accomplish that std::vector<bool> can not. While 
// we still have to explicitly define the type as either reference or pointer, they at least compile
// and behave as expected. There is definitely some work to be done still to make bit::vector behave 
// like a regular STL vector. Need to address the comments made here (and likely elsewhere).
// https://www.reddit.com/r/cpp/comments/ue911x/comment/i6nql6j/?utm_source=share&utm_medium=web2x&context=3
//
// It should be noted that I am not responsible for the creation of this aspect of the library, 
// the bit_iterator/ is thanks to Dr. Vincent Reverdy.

template<typename WordType>
void flip_bits(bit::bit_vector<WordType>& bvec) {
    // Unable to take references to bool, but it works for bits!
    // for (auto bval :  bvec) actually iterates over bit-references, which is misleading since
    // this syntax typically implies that bval would be a copy.
    for (bit::bit_reference<WordType> bval :  bvec) {
        bval = ~bval;
    }
    return;
}

template<typename WordType>
void bit_pointers(bit::bit_vector<WordType>& bvec) {
    // Unable to take pointers to bool, but it works for bits!
    auto p = &bvec[0];
    // bit::bit_pointer<WordType> p = &bvec[0]; also works
    *p = bit::bit1;
    return;
}

int main() {
    bit::bit_vector<unsigned char> bvec("111000110010");
    std::cout << bvec.debug_string() << std::endl;
    // Outputs 11100011 0010

    flip_bits(bvec);
    std::cout << bvec.debug_string() << std::endl;
    // Outputs 00011100 1101

    bit_pointers(bvec);
    std::cout << bvec.debug_string() << std::endl;
    // Outputs 10011100 1101

    return 0;
}
```

## Usage
The goal of BitLib is to be as similar to the C++ STL as possible. The interface of most functions and classes are the same as they are in the STL. Instead of the values being `bool`, we have `bit::bit_value`, which can take on either `bit::bit0` or `bit::bit1`. 

### Containers
 Right now, the only container I have implemented is the bitvector. `bit::bit_vector<WordType>` is essentially a wrapper around `std::vector<WordType>`. The interfaces are nearly identical. In addition to the normal `vector` constructors, you can also provide a string to construct your bitvector:
```cpp
using WordType = uint64_t;
bit::bit_vector<WordType> bvec1 {"011111010010"};
```

While the type of word that the bitvector is built off of is templated and you can use any unsigned type, it is likely that you'll want to use `uint64_t` or another 64 bit unsigned type, as that will leverage the most bit-parallelism.

### Algorithms
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

### Iterators
The bit-iterators are the foundation of the library. In most cases, users will only need to work w/ the `bit::bit_vector::begin()` and `bit::bit_vector::end()` methods to obtain iterators. However, constructing a bit iterator from any address is also straightforward:
```cpp
using WordType = uint64_t;
std::vector<WordType> wordVec = {1,2,3,4};
bit::bit_iterator<WordType>(&(wordVec[0])); // Constructs a bit iterator starting from the first bit from the first word of the vector
bit::bit_iterator<WordType>(&(wordVec[0]), 1); // Constructs a bit iterator from the second bit (position 1) of the first word of the vector
```

In order to grab the underlying word that a bit pointed to by a bit_iterator comes from, you can use the `bit_iterator.base()` function. 

It is worth noting that the "position" of a bit always increases from LSB to MSB. For those looking to create their own algorithms from bit_iterators, this can be a common "gotcha". For example, shifting a word to the right by `k` will eliminate the first `k` bits of the container. This is only important to those implementing their own algorithms. `bit::shift_*` works as described in the documentation i.e. `shift_right` shifts the container towards `end()` and `shift_left` shifts the container towards `begin()`. 

```
       MSB|<-----|LSB
Position: 76543210
Value:    01010001
Sequence: 10001010
// Shift right 2
       MSB|<-----|LSB
Position: 76543210
Value:    00010100
Sequence: 00101000
```

## Documentation
Given that the majority of the library is focused on having the same interface as the C++ STL iterators, containers, and algorithms, users should use the official [STL documentation website](https://en.cppreference.com/). We do plan on adding our own documentation in the future, however. 


## Performance Benchmarks
I used Google's [benchmark](https://github.com/google/benchmark) library for computing benchmarks. Each benchmark is formatted as `{bit, BitArray, std}::function` (size) [(alignment-tags)]. It should be noted that SIMD instructions were turned off for these benchmarks.

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
Load Average: 30.12, 34.38, 27.36
--------------------------------------------------------------------------------
Benchmark                                      Time             CPU   Iterations
--------------------------------------------------------------------------------
bit::shift_left (small) (UU)                3.32 ns         3.32 ns    210813946
bit::shift_left (small) (AA)                3.82 ns         3.82 ns    183929862
std::shift_left (small)                     30.0 ns         30.0 ns     23354867
bit::shift_left (large) (UU)                 224 ns          224 ns      3123287
bit::shift_left (large) (AA)                 249 ns          249 ns      2779768
std::shift_left (large)                   124613 ns       124608 ns         5619
bit::shift_right (small) (UU)               3.12 ns         3.12 ns    224795005
std::shift_right (small)                    28.3 ns         28.3 ns     24736322
bit::shift_right (large) (AA)                198 ns          198 ns      3557331
BitArray::shift_right (large)               1574 ns         1574 ns       444657
std::shift_right (large)                  105509 ns       105505 ns         6665
bit::reverse (small) (UU)                   6.97 ns         6.96 ns    100781790
std::reverse (small)                        32.6 ns         32.6 ns     21506909
bit::reverse (large) (AA)                    677 ns          677 ns      1029718
bit::reverse (large) (UU)                   1018 ns         1018 ns       681715
BitArray::reverse (large)                  10200 ns        10200 ns        68540
std::reverse (large)                      227075 ns       227074 ns         3086
bit::transform(UnaryOp) (small) (UU)        9.78 ns         9.78 ns     72256866
std::transform(UnaryOp) (small)             7.37 ns         7.37 ns    277564125
bit::transform(UnaryOp) (large) (UU)        1601 ns         1601 ns       500139
std::transform(UnaryOp) (large)           126781 ns       126776 ns         8421
bit::transform(BinaryOp) (small) (UU)       9.05 ns         9.05 ns     57675249
std::transform(BinaryOp) (small)            9.24 ns         9.24 ns     99408220
bit::transform(BinaryOp) (large) (UU)       3628 ns         3628 ns       203351
std::transform(BinaryOp) (large)          145481 ns       145476 ns        61477
bit::rotate (small) (ARA)                   6.35 ns         6.35 ns    109070004
std::rotate (small)                         70.5 ns         70.5 ns     17952078
bit::rotate (large) (ARA)                   6359 ns         6358 ns       114733
BitArray::rotate (large)                   21985 ns        21984 ns        31790
std::rotate (large)                       405739 ns       405724 ns         1723
bit::count (small) (AA)                     1.05 ns         1.05 ns    661326099
std::count (small)                          11.8 ns         11.8 ns     59495876
bit::count (large) (AA)                      638 ns          638 ns      1096136
BitArray::count (large)                     1023 ns         1023 ns       684351
std::count (large)                         48201 ns        48199 ns        14523
bit::swap_ranges (small) (UU)               6.93 ns         6.93 ns    101144710
std::swap_ranges (small)                    12.0 ns         12.0 ns    100000000
bit::swap_ranges (large) (UU)               5231 ns         5230 ns       153896
std::swap_ranges (large)                  309796 ns       309783 ns         3836
bit::copy (small) (UU)                      8.64 ns         8.64 ns    115207740
std::copy (small)                           14.6 ns         14.6 ns     57036393
bit::copy (large) (UU)                      2346 ns         2346 ns      6304472
std::copy (large)                          97726 ns        97721 ns        10000
bit::move (small) (UU)                      7.92 ns         7.92 ns    197443159
std::move (small)                           27.3 ns         27.3 ns    118842385
bit::move (large) (UU)                      2365 ns         2365 ns       522922
std::move (large)                          68891 ns        68888 ns       103608
bit::copy_backward (small) (UU)             9.81 ns         9.81 ns     71653257
std::copy_backward (small)                  4.50 ns         4.50 ns    256639096
bit::copy_backward (large) (UU)             4350 ns         4350 ns       222897
std::copy_backward (large)                161108 ns       161102 ns         4778
```

| Benchmark | Size  | Speedup |
|-----------|-------|---------|
|shift_left (UU) | small | 9.04x|
|shift_left (UU) | large | 556.84x|
|shift_right (UU) | small | 9.08x|
|shift_right (AA) | large | 534.04x|
|reverse (UU) | small | 4.70x|
|reverse (AA) | large | 335.55x|
|reverse (UU) | large | 223.67x|
|transform(UnaryOp) (UU) | small | 0.52x|
|transform(UnaryOp) (UU) | large | 47.38x|
|transform(BinaryOp) (UU) | small | 0.92x|
|transform(BinaryOp) (UU) | large | 129.64x|
|rotate (ARA) | small | 9.23x|
|rotate (ARA) | large | 73.98x|
|count (AA) | small | 11.23x|
|count (AA) | large | 75.51x|
|swap_ranges (UU) | small | 2.07x|
|swap_ranges (UU) | large | 142.06x|
|copy (UU) | small | 0.36x|
|copy (UU) | large | 3.29x|
|move (UU) | small | 2.70x|
|move (UU) | large | 21.48x|
|copy_backward (UU) | small | 0.83x|
|copy_backward (UU) | large | 51.39x|

