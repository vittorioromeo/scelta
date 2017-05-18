// Copyright (c) 2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

// clang-format off
// Usage of C++17: `__has_include`.
#if __has_include(<eggs/variant.hpp>)
// clang-format on

#include "../../utils/homogenizer.hpp"
#include "../../traits/variant.hpp"
#include <eggs/variant.hpp>

#define SCELTA_SUPPORT_VARIANT_EGGS 1

namespace scelta::impl
{
    template <typename... Alternatives>
    struct traits<::eggs::variant<Alternatives...>>
    {
        template <typename Tag, typename... Ts>
        static constexpr auto visit(Tag, Ts&&... xs)
            SCELTA_RETURNS(
                ::eggs::variants::apply(FWD(xs)...)
            )

        template <typename... Variants>
        static constexpr auto valid_state(Variants&&...)
            SCELTA_RETURNS(
                true
            )
    };
}

#endif
