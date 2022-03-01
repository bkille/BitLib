// ============================= TEST_UTILS ================================ //
// Project:         The Experimental Bit Algorithms Library
// Name:            test_utils.hpp
// Description:     General utilities for testing
// Creator:         Collin Gress
// Contributor(s):  Bryce Kille [2019], 
//                  Collin Gress [2019]
// License: BSD 3-Clause License
// ========================================================================== //
#ifndef _TEST_UTILS_HPP_INCLUDED
#define _TEST_UTILS_HPP_INCLUDED
// ========================================================================== //


// ================================ PREAMBLE ================================ //
// C++ standard library
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <list>
#include <forward_list>
// Project sources
#include "bit-iterator/bit.hpp"
// Third-party libraries
// Miscellaneous


// =============================== DEBUGGING ================================ //
// Display algorithm
template <class O, class Iterator> 
void display(O&& os, bit::bit_iterator<Iterator> first, bit::bit_iterator<Iterator> last)
{
    auto mem = first;
    for (auto it = first; it != last; ++it) {
        if (it.base() != mem.base()) {
            os << " ";
        } else if (it.position() % 8 == 0 && it.position() != 0) {
            os << '.';
        }
        std::forward<O>(os) << *it;
        mem = it;
    }
    std::forward<O>(os) << std::endl;
}

template <class O, class Iterator> 
void display(O&& os, Iterator first, Iterator last, size_t size)
{
    auto mem = first;
    auto position = 0;
    for (auto it = first; it != last; ++it) {
        if (position % size == 0 && position != 0) {
            os << " ";
        } else if (position % 8 == 0 && position != 0) {
            os << '.';
        }
        std::forward<O>(os) << *it;
        mem = it;
        ++position;
    }
    std::forward<O>(os) << std::endl;
}
// ========================================================================== //



// ================================ TESTING ================================- //
// Comparator for bit_iterator to other iterators
constexpr auto comparator = [](auto b1, auto b2){
    return static_cast<bool>(b1) == static_cast<bool>(b2);
};

// Helper struct for rebind_container
template <class Container, class NewType> struct rebind_container;

// Helper struct for rebind_container
template <class T, 
          class Alloc, 
          template <class, class, class...> class Container, 
          class NewType, 
          class... Parameters
         >
struct rebind_container<Container<T, Alloc, Parameters...>, NewType>
{
     typedef Container<NewType, typename Alloc::template rebind<NewType>::other > type;
};


// Takes in Container<T> and returns Container<bool> of the same underlying bits
template <class Container, typename T = typename Container::value_type>
auto bitcont_to_boolcont(const Container bitcont){
    auto bfirst = bit::bit_iterator<decltype(std::begin(bitcont))>(std::begin(bitcont));
    auto blast = bit::bit_iterator<decltype(std::end(bitcont))>(std::end(bitcont));
    typename rebind_container<Container, bool>::type c(std::distance(bfirst, 
                                                                     blast)
    );
    auto benchmark_it = std::begin(c);
    auto it = bfirst;
    for (; it != blast; ++it, ++benchmark_it) {
        *benchmark_it = static_cast<bool>(*it);
    }
    return c;
}

// Produces container of random numbers from min to max
template <class Container, typename T = typename Container::value_type>
Container make_random_container(
    std::size_t size, 
    T min = std::numeric_limits<T>::min(), 
    T max = std::numeric_limits<T>::max(), 
    const T& seed = T()
)
{
    Container c(size);
    std::random_device device;
    std::mt19937 engine(seed == T() ? device() : seed);
    std::uniform_int_distribution<std::uintmax_t> distribution(min, max);
    auto it = std::begin(c); 
    for (std::size_t i = 0; i < size; ++i) {
        *it = distribution(engine);
        ++it;
    }
    return c;
}
// ========================================================================== //



// =============================== BIT_STRING =============================== //
/**
 * Creates a length n string of the character '0' repeated
 * @param n the length of the string to be created
 * @return the constructed string
 */
std::string all_zero_str(std::size_t n) {
  return std::string(n, '0');
}

/**
 * Creates a length n string of the character '1' repeated
 * @param n the length of the string to be created
 * @return the constructed string
 */
