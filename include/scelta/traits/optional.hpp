// Copyright (c) 2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
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
