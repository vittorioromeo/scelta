// Copyright (c) 2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

// clang-format off
// Usage of C++17: `__has_include`.
#if __has_include(<variant>)
// clang-format on

// Usage of C++17: `<variant>`.
#include "../../utils/homogenizer.hpp"
#include "../../traits.hpp"
#include <variant>

#define SCELTA_SUPPORT_VARIANT_STD 1

namespace scelta::traits::adt
{
    template <typename... Alts>
    struct visit<::std::variant<Alts...>>
    {
        // clang-format off
        template <typename Tag, typename... Ts>
        constexpr auto operator()(Tag, Ts&&... xs)
            SCELTA_RETURNS(
                ::std::visit(FWD(xs)...)
            )
        // clang-format on
    };

    template <typename... Alts>
    struct valid<::std::variant<Alts...>>
    {
        // clang-format off
        template <typename... Ts>
        constexpr auto operator()(Ts&&... xs)
            SCELTA_RETURNS(
                (!xs.valueless_by_exception() && ...)
            )
        // clang-format on
    };
}

#endif
