// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "../../utils/optional_utils.hpp"

namespace scelta::traits::adt
{
    namespace impl
    {
        struct visit_optional_t
        {
            // clang-format off
            template <typename Tag, typename... Ts>
            constexpr auto operator()(Tag, Ts&&... xs)
                SCELTA_RETURNS(
                    ::scelta::impl::visit_optional(FWD(xs)...)
                )
            // clang-format on
        };
    }
}
