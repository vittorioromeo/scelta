// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once
#ifndef SCELTA_SUPPORT_VARIANT_EGGS_DISABLE

// clang-format off
#if __has_include(<eggs/variant.hpp>)
// clang-format on

#include "../../traits/adt/visit.hpp"
#include "../../utils/returns.hpp"
#include <eggs/variant.hpp>

#define SCELTA_SUPPORT_VARIANT_EGGS 1

namespace scelta::traits::adt
{
    template <typename... Alts>
    struct visit<::eggs::variant<Alts...>>
    {
        // clang-format off
        template <typename Tag, typename... Ts>
        constexpr auto operator()(Tag, Ts&&... xs)
            SCELTA_RETURNS(
                ::eggs::variants::apply(FWD(xs)...)
            )
        // clang-format on
    };
}

#endif
#endif
