// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <type_traits>

namespace scelta::recursive
{
    namespace impl
    {
        template <typename T, typename = void>
        struct original_type
        {
            using type = T;
        };

        template <typename T>
        struct original_type<T, std::void_t<typename T::base_type>>
        {
            using type = typename T::base_type;
        };

        // Evaluates to `T` if `T::base_type` does not exist, otherwise
        // evaluates to `T::base_type`. Used to retrieve the "original"
        // visitable type from a recursive wrapper.
        template <typename T>
        using original_type_t = typename original_type<T>::type;
    }
}
