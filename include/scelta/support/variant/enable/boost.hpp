// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "../../../traits/adt/visit.hpp"
#include "../../../utils/returns.hpp"

#define SCELTA_SUPPORT_VARIANT_BOOST 1

namespace scelta::traits::adt
{
    template <typename... Alts>
    struct visit<::boost::variant<Alts...>>
    {
        // clang-format off
        template <typename Tag, typename... Ts>
        constexpr auto operator()(Tag, Ts&&... xs)
            SCELTA_RETURNS(
                ::boost::apply_visitor(FWD(xs)...)
            )
        // clang-format on
    };
}

