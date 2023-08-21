// ================================= COUNT ================================== //
// Project: The Experimental Bit Algorithms Library
// Description: bit_iterator overloads for std::count, std::count_if
// License: BSD 3-Clause License
// ========================================================================== //
#ifndef _COUNT_HPP_INCLUDED
#define _COUNT_HPP_INCLUDED
// ========================================================================== //

// ============================== PREAMBLE ================================== //
// C++ standard library
#include <iterator>
#include <numeric>
// Project sources
#include "bitlib/bit-iterator/bit.hpp"
#include "bitlib/bit-algorithms//libpopcnt.h"
// Third-party libraries
#ifdef BITLIB_HWY
#include "hwy/highway.h"
#endif
// Miscellaneous

#define is_aligned(POINTER, BYTE_COUNT) \
    (((uintptr_t)(const void *)(POINTER)) % (BYTE_COUNT) == 0)

namespace bit {

#ifdef BITLIB_HWY
namespace hn = hwy::HWY_NAMESPACE;
#endif
// ========================================================================== //

template<class RandomAccessIt>
constexpr typename bit_iterator<RandomAccessIt>::difference_type
count(
    bit_iterator <RandomAccessIt> first,
    bit_iterator <RandomAccessIt> last,
    bit_value value
) {
    // Assertions
    _assert_range_viability(first, last);

    // Types and constants
    using word_type = typename bit_iterator<RandomAccessIt>::word_type;
    using difference_type = typename bit_iterator<RandomAccessIt>::difference_type;
    constexpr difference_type digits = binary_digits<word_type>::value;

    // Initialization
    difference_type result = 0;

    // Computation when bits belong to several underlying words
    if (first.base() != last.base()) {
        RandomAccessIt it = first.base();

        if (first.position() != 0) {
            word_type first_value = *first.base() >> first.position();
            result = _popcnt(first_value);
            ++it;
        }
// The SIMD implementation here is actually slower than the standard
//#ifdef BITLIB_HWY
        //// ReduceSum not implemented for unsigned char
        //if constexpr (digits > 8)
        //{
            //// Align to boundary
            //for (; it != last.base() && !is_aligned(&(*it), 64); ++it) {
                //result += _popcnt(*it);
            //}

            //// SIMD
            //hn::ScalableTag<word_type> d;
            //for (; std::distance(it, last.base()) >= hn::Lanes(d); it += hn::Lanes(d))
            //{
                //const auto popcntV = hn::PopulationCount(hn::Load(d, &*it));
                //result += hn::ReduceSum(d, popcntV);
            //}

            //// Remaining
            //for (; it != last.base(); ++it) {
                //result += _popcnt(*it);
            //}
        //} else
//#endif
        {
            //result += std::transform_reduce(
                    //it,
                    //last.base(),
                    //0,
                    //std::plus{},
                    //[](word_type word) {return popcnt(word); }
            //);
            result += popcnt(&*it, std::distance(it, last.base()));
        }
        if (last.position() != 0) {
            word_type last_value = *last.base() << (digits - last.position());
            result += _popcnt(last_value);
        }
    // Computation when bits belong to the same underlying word
    } else {
        result = _popcnt(
            _bextr<word_type>(*first.base(), first.position(), last.position()
              - first.position())
        );
    }

    // Negates when the number of zero bits is requested
    if (!static_cast<bool>(value)) {
        result = std::distance(first, last) - result;
    }

    // Finalization
    return result;
}

} // namespace bit

// ========================================================================== //
#endif // _COUNT_HPP_INCLUDED
// ========================================================================== //
