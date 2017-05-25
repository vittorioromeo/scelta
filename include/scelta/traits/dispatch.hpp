// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "../utils/fwd.hpp"
#include "../utils/returns.hpp"

// Usage of C++17: nested `namespace`.
namespace scelta::traits::impl
{
    // Without `dispatch` every user-provided trait specialization would require
    // `const` in `operator()`.
    template <typename Trait>
    struct dispatch
    {
        // clang-format off
        template <typename... Ts>
        constexpr auto operator()(Ts&&... xs) const
            SCELTA_RETURNS(
                Trait{}(FWD(xs)...)
            )
        // clang-format on
    };
}
