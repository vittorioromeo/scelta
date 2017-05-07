// Copyright (c) 2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "../utils/fwd.hpp"
#include "../utils/returns.hpp"
#include <type_traits>

// Usage of C++17: nested `namespace`.
namespace scelta::meta
{
    // Applies `T`'s value category on `Source`.
    template <typename Source, typename T>
    using as_if_forwarded = std::conditional_t< //
        std::is_lvalue_reference_v<Source>,     //
        std::remove_reference_t<T>&,            //
        std::remove_reference_t<T>&&>;

    // Forwards `x` with the same value category as `Source`.
    template <typename Source, typename T>
    auto forward_like(T&& x)
        SCELTA_RETURNS(static_cast<as_if_forwarded<Source, T&&>>(x))
}