std::string all_one_str(std::size_t n) {
  return std::string(n, '1');
}

/**
 * Copies a passed string and randomly sets n characters of the new 
 * string to the character passed to the function, c.
 * @param str the string to be copied and have its characters set
 * @param n the number of characters to be set (chosen at random)
 * @param c the character to set at random positions
 * @return a string with c set at n randomly selected positions
 */
std::string set_random_chars(std::string str, std::size_t n, char c) {
  std::vector<std::size_t> indices;
  for (std::size_t i = 0; i < str.size(); i++) {
    indices.push_back(i);
  }
  std::random_device device;
  std::default_random_engine engine(device());
  std::shuffle(indices.begin(), indices.end(), engine);

  for (std::size_t j = 0; j < n; j++) {
    std::size_t index = indices[j];
    str[index] = c;
  }
  return str;
}

/**
 * Copies a passed string to a new string and randomly chooses
 * n characters of the string to set to '1'.
 * @param str the string to copy from
 * @param n the number of characters to set to '1'
 * @return a string with n characters, chosen at randon, set to '1'
 */
std::string set_n_random_bits(std::string str, std::size_t n) {
  return set_random_chars(str, n, '1');
}

/**
 * Copies a passed string to a new string and randomly chooses
 * n characters of the string to set to '0'.
 * @param str the string to copy from
 * @param n the number of characters to set to '0'
 * @return a string with n characters, chosen at randon, set to '0'
 */
std::string unset_n_random_bits(std::string str, std::size_t n) {
  return set_random_chars(str, n, '0');
}

/**
 * Get a string of length len filled with 0s and 1s at random
 * @param len the length of the string to be constructed
 * @return the length len string filled with 0s and 1s
 */
std::string random_bit_str(std::size_t len) {
  std::random_device device;
  std::default_random_engine engine(device());
  std::uniform_int_distribution<int> uniform_dist(0, 1);

  std::string bit_str = "";
  for (std::size_t i = 0; i < len; i++) {
    int bit = uniform_dist(engine);
    int char_bit = '0' + bit;

    bit_str += char_bit;
  }
  return bit_str;
}

/**
 * Chooses a number uniformly at random on the range [lower_bound, upper_bound]
 * @param lower_bound the lower bound of the uniform distribution to draw from
 * @param upper_bound the upper bound of the uniform distribution to draw from
 * @return a number drawn uniformly at random on [lower_bound, upper_bound]
 */
template <class NumericType>
NumericType random_number(
  NumericType lower_bound = std::numeric_limits<NumericType>::min(),
  NumericType upper_bound = std::numeric_limits<NumericType>::max()
) {
  std::random_device device;
  std::default_random_engine engine(device());
  std::uniform_int_distribution<NumericType> numeric_dist(lower_bound, upper_bound);

  return numeric_dist(engine);
}

/**
 * Returns a vector of length len filled with random numbers drawn uniformly
 * @param len the number of elements in the vector
 * @return the vector of random numbers
 */
template <class NumericType>
std::vector<NumericType> get_random_vector(std::size_t len) {
  std::vector<NumericType> vec;
  for (std::size_t i = 0; i < len; i++) {
    vec.push_back(random_number<std::size_t>());
  }
  return vec;
}

/**
 * Converts a string of '0' and '1' to the decimal equivalent
 * @param str a string filled with the characters '0' and '1'
 * @return the decimal equivalent of str
 */
template <class NumericType>
NumericType string_as_bits(std::string str) {
  NumericType num = 0;

  auto begin = bit::bit_iterator<NumericType*>(&num, 0);
  auto end = bit::bit_iterator<NumericType*>(&num + 1, 0);

  for (auto it = begin; it != end; ++it) {
    std::size_t index = std::distance(begin, it);

    if (index > str.size() - 1) {
      break;
    }

    char cur = str[index];

    assert(cur == '0' || cur == '1');

    if (cur == '0') {
      *it = bit::bit0;
    } else {
      *it = bit::bit1;
    }
  }

  return num;
}
// ========================================================================== //
#endif // _TEST_UTILS_HPP_INCLUDED
// ========================================================================== //
