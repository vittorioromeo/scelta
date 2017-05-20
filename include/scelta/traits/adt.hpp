// Copyright (c) 2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "../meta/always_false.hpp"
#include "../utils/optional_utils.hpp"

// Usage of C++17: nested `namespace`.
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

    template <typename T>
    struct visit
    {
        static_assert(meta::always_false_v<T>,
            "`scelta::traits::adt::visit` not specialized for `T`.");
    };

    template <typename>
    struct valid
    {
        template <typename... Variants>
        constexpr auto operator()(Variants&&...)
        {
            return true;
        }
    };

// TODO: constexpr
    template <typename T>
    inline visit<T> visit_v{};

// TODO: constexpr
    template <typename T>
    inline valid<T> valid_v{};
}
