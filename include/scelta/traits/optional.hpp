// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "../utils.hpp"
#include "./dispatch.hpp"

// Usage of C++17: nested `namespace`.
namespace scelta::traits::optional
{
    template <typename>
    struct access
    {
        // clang-format off
        template <typename Optional>
        constexpr auto operator()(Optional&& o)
            SCELTA_RETURNS(
                *FWD(o)
            )
        // clang-format on
    };

    template <typename T>
    inline constexpr traits::impl::dispatch<access<T>> access_v{};
}
