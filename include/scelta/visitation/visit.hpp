// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "../utils.hpp"

namespace scelta
{
    // clang-format off
    template <typename Visitor>
    constexpr auto visit(Visitor&& visitor)
        SCELTA_RETURNS(FWD(visitor)())

    namespace impl
    {
        template <typename... Ts>
        constexpr auto visit_impl(Ts&&... xs)
            SCELTA_RETURNS(
                impl::visit_homogenizer(FWD(xs)...)
            )
    }
    // clang-format on

    template <typename... Ts>
    constexpr auto visit(Ts&&... xs)
        SCELTA_RETURNS(impl::visit_impl(impl::non_recursive_tag{}, FWD(xs)...))
}
