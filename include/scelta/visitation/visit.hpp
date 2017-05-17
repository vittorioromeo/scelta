// Copyright (c) 2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
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
        template <typename Tag, typename Visitor, typename Variant, typename... Variants>
        constexpr auto visit_impl(Tag tag,
            Visitor&& visitor, Variant&& variant, Variants&&... variants)
            SCELTA_RETURNS(
                impl::visit_homogenizer(tag,
                    FWD(visitor), FWD(variant), FWD(variants)...)
            )
    }
    // clang-format on

    template <typename... Ts>
    constexpr auto visit(Ts&&... xs)
        SCELTA_RETURNS(impl::visit_impl(impl::non_recursive_tag{}, FWD(xs)...))
}
