// =============================== BIT VALUE ================================ //
// Project:         The C++ Bit Library
// Name:            bit_value.hpp
// Description:     A class representing an independent, non-referenced bit
// Creator:         Vincent Reverdy
// Contributor(s):  Vincent Reverdy [2015-2017]
// License:         BSD 3-Clause License
// ========================================================================== //
#ifndef _BIT_VALUE_HPP_INCLUDED
#define _BIT_VALUE_HPP_INCLUDED
// ========================================================================== //



// ================================ PREAMBLE ================================ //
// C++ standard library
// Project sources
#include "bit_details.hpp"
// Third-party libraries
// Miscellaneous
namespace bit {
// ========================================================================== //



/* ******************************* BIT VALUE ******************************** */
// Bit value class definition
class bit_value
{
    // Friendship
    template <class> friend class bit_reference;
    
    // Types
    public:
    using size_type = std::size_t;
    
    // Lifecycle
    public:
    constexpr bit_value() noexcept;
    template <class T> 
    constexpr bit_value(bit_reference<T> ref) noexcept;
    template <class WordType> 
    explicit constexpr bit_value(WordType val) noexcept;
    explicit constexpr bit_value(bool val) noexcept;
    template <class WordType> 
    constexpr bit_value(WordType val, size_type pos);
    
    // Assignment
    public:
    template <class T> 
    constexpr bit_value& operator=(bit_reference<T> ref) noexcept;
    template <class WordType> 
    constexpr bit_value& assign(WordType val) noexcept;
    constexpr bit_value& assign(bool val) noexcept;
    template <class WordType> 
    constexpr bit_value& assign(WordType val, size_type pos);

    // Bitwise assignment operators
    public:
    constexpr bit_value& operator&=(bit_value other) noexcept;
    constexpr bit_value& operator|=(bit_value other) noexcept;
    constexpr bit_value& operator^=(bit_value other) noexcept;

    // Conversion
    public:
    constexpr operator bool() const noexcept;
    
    // Swap members
    public:
    void swap(bit_value& other) noexcept;
    template <class T> 
    void swap(bit_reference<T> other) noexcept;

    // Bit manipulation
    public:
    constexpr bit_value& set(bool b) noexcept;
    constexpr bit_value& set() noexcept;
    constexpr bit_value& reset() noexcept;
    constexpr bit_value& flip() noexcept;

    // Implementation details: data members
    private:
    bool _value;
    
    // Bitwise operators
    public:
    friend constexpr bit_value operator~(
        bit_value rhs
    ) noexcept;
    friend constexpr bit_value operator&(
        bit_value lhs, 
        bit_value rhs
    ) noexcept;
    friend constexpr bit_value operator|(
        bit_value lhs, 
        bit_value rhs
    ) noexcept;
    friend constexpr bit_value operator^(
        bit_value lhs, 
        bit_value rhs
    ) noexcept;

