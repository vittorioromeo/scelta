// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "../traits.hpp"
#include "./returns.hpp"
#include <type_traits>

// Usage of C++17: nested `namespace`.
namespace scelta::impl
{
    struct non_recursive_tag
    {
    };

    template <typename Return>
    struct recursive_tag
    {
        using return_type = Return;
    };

    // clang-format off
    template <typename Tag, typename Visitor, typename Variant, typename... Variants>
    constexpr auto visit_homogenizer(Tag tag,
        Visitor&& visitor, Variant&& variant, Variants&&... variants)
        SCELTA_RETURNS(
            ::scelta::traits::adt::visit_v<std::decay_t<Variant>>(tag,
                FWD(visitor), FWD(variant), FWD(variants)...)
        )
    // clang-format on
}
