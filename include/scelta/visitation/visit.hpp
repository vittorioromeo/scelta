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

    template <typename Visitor, typename Variant, typename... Variants>
    constexpr auto visit(
        Visitor&& visitor, Variant&& variant, Variants&&... variants)
        SCELTA_RETURNS(
            impl::homogenizer_helper_t<std::decay_t<Variant>>{}(
                FWD(visitor), FWD(variant), FWD(variants)...)
        )
    // clang-format on
}