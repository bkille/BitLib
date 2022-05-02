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
#   define SIMDPP_ARCH_X86_AVX512BW true
#elif __AVX512DQ__
#   define SIMDPP_ARCH_X86_AVX512DQ true
#elif __AVX512VL__
#   define SIMDPP_ARCH_X86_AVX512VL true
#elif __AVX512F__
#   define SIMDPP_ARCH_X86_AVX512DQ true
#elif __AVX2__
#   define SIMDPP_ARCH_X86_AVX2 true
#elif __AVX__
#   define SIMDPP_ARCH_X86_AVX true
#elif __SSE4_1__
#   define SIMDPP_ARCH_X86_SSE4_1 true
#elif __SSE3__
#   define SIMDPP_ARCH_X86_SSE3 true
#elif __SSE2__
#   define SIMDPP_ARCH_X86_SSE2 true
#endif
// ========================================================================== //



// ================================ PREAMBLE ================================ //
#include "debug_utils.hpp" //TODO does this belong somewhere else?
#include "bit_algorithm_details.hpp"
// <algorithm> overloads
#include "bit_algorithm_details.hpp"
#include "copy_backward.hpp"
#include "copy.hpp"
#include "count.hpp"
#include "debug_utils.hpp"
#include "fill.hpp"
#include "find.hpp"
#include "move.hpp"
#include "reverse.hpp"
#include "rotate.hpp"
#include "shift.hpp"
#include "swap_ranges.hpp"
#include "type_traits.hpp"
// ========================================================================== //



// ========================================================================== //
#endif // _BIT_ALGORITHM_HPP_INCLUDED
// ========================================================================== //
