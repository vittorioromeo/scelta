// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "../traits.hpp"
#include "./returns.hpp"

// Usage of C++17: nested `namespace`.
namespace scelta::impl
{
    // clang-format off
    template <typename Optional>
    constexpr auto access_optional(Optional&& o)
        SCELTA_RETURNS(
            ::scelta::traits::optional::access_v<std::decay_t<Optional>>(FWD(o))
        )
    // clang-format on
}
