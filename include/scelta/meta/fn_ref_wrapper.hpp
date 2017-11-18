// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "../utils/fwd.hpp"
#include "../utils/returns.hpp"
#include <type_traits>

namespace scelta::meta
{
    /// @brief `constexpr`-friendly function object reference wrapper.
    template <typename F>
    struct fn_ref_wrapper
    {
        static_assert(std::is_reference_v<F>);

        F _f;

        constexpr fn_ref_wrapper(F f) noexcept : _f{FWD(f)}
        {
        }

        // clang-format off
        template <typename... Ts>
        constexpr auto operator()(Ts&&... xs) const
            SCELTA_RETURNS(
                FWD(_f)(FWD(xs)...)
            )
        // clang-format on
    };
}
