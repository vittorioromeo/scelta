// Copyright (c) 2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <type_traits>

// Usage of C++17: nested `namespace`.
namespace scelta::meta
{
    template <typename Before, typename After, typename T>
    struct replace_all : std::conditional<std::is_same_v<T, Before>, After, T>
    {
    };

    template <typename Before, typename After, typename T>
    using replace_all_t = typename replace_all<Before, After, T>::type;

    // Usage of C++17: `template <...> typename`.
    template <typename Before, typename After,
        template <typename...> typename T, typename... Ts>
    struct replace_all<Before, After, T<Ts...>>
    {
        using type = T<replace_all_t<Before, After, Ts>...>;
    };
}