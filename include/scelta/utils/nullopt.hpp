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
        template <typename T>//,
            //typename = std::enable_if_t<>>>
        operator T() const
        {
           // static_assert(traits::adt::is_visitable_v<std::decay_t<T>>);
            return {};
        }
    };

    inline constexpr nullopt_t nullopt{};

    // clang-format off
    // TODO: test, docs
    template <typename... Ts>
    constexpr auto to_nullopt(Ts&&...)
        SCELTA_RETURNS(
            ::scelta::nullopt
        )
    // clang-format on

    template <typename Optional>
    constexpr bool is_nullopt(const Optional& o) noexcept
    {
        return !static_cast<bool>(o);
    }
}
