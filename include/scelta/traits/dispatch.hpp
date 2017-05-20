// Copyright (c) 2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "../utils/fwd.hpp"
#include "../utils/returns.hpp"
#include <utility>

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
