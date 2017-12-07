// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

// This header assumes that `eggs::variant` was already properly included
// beforehand.

#include "../../../traits/adt/visit.hpp"
#include "../../../utils/returns.hpp"

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
