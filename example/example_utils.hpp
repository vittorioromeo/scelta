// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <scelta.hpp>
#include <scelta/recursive/experimental_match.hpp>

namespace example
{
    // First available variant type.
    template <typename... Ts>
    using variant =
#if defined(SCELTA_SUPPORT_VARIANT_STD)
        ::std::variant
#elif defined(SCELTA_SUPPORT_VARIANT_BOOST)
        ::boost::variant
#elif defined(SCELTA_SUPPORT_VARIANT_EGGS)
        ::eggs::variant
#elif defined(SCELTA_SUPPORT_VARIANT_MPARK)
        ::mpark::variant
#elif defined(SCELTA_SUPPORT_VARIANT_TYPE_SAFE)
        ::type_safe::variant
#else
#error "No variant type available."
#endif
        <Ts...>;


    // First available optional type.
    template <typename... Ts>
    using optional =
#if defined(SCELTA_SUPPORT_OPTIONAL_STD)
        ::std::optional
#elif defined(SCELTA_SUPPORT_OPTIONAL_BOOST)
        ::boost::optional
#elif defined(SCELTA_SUPPORT_OPTIONAL_TYPE_SAFE)
        ::type_safe::optional
#else
#error "No optional type available."
#endif
        <Ts...>;
}
