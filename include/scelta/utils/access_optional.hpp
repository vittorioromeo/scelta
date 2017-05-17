// Copyright (c) 2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./returns.hpp"
#include <type_traits>

// Usage of C++17: nested `namespace`.
namespace scelta::impl
{
    template <typename T>
    struct optional_access_homogenizer
    {
        template <typename U>
        constexpr auto operator()(U&& x) SCELTA_RETURNS(*FWD(x))
    };

    template <typename T>
    constexpr auto access_optional(T&& x) SCELTA_RETURNS(
        optional_access_homogenizer<std::decay_t<T>>{}(FWD(x))
    )
}
