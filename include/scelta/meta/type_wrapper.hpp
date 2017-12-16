// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

namespace scelta::meta
{
    template <typename T>
    struct type_wrapper
    {
        using type = T;
    };

    template <typename T>
    inline constexpr type_wrapper<T> t{};

    template <typename T>
    using unwrap = typename T::type;
}
