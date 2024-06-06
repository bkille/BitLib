// ============================== TYPE TRAITS =============================== //
// Project:         The Experimental Bit Algorithms Library
// Name:            type_traits.hpp
// Description:     Type traits for bits
// Contributor(s):  Vincent Reverdy [2019]
// License:         BSD 3-Clause License
// ========================================================================== //
#ifndef _TYPE_TRAITS_HPP_INCLUDED
#define _TYPE_TRAITS_HPP_INCLUDED
// ========================================================================== //



// ================================ PREAMBLE ================================ //
// C++ standard library
// Project sources
#include "bit_algorithm_details.hpp"
// Third-party libraries
// Miscellaneous
namespace bit {
// ========================================================================== //



/* *********************************** BIT ********************************** */
// Bit structure definition
struct bit
{
};
/* ************************************************************************** */



/* ********************************* IS BIT ********************************* */
// Is bit structure definition
template <class T>
struct is_bit
: std::false_type
{
};

// Is bit structure specialization: bit
template <>
struct is_bit<::bit::bit>
: std::true_type
{
};

// Is bit structure specialization: bit value
template <>
struct is_bit<bit_value>
: std::true_type
{
};

// Is bit structure specialization: bit reference
template <class WordType>
struct is_bit<bit_reference<WordType>>
: std::true_type
{
};

// Is bit value template definition
template <class T>
inline constexpr bool is_bit_v = is_bit<T>::value;
/* ************************************************************************** */



/* ***************************** IS ITERATOR ******************************** */
template<typename T, typename = void>
struct is_iterator : std::false_type {};

template<typename T>
struct is_iterator<
    T,
    std::void_t<typename std::iterator_traits<T>::value_type>
> : std::true_type {};

template <class T>
inline constexpr bool is_iterator_v = is_iterator<T>::value;
/* ************************************************************************** */



/* *************************** IS FORWARD ITERATOR ************************** */
template<typename T, typename = void>
struct is_forward_iterator : std::false_type {};

template<typename T>
struct is_forward_iterator<
    T,
    std::enable_if_t<
        std::is_base_of_v<
            std::forward_iterator_tag,
            typename std::iterator_traits<T>::iterator_category
        >
    >
> : std::true_type {};

template <class T>
inline constexpr bool is_forward_iterator_v = is_forward_iterator<T>::value;
/* ************************************************************************** */



/* ********************** IS BIDIRECTIONAL ITERATOR ************************* */
template<typename T, typename = void>
struct is_bidirectional_iterator : std::false_type {};

template<typename T>
struct is_bidirectional_iterator<
    T,
    std::enable_if_t<
        std::is_base_of_v<
            std::bidirectional_iterator_tag,
            typename std::iterator_traits<T>::iterator_category
        >
    >
> : std::true_type {};

template <class T>
inline constexpr bool is_bidirectional_iterator_v
    = is_bidirectional_iterator<T>::value;
/* ************************************************************************** */



/* *********************** IS RANDOM ACCESS ITERATOR ************************ */
template<typename T, typename = void>
struct is_random_access_iterator : std::false_type {};

template<typename T>
struct is_random_access_iterator<
    T,
    std::enable_if_t<
        std::is_base_of_v<
            std::random_access_iterator_tag,
            typename std::iterator_traits<T>::iterator_category
        >
    >
> : std::true_type {};

template <class T>
inline constexpr bool is_random_access_iterator_v =
    is_random_access_iterator<T>::value;
/* ************************************************************************** */



/* *************************** IS INPUT ITERATOR **************************** */
template<typename T, typename = void>
struct is_input_iterator : std::false_type {};

template<typename T>
struct is_input_iterator<
    T,
    std::enable_if_t<
        std::is_base_of_v<
            std::input_iterator_tag,
            typename std::iterator_traits<T>::iterator_category
        >
    >
> : std::true_type {};

template <class T>
inline constexpr bool is_input_iterator_v = is_input_iterator<T>::value;
/* ************************************************************************** */



/* ************************** IS OUTPUT ITERATOR **************************** */
template<typename T, typename = void>
struct is_output_iterator : std::false_type {};

template<typename T>
struct is_output_iterator<
    T,
    std::enable_if_t<
        std::is_base_of_v<
            std::output_iterator_tag,
            typename std::iterator_traits<T>::iterator_category
        >
    >
> : std::true_type {};

template <class T>
inline constexpr bool is_output_iterator_v = is_output_iterator<T>::value;
/* ************************************************************************** */



// ========================================================================== //
} // namespace bit
#endif // _TYPE_TRAITS_HPP_INCLUDED
// ========================================================================== //

