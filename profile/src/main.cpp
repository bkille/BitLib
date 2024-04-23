// =============================== TEST ROOT ================================ //
// Project:         The Experimental Bit Algorithms Library
// Name:            test_root.cc
// Description:     Brings in all of the test headers into an object to be linked
//                  with the test main 
// Creator:         Vincent Reverdy
// Contributor(s):  Bryce Kille [2019]
// License:         BSD 3-Clause License
// ========================================================================== //



// ============================== PREAMBLE ================================== //
// C++ standard library
#include <tuple>
#include <cstdint>
#include <vector>
#include <list>
#include <forward_list>
// Project sources
#include "bitlib/bitlib.hpp"
#include "test_utils.hpp"
// Third party libraries
#include <iostream>
#include <cstdlib>
#include <memory>
#include <cxxabi.h>
#include <string>
#include <string_view>
#include <typeinfo>
// ========================================================================== //



int main()
{
    using container_type = std::vector<uint64_t>;
    const int container_size = 1 << 24;
    container_type bitcont = make_random_container<container_type>(container_size); 
    auto first = bit::bit_iterator<decltype(std::begin(bitcont))>(std::begin(bitcont));
    auto last = bit::bit_iterator<decltype(std::end(bitcont))>(std::end(bitcont));
    auto n = 1 << 10;
    for (int i = 0; i < 100; i++)
    {
        if (i % 10 == 0)
            std::cerr << i << "\n";
        bit::shift_left(first + 2, last, n + 4);
    }
    return 0;
}
