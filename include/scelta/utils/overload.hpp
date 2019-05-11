// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./returns.hpp"
#include <type_traits>

namespace scelta
{
    namespace impl
    {
        template <typename... Ts>
        struct overloader : Ts...
        {
            template <typename... Args>
            constexpr overloader(Args&&... xs) noexcept(
                (noexcept(Ts{FWD(xs)}) && ...))
                : Ts{FWD(xs)}...
            {
            }

            using Ts::operator()...;
        };
    }

    template <typename... Ts>
    [[nodiscard]] constexpr auto overload(Ts&&... xs) SCELTA_RETURNS(
        impl::overloader<std::remove_reference_t<Ts>...>{FWD(xs)...})
}
