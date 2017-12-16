// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "../../utils/returns.hpp"
#include "../dispatch.hpp"

namespace scelta::traits::optional
{
    template <typename>
    struct make;

    template <template <typename...> typename Optional, typename T,
        typename... Ts>
    struct make<Optional<T, Ts...>>
    {
        // clang-format off
        template <typename... Xs>
        constexpr auto operator()(Xs&&... xs)
            SCELTA_RETURNS(
                Optional<std::decay_t<Xs>...>{FWD(xs)...}
            )
        // clang-format on
    };

    template <typename T>
    inline constexpr traits::impl::dispatch<make<T>> make_v{};
}
