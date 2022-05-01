# BitLib

![Actions](https://github.com/bkille/BitLib/actions/workflows/cmake.yml/badge.svg?branch=master)


## Description
While [bit iterators](https://github.com/vreverdy/bit) are currently being proposed and reviewed by the ISO C++ Standards Committee, this repository is intended for practical use of bit containers and algorithms based on bit iterators. Primarily, this repository acts as an efficient replacement of `std::vector<bool>`. It provides implementations of many of the functions in `<algorithms>` optimized for containers of bits, in addition to providing a `bitvector` class, which has the same interface as `std::vector<bool>`.

Many of the implementations in `include/bit-algorithms` come from some of my previous work [here](https://github.com/vreverdy/bit-algorithms), however that repository is also somewhat frozen, as it is tied to the ISO C++ Standards Committee proposal as well. In addition, it contains many overloads that are likely less practical (like forward lists of bits). 

## Example
The code below is from `example/print.cpp`. 
```
#include <iostream>
#include "bitlib.hpp""

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

    // Same behavior as the corresponding std::vector::push_back and std::vector::insert
    bv1.push_back(bit::bit0);
    bv1.insert(bv.end(), 10, bit::bit1);
    std::cout << "Extended bitvec:  " << bv1.debug_string() << std::endl;
    // Extended bitvec:  01011111 00100111 1111111

    return 0;
}
```

Another example can be seen which showcases some of the capabilities of the `bit_iterators/` library:

```
#include <iostream>
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
    // for (bit::bit_reference<WordType> bval :  bvec) also works
    for (auto bval :  bvec) {
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


## Documentation
Given that the majority of the library is focused on having the same interface as the C++ STL iterators, containers, and algorithms, users should use the official [STL documentation website](https://en.cppreference.com/). We do plan on adding our own documentation in the future, however. 


## Performance Benchmarks
I used Google's [benchmark](https://github.com/google/benchmark) library for computing benchmarks. Each benchmark is formatted as `{bit, std}::function` (size) [(alignment-tags)].

* `bit` is for this library, `std` is the standard library operating on the infamous `vector<bool>`. 
* (size) denotes the size of the container in bits. `small = 1 << 4`, `large = 1 << 16`
* (alignment-tags) refers to the memory alignment of the bit-iterators. `U` means the iterator does not fall on a word boundary, `R` means the iterator is placed at random, and `A` means the iterator is aligned with a word boundary.

For example, `bit::rotate (large) (ARA)` refers to our library's implementation of the `rotate` algorithm operating on a container of 32768 bits, where `first` and `last` are aligned but `n_first` is selected at random.

```
Run on (80 X 1144.24 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x40)
  L1 Instruction 32 KiB (x40)
  L2 Unified 1024 KiB (x40)
  L3 Unified 28160 KiB (x2)
Load Average: 1.14, 1.20, 1.27
--------------------------------------------------------------------------
Benchmark                                Time             CPU   Iterations
--------------------------------------------------------------------------
bit::shift_left (small) (UU)          2.61 ns         2.61 ns    268554879
bit::shift_left (small) (AA)          2.71 ns         2.71 ns    257942581
std::shift_left (small)               23.5 ns         23.5 ns     29733027
bit::shift_left (large) (UU)           154 ns          154 ns      4574307
bit::shift_left (small) (AA)          2.76 ns         2.76 ns    256811575
std::shift_left (large)              97789 ns        97785 ns         7168
bit::shift_right (small) (UU)         2.45 ns         2.45 ns    286391427
std::shift_right (small)              22.2 ns         22.2 ns     31586703
bit::shift_right (large) (AA)          139 ns          139 ns      5017866
std::shift_right (large)             82441 ns        82437 ns         8491
bit::reverse (small) (UU)             5.46 ns         5.46 ns    127987521
std::reverse (small)                  25.0 ns         25.0 ns     28066883
bit::reverse (large) (AA)              580 ns          580 ns      1197374
bit::reverse (large) (UU)              765 ns          765 ns       920478
std::reverse (large)                178233 ns       178226 ns         3893
bit::rotate (small) (ARA)             5.44 ns         5.44 ns    128481219
std::rotate (small)                   57.8 ns         57.8 ns     10000000
bit::rotate (large) (ARA)             4772 ns         4772 ns       171120
std::rotate (large)                 357265 ns       357251 ns         2113
bit::count (small) (AA)               1.09 ns         1.09 ns    643063971
std::count (small)                    9.52 ns         9.52 ns     73400317
bit::count (large) (AA)                443 ns          443 ns      1576447
std::count (large)                   35990 ns        35988 ns        19427
bit::swap_ranges (small) (UU)         5.83 ns         5.83 ns    119831059
std::swap_ranges (small)              8.75 ns         8.75 ns    100000000
bit::swap_ranges (large) (UU)         2082 ns         2082 ns       527351
std::swap_ranges (large)            345883 ns       345870 ns        28154
bit::copy (small) (UU)                7.23 ns         7.23 ns     98213498
std::copy (small)                     3.83 ns         3.83 ns   1000000000
bit::copy (large) (UU)                2158 ns         2158 ns       667486
std::copy (large)                    92918 ns        92914 ns         6808
bit::move (small) (UU)                6.28 ns         6.28 ns     85940974
std::move (small)                     15.4 ns         15.4 ns     40480293
bit::move (large) (UU)                1703 ns         1703 ns      1000000
std::move (large)                    69221 ns        69218 ns        10000
bit::copy_backward (small) (UU)       8.97 ns         8.97 ns     77872677
std::copy_backward (small)            1.63 ns         1.63 ns   1000000000
bit::copy_backward (large) (UU)       2294 ns         2294 ns      1095120
std::copy_backward (large)          184672 ns       184665 ns        16136
```

| Benchmark | Size  | Speedup |
|-----------|-------|---------|
| shift     | small | 10x   |
| shift     | large | 600x    |
| reverse   | small | 4.4x    |
| reverse   | large | 258x    |
| rotate    | small | 10x    |
| rotate    | large | 70x   |
| count     | small | 9x   |
| count     | large | 80x    |
| copy      | small | 0.5x |
| copy      | large | 43x |
| copy_backward      | small | 0.15x |
| copy_backward      | large | 80x |

