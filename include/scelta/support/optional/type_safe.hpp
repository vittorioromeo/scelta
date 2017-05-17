// Copyright (c) 2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

// Usage of C++17: `__has_include`.
#if __has_include(<type_safe/optional.hpp>)

#include "../../utils/optional_utils.hpp"
#include "../../utils/homogenizer.hpp"
#include <type_safe/optional.hpp>

#define SCELTA_SUPPORT_OPTIONAL_TYPE_SAFE 1

namespace scelta::impl
{
    template <typename StoragePolicy>
    struct ts_optional_homogenizer
    {
        template <typename... Ts>
        constexpr auto operator()(Ts&&... xs) const
            SCELTA_RETURNS(::scelta::impl::visit_optional(FWD(xs)...))
    };

    template <typename StoragePolicy>
    struct homogenizer_helper<::type_safe::basic_optional<StoragePolicy>>
    {
        using type = ts_optional_homogenizer<StoragePolicy>;
    };

    template <typename StoragePolicy>
    struct optional_access_homogenizer<
        ::type_safe::basic_optional<StoragePolicy>>
    {
        template <typename U>
        constexpr auto operator()(U&& x) SCELTA_RETURNS(FWD(x).value())
    };
}

#endif
