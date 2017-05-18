// Copyright (c) 2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./returns.hpp"
#include <type_traits>

// Usage of C++17: nested `namespace`.
namespace scelta::impl
{
    template <typename>
    struct traits;

    template <typename>
    struct optional_traits
    {
        template <typename Optional>
        static constexpr auto access(Optional&& o)
            SCELTA_RETURNS(
                *FWD(o)
            )
    };

    template <typename Optional>
    constexpr auto access_optional(Optional&& o) SCELTA_RETURNS(
        optional_traits<std::decay_t<Optional>>::access(FWD(o))
    )

    // clang-format off
    template <typename T>
    struct visitor_adapter
    {
        template <typename Tag, typename U>
        constexpr auto operator()(Tag, U&& x) const
            SCELTA_RETURNS(
                FWD(x)
            )
    };

    struct non_recursive_tag {};

    template <typename Return>
    struct recursive_tag { using return_type = Return; };

    template <typename Tag, typename Visitor, typename Variant, typename... Variants>
    constexpr auto visit_homogenizer(Tag tag,
        Visitor&& visitor, Variant&& variant, Variants&&... variants)
        SCELTA_RETURNS(
            traits<std::decay_t<Variant>>::visit(tag, FWD(visitor),
                FWD(variant), FWD(variants)...)
        )
    // clang-format on
}
