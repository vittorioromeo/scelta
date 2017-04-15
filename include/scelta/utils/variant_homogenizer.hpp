// Copyright (c) 2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./returns.hpp"

// Usage of C++17: nested `namespace`.
namespace scelta::impl
{
    // Usage of C++17: `template <...> typename`.
    template <template <typename...> typename>
    struct variant_homogenizer;

    template <typename>
    struct variant_homogenizer_helper;

    template <template <typename...> class Variant, typename... Ts>
    struct variant_homogenizer_helper<Variant<Ts...>>
    {
        using type = variant_homogenizer<Variant>;
    };
}

#define SCELTA_DEFINE_VARIANT_HOMOGENIZER(m_type, m_function) \
    namespace scelta::impl                                    \
    {                                                         \
        template <>                                           \
        struct variant_homogenizer<m_type>                    \
        {                                                     \
            static constexpr const char* name{#m_type};       \
                                                              \
            template <typename... Ts>                         \
            constexpr auto operator()(Ts&&... xs) const       \
                SCELTA_RETURNS(m_function(FWD(xs)...))        \
        };                                                    \
    }
