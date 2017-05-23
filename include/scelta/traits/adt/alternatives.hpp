// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "../dispatch.hpp"
#include <tuple>

// Usage of C++17: nested `namespace`.
namespace scelta::traits::adt
{
    namespace impl
    {
        template <typename... Ts>
        struct alternative_tuple
        {
            using type = std::tuple<Ts...>;
        };
    }

    template <typename>
    struct alternatives;

    template <template <typename...> class T, typename... Ts>
    struct alternatives<T<Ts...>> : impl::alternative_tuple<Ts...>
    {
    };

    template <typename T>
    inline constexpr traits::impl::dispatch<alternatives<T>> alternatives_v{};

    template <typename T, std::size_t N>
    using nth_alternative =
        std::tuple_element_t<N, typename alternatives<T>::type>;

    template <typename T>
    using first_alternative = nth_alternative<T, 0>;
}
