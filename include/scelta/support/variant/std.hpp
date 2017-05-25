// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once
#ifndef SCELTA_SUPPORT_VARIANT_STD_DISABLE

// clang-format off
// Usage of C++17: `__has_include`.
#if __has_include(<variant>)
// clang-format on

// Usage of C++17: `<variant>`.
#include "../../traits/adt/visit.hpp"
#include "../../traits/adt/valid.hpp"
#include "../../utils/returns.hpp"
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
#endif
