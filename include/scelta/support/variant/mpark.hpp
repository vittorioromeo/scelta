// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once
#ifndef SCELTA_SUPPORT_VARIANT_MPARK_DISABLE

// clang-format off
// Usage of C++17: `__has_include`.
#if __has_include(<mpark/variant.hpp>)
// clang-format on

#include "../../traits/adt/visit.hpp"
#include "../../traits/adt/valid.hpp"
#include "../../utils/returns.hpp"
#include <mpark/variant.hpp>

#define SCELTA_SUPPORT_VARIANT_MPARK 1

namespace scelta::traits::adt
{
    template <typename... Alts>
    struct visit<::mpark::variant<Alts...>>
    {
        // clang-format off
        template <typename Tag, typename... Ts>
        constexpr auto operator()(Tag, Ts&&... xs)
            SCELTA_RETURNS(
                ::mpark::visit(FWD(xs)...)
            )
        // clang-format on
    };

    template <typename... Alts>
    struct valid<::mpark::variant<Alts...>>
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
