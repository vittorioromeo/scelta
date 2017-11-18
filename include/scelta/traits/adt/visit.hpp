// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "../../meta/always_false.hpp"
#include "../../utils/optional_utils.hpp"
#include "../dispatch.hpp"

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

    // TODO:
    /*template <typename T>
    struct visit
    {
        static_assert(meta::always_false_v<T>,
            "`scelta::traits::adt::visit` not specialized for `T`.");
    };*/

    template <typename>
    struct visit;

    template <typename T>
    inline constexpr traits::impl::dispatch<visit<T>> visit_v{};
}
