// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "../utils/returns.hpp"
#include <type_traits>

namespace scelta::meta
{
    /// @brief Applies `T`'s value category on `Source`.
    template <typename Source, typename T>
    using as_if_forwarded = std::conditional_t< //
        std::is_lvalue_reference_v<Source>,     //
        std::remove_reference_t<T>&,            //
        std::remove_reference_t<T>&&>;

    /// @brief Forwards `x` with the same value category as `Source`.
    template <typename Source, typename T>
    [[nodiscard]] auto forward_like(T&& x)
        SCELTA_RETURNS(static_cast<as_if_forwarded<Source, T&&>>(x))
}
