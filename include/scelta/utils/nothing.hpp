// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./returns.hpp"
#include <type_traits>

namespace scelta::detail
{
    /// @brief Empty type used to replace `void`. Used to avoid specializing
    /// higher-level functions on `void`.
    struct nothing
    {
    };

    // clang-format off
    /// @brief Invokes `f(xs...)` and returns `nothing`.
    template <typename F, typename... Ts>
    constexpr auto invoke_void_to_nothing(F&& f, Ts&&... xs)
        SCELTA_RETURNS_ENABLE_IF(
            (std::is_void_v<decltype(FWD(f)(FWD(xs)...))>),
            FWD(f)(FWD(xs)...), nothing{}
        )

    /// @brief Invokes and returns `f(xs...)`.
    template <typename F, typename... Ts>
    constexpr auto invoke_void_to_nothing(F&& f, Ts&&... xs)
        SCELTA_RETURNS_ENABLE_IF(
            (!std::is_void_v<decltype(FWD(f)(FWD(xs)...))>),
            FWD(f)(FWD(xs)...)
        )

    // clang-format on
}
