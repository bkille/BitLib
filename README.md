# BitLib

![Actions](https://github.com/bkille/BitLib/actions/workflows/cmake.yml/badge.svg?branch=master)[![Coverage Status](https://coveralls.io/repos/github/bkille/BitLib/badge.svg?branch=master)](https://coveralls.io/github/bkille/BitLib?branch=master)


## Description
While [bit iterators](https://github.com/vreverdy/bit) are currently being proposed and reviewed by the ISO C++ Standards Committee, this repository is intended for practical use of bit containers and algorithms based on bit iterators. Primarily, this repository acts as an efficient replacement of `std::vector<bool>`. It provides implementations of many of the functions in `<algorithms>` optimized for containers of bits, in addition to providing a `bit_vector` class which has roughly 🚡the same interface as `std::vector<bool>`.

Many of the implementations in `include/bit-algorithms` come from some of my previous work [here](https://github.com/vreverdy/bit-algorithms), however that repository is also somewhat frozen, as it is tied to the ISO C++ Standards Committee proposal as well. In addition, it contains many overloads that are likely less practical (like forward lists of bits). 

## Installation
BitLib is a header-only libarary. In order to use BitLib, just make sure the `include/` folder is added compiler's search path. 


## Example
The code below is from `example/print.cpp`. While the type of word that the bitvector is built off of is templated and you can use any unsigned type, it is likely that you'll want to use `uint64_t` or another 64 bit unsigned type, as that will leverage the most bit-parallelism.
```cpp
#include <iostream>
#include "bitlib.hpp"

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
// and behave as expected
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
The goal of BitLib is to be as similar to the C++ STL as possible. The interface of most functions and classes are the same as they are in the STL. 

### Containers
 Right now, the only container I have implemented is the bitvector. `bit::vector<WordType>` is essentially a wrapper around `std::vector<WordType>`. The interfaces are nearly identical. In addition to the normal `vector` constructors, you can also provide a string to construct your bitvector:
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

### Iterators
The bit-iterators are the foundation of the library. In most cases, users will only need to work w/ the `bit::bit_vector::begin()` and `bit::bit_vector::end()` methods to obtain iterators. However, constructing a bit iterator from any address is also straightforward:
```
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

For example, `bit::rotate (large) (ARA)` refers to our library's implementation of the `rotate` algorithm operating on a container of 32768 bits, where `first` and `last` are aligned but `n_first` is selected at random.

```
2022-05-02T12:15:34-05:00
Running ./bin/bench
Run on (80 X 1439.35 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x40)
  L1 Instruction 32 KiB (x40)
  L2 Unified 1024 KiB (x40)
  L3 Unified 28160 KiB (x2)
Load Average: 0.18, 0.30, 1.63
--------------------------------------------------------------------------
Benchmark                                Time             CPU   Iterations
--------------------------------------------------------------------------
bit::shift_left (small) (UU)          2.61 ns         2.61 ns    268071890
bit::shift_left (small) (AA)          2.98 ns         2.98 ns    234664285
std::shift_left (small)               23.5 ns         23.5 ns     29816869
bit::shift_left (large) (UU)           162 ns          162 ns      4297326
bit::shift_left (small) (AA)          2.98 ns         2.98 ns    234698017
std::shift_left (large)              97663 ns        97659 ns         7170
bit::shift_right (small) (UU)         2.44 ns         2.44 ns    286670520
std::shift_right (small)              22.2 ns         22.2 ns     31539227
bit::shift_right (large) (AA)          141 ns          141 ns      4956794
BitArray::shift_right (large)         1574 ns         1574 ns       444657
std::shift_right (large)             82202 ns        82198 ns         8504
bit::reverse (small) (UU)             5.44 ns         5.44 ns    128267079
std::reverse (small)                  24.9 ns         24.9 ns     26432325
bit::reverse (large) (AA)              580 ns          580 ns      1209237
bit::reverse (large) (UU)              748 ns          748 ns       937796
BitArray::reverse (large)            10200 ns        10200 ns        68540
std::reverse (large)                159196 ns       159190 ns         4390
bit::rotate (small) (ARA)             5.43 ns         5.43 ns    128867785
std::rotate (small)                   25.5 ns         25.5 ns     27502077
bit::rotate (large) (ARA)             4811 ns         4810 ns       146478
BitArray::rotate (large)             21985 ns        21984 ns        31790
std::rotate (large)                 369593 ns       369577 ns         2262
bit::count (small) (AA)               1.90 ns         1.90 ns    368687307
std::count (small)                    9.22 ns         9.22 ns     75829944
bit::count (large) (AA)                449 ns          449 ns      1552065
BitArray::count (large)               1023 ns         1023 ns       684351
std::count (large)                   35858 ns        35857 ns        19474
bit::swap_ranges (small) (UU)         5.69 ns         5.69 ns    122994258
std::swap_ranges (small)              26.3 ns         26.3 ns    100000000
bit::swap_ranges (large) (UU)         1478 ns         1478 ns       398502
std::swap_ranges (large)            264861 ns       264851 ns         2535
bit::copy (small) (UU)                6.53 ns         6.53 ns    106877661
std::copy (small)                     7.69 ns         7.69 ns    100000000
bit::copy (large) (UU)                 555 ns          555 ns      2496160
std::copy (large)                    97572 ns        97568 ns        10000
bit::move (small) (UU)                6.57 ns         6.57 ns    106398702
std::move (small)                     7.70 ns         7.70 ns    120517641
bit::move (large) (UU)                1063 ns         1063 ns       941543
std::move (large)                    73594 ns        73591 ns        25308
bit::copy_backward (small) (UU)       8.82 ns         8.82 ns    100000000
std::copy_backward (small)            5.32 ns         5.32 ns   1000000000
bit::copy_backward (large) (UU)       3104 ns         3104 ns       291662
std::copy_backward (large)          185251 ns       185244 ns         3132
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

