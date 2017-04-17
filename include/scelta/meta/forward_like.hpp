// Copyright (c) 2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "../utils/fwd.hpp"
#include <type_traits>

// Usage of C++17: nested `namespace`.
namespace scelta::meta
{
    template <typename Source, typename T>
    decltype(auto) forward_like(T&& x) noexcept
    {
        using type = std::conditional_t<
            std::is_lvalue_reference_v<Source>, //
            std::remove_reference_t<T>&,        //
            std::remove_reference_t<T>&&>;

        return static_cast<type>(x);
    }
}