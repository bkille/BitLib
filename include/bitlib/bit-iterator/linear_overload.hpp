// ============================ LINEAR OVERLOAD ============================= //
// Project:         The C++ Bit Library
// Name:            linear_overload.hpp
// Description:     Utilities to invoke the first valid call of an overload set
// Creator:         Vincent Reverdy
// Contributor(s):  Vincent Reverdy [2015-2017]
// License:         BSD 3-Clause License
// ========================================================================== //
#ifndef _LINEAR_OVERLOAD_HPP_INCLUDED
#define _LINEAR_OVERLOAD_HPP_INCLUDED
// ========================================================================== //



// ================================ PREAMBLE ================================ //
// C++ standard library
#include <tuple>
#include <utility>
#include <type_traits>
// Project sources
// Third-party libraries
// Miscellaneous
namespace bit {
// ========================================================================== //



/* **************************** LINEAR OVERLOAD ***************************** */
// Linear overload class definition
template <class... F>
class linear_overload
{
    // Types
    public:
    using tuple = std::tuple<F...>;

    // Lifecycle
    public:
    template <class... G>
    explicit constexpr linear_overload(G&&... g);

    // Access
    public:
    template <std::size_t I>
    decltype(auto) get() noexcept;
    template <std::size_t I>
    constexpr decltype(auto) get() const noexcept;
    template <class G>
    decltype(auto) get() noexcept;
    template <class G>
    constexpr decltype(auto) get() const noexcept;

    // Capacity
    public:
    static constexpr bool empty() noexcept;
    static constexpr std::size_t size() noexcept;
    static constexpr std::size_t max_size() noexcept;

    // Call
    public:
    template <
        std::size_t N = 0,
        class... Args,
        class = typename std::enable_if<N >= size()>::type
    >
    void operator()(Args&&...);
    template <
        std::size_t N = 0,
        class = typename std::enable_if<N < size()>::type,
        class = decltype(std::get<N>(std::declval<tuple>())())
    >
    decltype(auto) operator()();
    template <
        std::size_t N = 0,
        class Arg,
        class... Args,
        class = typename std::enable_if<N < size()>::type,
        class = decltype(std::get<N>(std::declval<tuple>())(
            std::declval<Arg>(),
            std::declval<Args>()...
        ))
    >
    decltype(auto) operator()(Arg&& arg, Args&&... args);
    template <
        std::size_t N = 0,
        class... Args,
        class = typename std::enable_if<N < size()>::type
    >
    decltype(auto) operator()(Args&&... args);

    // Implementation details: data members
    private:
    tuple _f;

    // Maker
    public:
    template <class... G>
    friend constexpr linear_overload<G...> overload_linearly(G&&... g);
};
/* ************************************************************************** */



// ----------------------- LINEAR OVERLOAD: LIFECYCLE ----------------------- //
// Explicitly constructs a linear overload from a list of functions
template <class... F>
template <class... G>
constexpr linear_overload<F...>::linear_overload(
    G&&... g
)
: _f{std::forward<G>(g)...}
{
}
// -------------------------------------------------------------------------- //



// ------------------------ LINEAR OVERLOAD: ACCESS ------------------------- //
// Gets the i-th function of the linear overload
template <class... F>
template <std::size_t I>
decltype(auto) linear_overload<F...>::get(
) noexcept
{
    return std::get<I>(_f);
}

// Gets the i-th function of the immutable linear overload
template <class... F>
template <std::size_t I>
constexpr decltype(auto) linear_overload<F...>::get(
) const noexcept
{
    return std::get<I>(_f);
}

// Gets the function of the given type from the linear overload
template <class... F>
template <class G>
decltype(auto) linear_overload<F...>::get() noexcept
{
    return std::get<G>(_f);
}

// Gets the function of the given type from the immutable linear overload
template <class... F>
template <class G>
constexpr decltype(auto) linear_overload<F...>::get(
) const noexcept
{
    return std::get<G>(_f);
}
// -------------------------------------------------------------------------- //



// ----------------------- LINEAR OVERLOAD: CAPACITY ------------------------ //
// Checks whether the linear overload is empty
template <class... F>
constexpr bool linear_overload<F...>::empty(
) noexcept
{
    return std::tuple_size<tuple>::value == 0;
}

// Returns the number of functions in the linear overload
template <class... F>
constexpr std::size_t linear_overload<F...>::size(
) noexcept
{
    return std::tuple_size<tuple>::value;
}

// Returns the maximum possible number of functions in the linear overload
template <class... F>
constexpr std::size_t linear_overload<F...>::max_size(
) noexcept
{
    return std::tuple_size<tuple>::value;
}
// -------------------------------------------------------------------------- //



// ------------------------- LINEAR OVERLOAD: CALL -------------------------- //
// Calls the linear overload with the provided arguments: no valid overload
template <class... F>
template <std::size_t N, class... Args, class>
void linear_overload<F...>::operator()(
    Args&&...
)
{
}

// Calls the linear overload with the provided arguments: no argument
template <class... F>
template <std::size_t N, class, class>
decltype(auto) linear_overload<F...>::operator()(
)
{
    return std::get<N>(_f)();
}

// Calls the linear overload with the provided arguments: valid call
template <class... F>
template <std::size_t N, class Arg, class... Args, class, class>
decltype(auto) linear_overload<F...>::operator()(
    Arg&& arg,
    Args&&... args
)
{
    return std::get<N>(_f)(std::forward<Arg>(arg), std::forward<Args>(args)...);
}

// Calls the linear overload with the provided arguments: invalid call
template <class... F>
template <std::size_t N, class... Args, class>
decltype(auto) linear_overload<F...>::operator()(
    Args&&... args
)
{
    return operator()<N + 1>(std::forward<Args>(args)...);
}
// -------------------------------------------------------------------------- //



// ------------------------- LINEAR OVERLOAD: MAKER ------------------------- //
// Builds a linear overload from a list of functions
template <class... G>
constexpr linear_overload<G...> overload_linearly(G&&... g)
{
    return linear_overload<G...>(std::forward<G>(g)...);
}
// -------------------------------------------------------------------------- //



// ========================================================================== //
} // namespace bit
#endif // _LINEAR_OVERLOAD_HPP_INCLUDED
// ========================================================================== //
