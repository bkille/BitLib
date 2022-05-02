# BitLib

![Actions](https://github.com/bkille/BitLib/actions/workflows/cmake.yml/badge.svg?branch=master)[![Coverage Status](https://coveralls.io/repos/github/bkille/BitLib/badge.svg?branch=master)](https://coveralls.io/github/bkille/BitLib?branch=master)


## Description
While [bit iterators](https://github.com/vreverdy/bit) are currently being proposed and reviewed by the ISO C++ Standards Committee, this repository is intended for practical use of bit containers and algorithms based on bit iterators. Primarily, this repository acts as an efficient replacement of `std::vector<bool>`. It provides implementations of many of the functions in `<algorithms>` optimized for containers of bits, in addition to providing a `bitvector` class, which has the same interface as `std::vector<bool>`.

Many of the implementations in `include/bit-algorithms` come from some of my previous work [here](https://github.com/vreverdy/bit-algorithms), however that repository is also somewhat frozen, as it is tied to the ISO C++ Standards Committee proposal as well. In addition, it contains many overloads that are likely less practical (like forward lists of bits). 

## Example
The code below is from `example/print.cpp`. 
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
I used Google's [benchmark](https://github.com/google/benchmark) library for computing benchmarks. Each benchmark is formatted as `{bit, BitArray, std}::function` (size) [(alignment-tags)].

* `bit` is for this library, `BitArray` is for the popular C-based [BitArray library](https://github.com/noporpoise/BitArray), and`std` is the standard library operating on the infamous `vector<bool>`. 
* (size) denotes the size of the container in bits. `small = 1 << 4`, `large = 1 << 16`
* (alignment-tags) refers to the memory alignment of the bit-iterators. `U` means the iterator does not fall on a word boundary, `R` means the iterator is placed at random, and `A` means the iterator is aligned with a word boundary.

For example, `bit::rotate (large) (ARA)` refers to our library's implementation of the `rotate` algorithm operating on a container of 32768 bits, where `first` and `last` are aligned but `n_first` is selected at random.

```
2022-05-02T12:09:20-05:00
Running ./bin/bench
Run on (80 X 1354 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x40)
  L1 Instruction 32 KiB (x40)
  L2 Unified 1024 KiB (x40)
  L3 Unified 28160 KiB (x2)
Load Average: 0.58, 0.62, 2.34
--------------------------------------------------------------------------
Benchmark                                Time             CPU   Iterations
--------------------------------------------------------------------------
bit::shift_left (small) (UU)          2.62 ns         2.62 ns    267553966
bit::shift_left (small) (AA)          1.63 ns         1.63 ns    430281940
std::shift_left (small)               23.6 ns         23.6 ns     29634990
bit::shift_left (large) (UU)           440 ns          439 ns      1593451
bit::shift_left (small) (AA)          1.63 ns         1.63 ns    430463509
std::shift_left (large)              97708 ns        97704 ns         7159
bit::shift_right (small) (UU)         2.44 ns         2.44 ns    286737201
std::shift_right (small)              22.7 ns         22.7 ns     30796064
bit::shift_right (large) (AA)          395 ns          395 ns      1772955
BitArray::shift_right (large)         1624 ns         1624 ns       429537
std::shift_right (large)             82238 ns        82233 ns         8501
bit::reverse (small) (UU)             5.44 ns         5.44 ns    128669666
std::reverse (small)                  25.5 ns         25.5 ns     27776313
bit::reverse (large) (AA)              587 ns          587 ns      1208016
bit::reverse (large) (UU)             1300 ns         1300 ns       534821
BitArray::reverse (large)            10238 ns        10237 ns        68383
std::reverse (large)                183020 ns       183007 ns         3820
bit::rotate (small) (ARA)             5.42 ns         5.42 ns    215203657
std::rotate (small)                   40.4 ns         40.4 ns    100000000
bit::rotate (large) (ARA)             4953 ns         4952 ns       147007
BitArray::rotate (large)             22157 ns        22156 ns        31641
std::rotate (large)                 361537 ns       361514 ns         2248
bit::count (small) (AA)               1.64 ns         1.64 ns    427770306
std::count (small)                    9.49 ns         9.49 ns     73793787
bit::count (large) (AA)                356 ns          356 ns      1968868
BitArray::count (large)                568 ns          568 ns      1233047
std::count (large)                   35838 ns        35836 ns        19533
bit::swap_ranges (small) (UU)         5.43 ns         5.43 ns    135105498
std::swap_ranges (small)              6.38 ns         6.38 ns   1000000000
bit::swap_ranges (large) (UU)         2537 ns         2537 ns       723238
std::swap_ranges (large)            155129 ns       155123 ns         3703
bit::copy (small) (UU)                7.33 ns         7.33 ns    198320679
std::copy (small)                     5.81 ns         5.81 ns   1000000000
bit::copy (large) (UU)                1529 ns         1529 ns       500483
std::copy (large)                   100334 ns       100329 ns        13093
bit::move (small) (UU)                6.66 ns         6.66 ns    104952741
std::move (small)                     19.2 ns         19.2 ns     33316110
bit::move (large) (UU)                2706 ns         2706 ns      2597871
std::move (large)                    58536 ns        58533 ns        87746
bit::copy_backward (small) (UU)       8.16 ns         8.16 ns     85710144
std::copy_backward (small)            4.16 ns         4.16 ns    759646390
bit::copy_backward (large) (UU)       2803 ns         2803 ns       179331
std::copy_backward (large)          191787 ns       191779 ns         2926
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

