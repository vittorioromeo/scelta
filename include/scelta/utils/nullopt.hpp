// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "../traits/adt/is_visitable.hpp"

namespace scelta
{
    struct nullopt_t
    {
        // TODO: limit to optional
        template <typename T, typename = std::enable_if_t<
                                  traits::adt::is_visitable_v<std::decay_t<T>>>>
        operator T() const
        {
            return {};
        }
    };

    inline constexpr nullopt_t nullopt{};

    // clang-format off
    /// @brief Always returns `nullopt`.
    template <typename... Ts>
    constexpr auto to_nullopt(Ts&&...)
        SCELTA_RETURNS(
            ::scelta::nullopt
        )

    /// @brief Returns `true` if `o` is an unset optional.
    template <typename Optional>
    [[nodiscard]] constexpr bool is_nullopt(const Optional& o) noexcept
    {
        return !static_cast<bool>(o);
    }

    /// @brief Returns `true` if `o` is an unset optional.
    [[nodiscard]] constexpr bool is_nullopt(const nullopt_t&) noexcept
    {
        return true;
    }
    // clang-format on
}
