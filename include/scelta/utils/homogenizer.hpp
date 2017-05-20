// Copyright (c) 2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./returns.hpp"
#include "../traits.hpp"
#include <type_traits>

// Usage of C++17: nested `namespace`.
namespace scelta::impl
{
    struct non_recursive_tag {};

    template <typename Return>
    struct recursive_tag { using return_type = Return; };

    template <typename Tag, typename Visitor, typename Variant, typename... Variants>
    constexpr auto visit_homogenizer(Tag tag,
        Visitor&& visitor, Variant&& variant, Variants&&... variants)
        SCELTA_RETURNS(
            ::scelta::traits::adt::visit_v<std::decay_t<Variant>>(tag, FWD(visitor),
                FWD(variant), FWD(variants)...)
        )
    // clang-format on
}
