// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "../meta/type_wrapper.hpp"
#include "../traits/optional.hpp"
#include "./nothing.hpp"
#include "./returns.hpp"
#include <type_traits>

namespace scelta
{
    // clang-format off
    /// @brief Always returns `nothing`. Used to generalize optional operations
    /// that return `void`.
    template <typename Optional>
    constexpr auto make_like(const detail::nothing&)
        SCELTA_RETURNS(
            detail::nothing{}
        )

    /// @brief Creates a new `Optional` containing `xs...`. Deduces the type
    /// of the stored item from `xs...`.
    template <typename Optional, typename... Ts>
    constexpr auto make_like(Ts&&... xs)
        SCELTA_RETURNS(
            ::scelta::traits::optional::make_v<std::decay_t<Optional>>(
                FWD(xs)...
            )
        )

    /// @brief Creates an unset `Optional` whose stored type is `T`.
    template <typename Optional, typename T>
    constexpr auto make_unset_like()
        SCELTA_RETURNS_ENABLE_IF(
            (!std::is_same_v<T, detail::nothing>),
            ::scelta::traits::optional::make_unset_v<std::decay_t<Optional>>(
                ::scelta::meta::t<T>
            )
        )

    /// @brief Always returns `nothing`. Used to generalize optional operations
    /// that return `void`.
    template <typename Optional, typename T>
    constexpr auto make_unset_like()
        SCELTA_RETURNS_ENABLE_IF(
            (std::is_same_v<T, detail::nothing>),
            detail::nothing{}
        )
    // clang-format on
}
