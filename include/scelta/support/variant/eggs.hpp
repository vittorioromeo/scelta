// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

// clang-format off
// Usage of C++17: `__has_include`.
#if __has_include(<eggs/variant.hpp>)
// clang-format on

#include "../../utils/homogenizer.hpp"
#include "../../traits.hpp"
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
