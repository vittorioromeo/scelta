// Copyright (c) 2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <scelta.hpp>

namespace example
{
    // First available variant type.
    template <typename... Ts>
    using variant =
#if defined(SCELTA_SUPPORT_VARIANT_STD)
        std::variant
#elif defined(SCELTA_SUPPORT_VARIANT_BOOST)
        boost::variant
#elif defined(SCELTA_SUPPORT_VARIANT_EGGS)
        eggs::variant
#else
#error "No variant type available."
#endif
        <Ts...>;


    // First available optional type.
    template <typename... Ts>
    using optional =
#if defined(SCELTA_SUPPORT_OPTIONAL_STD)
        std::optional
#elif defined(SCELTA_SUPPORT_OPTIONAL_BOOST)
        boost::optional
#else
#error "No optional type available."
#endif
        <Ts...>;
}