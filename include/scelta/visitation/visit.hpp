// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "../traits/adt/valid.hpp"
#include "../utils/returns.hpp"
#include "../utils/assert.hpp"
#include "../utils/homogenizer.hpp"

namespace scelta
{
    // clang-format off
    template <typename Visitor>
    constexpr auto visit(Visitor&& visitor)
        SCELTA_RETURNS(FWD(visitor)())

    namespace impl
    {
        constexpr void visit_impl() noexcept { return; }

        template <typename Tag, typename Visitor, typename Variant, typename... Variants>
        constexpr auto visit_impl(Tag tag,
            Visitor&& visitor, Variant&& v, Variants&&... vs)
            SCELTA_NOEXCEPT_AND_TRT(
                impl::visit_homogenizer(tag, FWD(visitor), FWD(v), FWD(vs)...)
            )
        {
            return
                SCELTA_CONSTEXPR_ASSERT(
                    ::scelta::traits::adt::valid_v<std::decay_t<Variant>>(v, vs...)
                ),
                impl::visit_homogenizer(tag, FWD(visitor), FWD(v), FWD(vs)...);
        }
    }
    // clang-format on

    template <typename... Ts>
    constexpr auto visit(Ts&&... xs)
        SCELTA_RETURNS(impl::visit_impl(impl::non_recursive_tag{}, FWD(xs)...))
}
