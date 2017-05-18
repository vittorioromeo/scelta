// Copyright (c) 2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

// clang-format off
// Usage of C++17: `__has_include`.
#if __has_include(<type_safe/optional.hpp>)
// clang-format on

#include "../../utils/optional_utils.hpp"
#include "../../utils/homogenizer.hpp"
#include "../../traits/optional.hpp"
#include <type_safe/optional.hpp>

#define SCELTA_SUPPORT_OPTIONAL_TYPE_SAFE 1

namespace scelta::impl
{
    template <typename T>
    struct traits<::type_safe::optional<T>>
    {
        template <typename Tag, typename... Ts>
        static constexpr auto visit(Tag, Ts&&... xs)
            SCELTA_RETURNS(
                ::scelta::impl::visit_optional(FWD(xs)...)
            )

        template <typename... Variants>
        static constexpr auto valid_state(Variants&&...)
            SCELTA_RETURNS(
                true
            )
    };

    template <typename T>
    struct optional_traits<::type_safe::optional<T>>
    {
        template <typename Optional>
        static constexpr auto access(Optional&& o)
            SCELTA_RETURNS(
                FWD(o).value()
            )
    };
}

#endif