    // Comparison operators
    public:
    friend constexpr bool operator==(
        bit_value lhs, 
        bit_value rhs
    ) noexcept;
    friend constexpr bool operator!=(
        bit_value lhs, 
        bit_value rhs
    ) noexcept;
    friend constexpr bool operator<(
        bit_value lhs, 
        bit_value rhs
    ) noexcept;
    friend constexpr bool operator<=(
        bit_value lhs, 
        bit_value rhs
    ) noexcept;
    friend constexpr bool operator>(
        bit_value lhs, 
        bit_value rhs
    ) noexcept;
    friend constexpr bool operator>=(
        bit_value lhs, 
        bit_value rhs
    ) noexcept;
};

// Stream functions
template <class CharT, class Traits>
std::basic_istream<CharT, Traits>& operator>>(
    std::basic_istream<CharT, Traits>& is,
    bit_value& x
);
template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(
    std::basic_ostream<CharT, Traits>& os,
    bit_value x
);

// Constants
/* ************************************************************************** */



// -------------------------- BIT VALUE: LIFECYCLE -------------------------- //
// Implicitly default constructs a bit value initialized to zero
constexpr bit_value::bit_value(
) noexcept
: _value(false)
{
}

// Implicitly constructs a bit value from a bit reference
template <class T> 
constexpr bit_value::bit_value(
    bit_reference<T> ref
) noexcept
: _value(static_cast<bool>(ref))
{
}
    
// Explicitly constructs an aligned bit value
template <class WordType>
constexpr bit_value::bit_value(
    WordType val
) noexcept
: _value(val & 1)
{
    static_assert(binary_digits<WordType>::value, "");
}

// Explicitly constructs an unaligned bit value
template <class WordType> 
constexpr bit_value::bit_value(
    WordType val, 
    size_type pos
)
: _value((assert(pos < binary_digits<WordType>::value), val >> pos & 1))
{
    static_assert(binary_digits<WordType>::value, "");
}
// -------------------------------------------------------------------------- //



// ------------------------- BIT VALUE: ASSIGNMENT -------------------------- //
// Assigns a bit reference to the bit value
template <class T> 
constexpr bit_value& bit_value::operator=(
    bit_reference<T> ref
) noexcept
{
    _value = static_cast<bool>(ref);
    return *this;
}

// Assigns the aligned bit of a value to the bit value
template <class WordType> 
constexpr bit_value& bit_value::assign(
    WordType val
) noexcept
{
    static_assert(binary_digits<WordType>::value, "");
    _value = val & 1;
    return *this;
}

// Assigns an unaligned bit of a value to the bit value
template <class WordType> 
constexpr bit_value& bit_value::assign(
    WordType val, 
    size_type pos
)
{
    assert(pos < binary_digits<WordType>::value);
    _value = val >> pos & 1;
    return *this;
}
// -------------------------------------------------------------------------- //



// ---------------- BIT VALUE: BITWISE ASSIGNMENT OPERATORS ----------------- //
// Assigns the value of the bit through a bitwise and operation
constexpr bit_value& bit_value::operator&=(
    bit_value other
) noexcept
{
    _value &= other._value;
    return *this;
}

// Assigns the value of the bit through a bitwise or operation
constexpr bit_value& bit_value::operator|=(
    bit_value other
) noexcept
{
    _value |= other._value;
    return *this;
}

// Assigns the value of the bit through a bitwise xor operation
constexpr bit_value& bit_value::operator^=(
    bit_value other
) noexcept
{
    _value ^= other._value;
    return *this;
}
// -------------------------------------------------------------------------- //



// ------------------------- BIT VALUE: CONVERSION -------------------------- //
// Implicitly converts the bit value to a boolean value
constexpr bit_value::operator bool(
) const noexcept
{
    return _value;
}
// -------------------------------------------------------------------------- //



// ------------------------ BIT VALUE: SWAP MEMBERS ------------------------- //
// Swaps the bit value with another bit value
inline void bit_value::swap(
    bit_value& other
) noexcept
{
    std::swap(*this, other);
}

// Swaps the bit value with the value of a bit reference
template <class T>
void bit_value::swap(
    bit_reference<T> other
) noexcept
{
    if (other != _value) {
        flip();
        other.flip();
    }
}
// -------------------------------------------------------------------------- //



// ---------------------- BIT VALUE: BIT MANIPULATION ----------------------- //
// Sets the value of the bit to the provided boolean value
constexpr bit_value& bit_value::set(
    bool b
) noexcept
{
    _value = b;
    return *this;
}

// Sets the value of the bit to 1
constexpr bit_value& bit_value::set(
) noexcept
{
    _value = true;
    return *this;
}

// Resets the value of the bit to 0
constexpr bit_value& bit_value::reset(
) noexcept
{
    _value = false;
    return *this;
}

// Flips the value of the bit
constexpr bit_value& bit_value::flip(
) noexcept
{
    _value = !_value;
    return *this;
}
// -------------------------------------------------------------------------- //



// ---------------------- BIT VALUE: BITWISE OPERATORS ---------------------- //
// Returns the result of a bitwise not on the right hand side
constexpr bit_value operator~( 
    bit_value rhs
) noexcept
{
    using type = unsigned int;
    return bit_value(static_cast<type>(!rhs._value));
}

// Returns the result of a bitwise and between the left and right hand sides
constexpr bit_value operator&(
    bit_value lhs, 
    bit_value rhs
) noexcept
{
    using type = unsigned int;
    return bit_value(static_cast<type>(lhs._value & rhs._value));
}

// Returns the result of a bitwise or between the left and right hand sides
constexpr bit_value operator|(
    bit_value lhs, 
    bit_value rhs
) noexcept
{
    using type = unsigned int;
    return bit_value(static_cast<type>(lhs._value | rhs._value));
}

// Returns the result of a bitwise xor between the left and right hand sides
constexpr bit_value operator^(
    bit_value lhs, 
    bit_value rhs
) noexcept
{
    using type = unsigned int;
    return bit_value(static_cast<type>(lhs._value ^ rhs._value));
}
// -------------------------------------------------------------------------- //



// -------------------- BIT VALUE: COMPARISON OPERATORS --------------------- //
// Checks if the left hand side is equal to the right hand side
constexpr bool operator==(
    bit_value lhs, 
    bit_value rhs
) noexcept
{
    return lhs._value == rhs._value;
}

// Checks if the left hand side is non equal to the right hand side
constexpr bool operator!=(
    bit_value lhs, 
    bit_value rhs
) noexcept
{
    return lhs._value != rhs._value;
}

// Checks if the left hand side is less than the right hand side
constexpr bool operator<(
    bit_value lhs, 
    bit_value rhs
) noexcept
{
    return lhs._value < rhs._value;
}

// Checks if the left hand side is less than or equal to the right hand side
constexpr bool operator<=(
    bit_value lhs, 
    bit_value rhs
) noexcept
{
    return lhs._value <= rhs._value;
}

// Checks if the left hand side is greater than the right hand side
constexpr bool operator>(
    bit_value lhs, 
    bit_value rhs
) noexcept
{
    return lhs._value > rhs._value;
}

// Checks if the left hand side is greater than or equal to the right hand side
constexpr bool operator>=(
    bit_value lhs, 
    bit_value rhs
) noexcept
{
    return lhs._value >= rhs._value;
}
// -------------------------------------------------------------------------- //



// ---------------------- BIT VALUE: STREAM FUNCTIONS ----------------------- //
// Extracts a bit value from an input stream
template <class CharT, class Traits>
std::basic_istream<CharT, Traits>& operator>>(
    std::basic_istream<CharT, Traits>& is,
    bit_value& x
)
{
    using stream_type = std::basic_istream<CharT, Traits>;
    using traits_type = typename stream_type::traits_type;
    using ios_base = typename stream_type::ios_base;
    constexpr char zero = '0';
    constexpr char one = '1';
    constexpr typename stream_type::int_type eof = traits_type::eof();
    typename ios_base::iostate state = ios_base::goodbit;
    typename stream_type::char_type char_value = 0;
    typename stream_type::int_type int_value = 0;
    typename stream_type::sentry sentry(is);
    bool ok = false;
    bit_value tmp = x;
    if (sentry) {
        try {
            int_value = is.rdbuf()->sbumpc();
            if (traits_type::eq_int_type(int_value, eof)) {
                state |= ios_base::eofbit;
            } else {
                char_value = traits_type::to_char_type(int_value);
                if (traits_type::eq(char_value, is.widen(zero))) {
                    tmp.reset();
                    ok = true;
                } else if (traits_type::eq(char_value, is.widen(one))) {
                    tmp.set();
                    ok = true;
                } else {
                    int_value = is.rdbuf()->sputbackc(char_value);
                    if (traits_type::eq_int_type(int_value, eof)) {
                        state |= ios_base::failbit;
                    }
                }
            }
        } catch(...) {
            is.setstate(ios_base::badbit);
        }
    }
    if (ok) {
        x = tmp;
    } else {
        state |= ios_base::failbit;
    }
    state ? is.setstate(state) : void();
    return is;
}

// Inserts a bit value in an output stream
template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(
    std::basic_ostream<CharT, Traits>& os,
    bit_value x
)
{
    constexpr char zero = '0';
    constexpr char one = '1';
    return os << os.widen(x ? one : zero);
}
// -------------------------------------------------------------------------- //



// -------------------------- BIT VALUE: CONSTANTS -------------------------- //
// Constant bit values
constexpr bit_value bit_off(0U);
constexpr bit_value bit_on(1U);
constexpr bit_value bit0(0U);
constexpr bit_value bit1(1U);
// -------------------------------------------------------------------------- //



// ========================================================================== //
} // namespace bit
#endif // _BIT_VALUE_HPP_INCLUDED
// ========================================================================== //
