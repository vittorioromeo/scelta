// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "../dispatch.hpp"

// Usage of C++17: nested `namespace`.
namespace scelta::traits::adt
{
    template <typename>
    struct valid
    {
        template <typename... Variants>
        constexpr auto operator()(Variants&&...)
        {
            return true;
        }
    };

    template <typename T>
    inline constexpr traits::impl::dispatch<valid<T>> valid_v{};
}
