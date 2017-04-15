// Copyright (c) 2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./returns.hpp"
#include <type_traits>

namespace scelta
{
    template <typename... Ts>
    struct overloader : Ts...
    {
        // Usage of C++17: fold expression.
        template <typename... Args>
        constexpr overloader(Args&&... xs) noexcept(
            (noexcept(Ts{FWD(xs)}) && ...))
            : Ts{FWD(xs)}...
        {
        }

        // Usage of C++17: variadic `using`-directive.
        using Ts::operator()...;
    };

    template <typename... Ts>
    constexpr auto overload(Ts&&... xs)
        SCELTA_RETURNS(overloader<std::remove_reference_t<Ts>...>{FWD(xs)...})
}
