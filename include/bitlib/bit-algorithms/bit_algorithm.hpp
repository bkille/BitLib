// ============================= BIT_ALGORITHM ============================== //
// Project:         The C++ Bit Library
// Name:            bit_algorithm.hpp
// Description:     Optimized versions of algorithms for bit manipulation
// Creator:         Vincent Reverdy
// Contributor(s):  Vincent Reverdy [2015-2017]
//                  Maghav Kumar [2016-2017]
//                  Bryce Kille [2019]
//                  Collin Gress [2019]
// License:         BSD 3-Clause License
// ========================================================================== //
#ifndef _BIT_ALGORITHM_HPP_INCLUDED
#define _BIT_ALGORITHM_HPP_INCLUDED
#define _Bit_Algorithm_VERSION_MAJOR @Bit_Algorithm_VERSION_MAJOR@
#define _Bit_Algorithm_VERSION_MINOR @Bit_Algorithm_VERSION_MINOR@
#if __AVX512BW__
#   pragma message("Building with AVX512BW support")
#   define SIMDPP_ARCH_X86_AVX512BW true
#elif __AVX512DQ__
#   pragma message("Building with AVX512DQ support")
#   define SIMDPP_ARCH_X86_AVX512DQ true
#elif __AVX512VL__
#   pragma message("Building with AVX512VL support")
#   define SIMDPP_ARCH_X86_AVX512VL true
#elif __AVX512F__
#   pragma message("Building with AVX512F support")
#   define SIMDPP_ARCH_X86_AVX512DQ true
#elif __AVX2__
#   pragma message("Building with AVX2 support")
#   define SIMDPP_ARCH_X86_AVX2 true
#elif __AVX__
#   pragma message("Building with AVX support")
#   define SIMDPP_ARCH_X86_AVX true
#elif __SSE4_1__
#   pragma message("Building with SSE4.1 support")
#   define SIMDPP_ARCH_X86_SSE4_1 true
#elif __SSE3__
#   pragma message("Building with SSE3 support")
#   define SIMDPP_ARCH_X86_SSE3 true
#elif __SSE2__
#   pragma message("Building with SSE2 support")
#   define SIMDPP_ARCH_X86_SSE2 true
#else
#  pragma message("Building with no vectorization")
#endif
// ========================================================================== //



// ================================ PREAMBLE ================================ //
#include "debug_utils.hpp" //TODO does this belong somewhere else?
#include "bit_algorithm_details.hpp"
// <algorithm> overloads
#include "all_of.hpp"
#include "any_of.hpp"
#include "bit_algorithm_details.hpp"
#include "bit_algorithm.hpp"
#include "copy_backward.hpp"
#include "copy.hpp"
#include "copy_n.hpp"
#include "count.hpp"
#include "debug_utils.hpp"
#include "equal.hpp"
#include "equal_range.hpp"
#include "fill.hpp"
#include "fill_n.hpp"
#include "find_end.hpp"
#include "find_first_of.hpp"
#include "find.hpp"
#include "for_each.hpp"
#include "for_each_n.hpp"
#include "includes.hpp"
#include "inner_product.hpp"
#include "move_backward.hpp"
#include "move.hpp"
#include "next_permutation.hpp"
#include "none_of.hpp"
#include "reduce.hpp"
#include "replace_copy.hpp"
#include "replace.hpp"
#include "reverse_copy.hpp"
#include "reverse.hpp"
#include "rotate.hpp"
#include "search.hpp"
#include "search_n.hpp"
#include "shift.hpp"
#include "shuffle.hpp"
#include "swap.hpp"
#include "swap_ranges.hpp"
#include "transform_exclusive_scan.hpp"
#include "transform.hpp"
#include "transform_inclusive_scan.hpp"
#include "transform_reduce.hpp"
#include "type_traits.hpp"
// ========================================================================== //



// ========================================================================== //
#endif // _BIT_ALGORITHM_HPP_INCLUDED
// ========================================================================== //
