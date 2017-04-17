// Copyright (c) 2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "../meta/forward_like.hpp"
#include "./returns.hpp"

namespace scelta
{
    struct nullopt_t
    {
    };
}

namespace scelta::impl
{
    template <typename... Ts>
    constexpr scelta::nullopt_t to_nullopt(Ts&&...) noexcept
    {
        return {};
    }

    // clang-format off
    template <typename Visitor>
    constexpr auto visit_optional(Visitor&& visitor)
        SCELTA_RETURNS(
            FWD(visitor)()
        )

    template <typename F, typename Continuation, typename Optional>
    constexpr auto call_with_optional(F&& f, Continuation&& c, Optional&& o)
        SCELTA_RETURNS(
            o ? FWD(c)(FWD(f)(meta::forward_like<Optional>(*o)))
              : FWD(c)(FWD(f)(to_nullopt()))
        )

    template <typename Visitor, typename Optional, typename... Optionals>
    constexpr auto visit_optional(
        Visitor&& visitor, Optional&& o, Optionals&&... os)
        SCELTA_NOEXCEPT_AND_TRT(FWD(visitor)(to_nullopt(), to_nullopt(os)...))
    {
        return call_with_optional(
            [&](auto&& x) noexcept
            {
                return [&](auto&&... xs) SCELTA_RETURNS(
                    FWD(visitor)(FWD(x), FWD(xs)...)
                );
            }, 
            [&](auto&& bound_visitor) SCELTA_RETURNS(
                visit_optional(FWD(bound_visitor), FWD(os)...)
            ), 
            FWD(o)
        );
    }
}
