// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "../../utils/returns.hpp"
#include "../dispatch.hpp"
#include "../../meta/type_wrapper.hpp"

namespace scelta::traits::optional
{
    template <typename>
    struct make_unset;

    template <template <typename...> typename Optional, typename T,
        typename... Ts>
    struct make_unset<Optional<T, Ts...>>
    {
        // clang-format off
        template <typename X>
        constexpr auto operator()(X)
            SCELTA_RETURNS(
                Optional<scelta::meta::unwrap<X>>{}
            )
        // clang-format on
    };

    template <typename T>
    inline constexpr traits::impl::dispatch<make_unset<T>> make_unset_v{};
}
