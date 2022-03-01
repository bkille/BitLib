# BitLib

![Actions](https://github.com/bkille/BitLib/actions/workflows/cmake.yml/badge.svg)


## Description
While [bit iterators](https://github.com/vreverdy/bit) are currently being proposed and reviewed by the ISO C++ Standards Committee, this repository is intended for practical use of bit containers and algorithms based on bit iterators. Primarily, this repository acts as an efficient replacement of `std::vector<bool>`. It provides implementations of many of the functions in `<algorithms>` optimized for containers of bits, in addition to providing a `bitvector` class, which has the same interface as `std::vector<bool>`.

Many of the implementations in `include/bit-algorithms` come from some of my previous work [here](https://github.com/vreverdy/bit-algorithms), however that repository is also somewhat frozen, as it is tied to the ISO C++ Standards Committee proposal as well. In addition, it contains many overloads that are likely less practical (like forward lists of bits). 

## Example
The code below is from `example/print.cpp`. 
```
#include <iostream>
#include "bit-iterator/bit.hpp"
#include "bit-algorithms/bit_algorithm.hpp"
#include "bit-containers/bitvector.hpp"


int main() {
    bit::bit_vector<unsigned char> bv1 {"01001010010"};
    std::cout << bv1 << std::endl;
    return 0;
}
```

When executed, the following is printed:
```
01001010 010
```


## Performance Benchmarks
I used Google's [benchmark](https://github.com/google/benchmark) library for computing benchmarks. Each benchmark is formatted as `{bit, std}::function` (size) [(alignment-tags)].

* `bit` is for this library, `std` is the standard library operating on the infamous `vector<bool>`. 
* (size) denotes the size of the container in bits. `small = 1 << 4`, `large = 1 << 16`
* (alignment-tags) refers to the memory alignment of the bit-iterators. `U` means the iterator does not fall on a word boundary, `R` means the iterator is placed at random, and `A` means the iterator is aligned with a word boundary.

For example, `bit::rotate (large) (ARA)` refers to our library's implementation of the `rotate` algorithm operating on a container of 32768 bits, where `first` and `last` are aligned but `n_first` is selected at random.

```
Run on (64 X 2300 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x32)
  L1 Instruction 32 KiB (x32)
  L2 Unified 1024 KiB (x32)
  L3 Unified 22528 KiB (x2)
Load Average: 0.41, 0.67, 0.73
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
--------------------------------------------------------------------
Benchmark                          Time             CPU   Iterations
--------------------------------------------------------------------
bit::shift_left (small) (UU)        3.77 ns         3.77 ns    185649261
std::shift_left (small)             45.6 ns         45.6 ns     15359502
bit::shift_left (large) (AA)         254 ns          254 ns      2760779
std::shift_left (large)           195023 ns       195021 ns         3590
bit::shift_right (small) (UU)       3.77 ns         3.77 ns    185654156
std::shift_right (small)            34.6 ns         34.6 ns     20218713
bit::shift_right (large) (AA)        240 ns          240 ns      2921340
std::shift_right (large)          131946 ns       131946 ns         5294
bit::reverse (small) (UU)           8.87 ns         8.87 ns     78851173
std::reverse (small)                39.1 ns         39.1 ns     17922588
bit::reverse (large) (AA)            962 ns          962 ns       727702
std::reverse (large)              246764 ns       246770 ns         2844
bit::rotate (small) (ARA)           6.76 ns         6.76 ns    105597804
std::rotate (small)                 71.5 ns         71.5 ns      8798226
bit::rotate (large) (ARA)           7577 ns         7577 ns        93948
std::rotate (large)               505214 ns       505212 ns         1383
bit::count (small) (AA)             1.16 ns         1.16 ns    603331880
std::count (small)                  17.9 ns         17.9 ns     39219825
bit::count (large) (AA)              664 ns          664 ns      1054400
std::count (large)                 72859 ns        72860 ns         9610
```

| Benchmark | Size  | Speedup |
|-----------|-------|---------|
| shift     | small | 10.6x   |
| shift     | large | 662x    |
| reverse   | small | 4.4x    |
| reverse   | large | 258x    |
| rotate    | small | 9.7x    |
| rotate    | large | 66.7x   |
| count     | small | 15.4x   |
| count     | large | 110x    |

