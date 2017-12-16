// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "../meta/type_wrapper.hpp"
#include "../traits/optional.hpp"
#include "./returns.hpp"
#include <type_traits>

namespace scelta
{
    // clang-format off
    template <typename Optional, typename... Ts>
    constexpr auto make_like(Ts&&... xs)
        SCELTA_RETURNS(
            ::scelta::traits::optional::make_v<std::decay_t<Optional>>(
                FWD(xs)...
            )
        )

    template <typename Optional, typename T>
    constexpr auto make_unset_like()
        SCELTA_RETURNS(
            ::scelta::traits::optional::make_unset_v<std::decay_t<Optional>>(
                ::scelta::meta::t<T>
            )
        )
    // clang-format on
}
