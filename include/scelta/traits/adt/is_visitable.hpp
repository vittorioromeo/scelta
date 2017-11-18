// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./visit.hpp"
#include <experimental/type_traits>

namespace scelta::traits::adt
{
    namespace impl
    {
        template <typename T>
        using is_visitable_impl =
            decltype(::scelta::traits::adt::visit<std::decay_t<T>>{});
    }

    // Something is visitable when there's a valid `adt::visit` trait for it.
    template <typename T>
    inline constexpr auto is_visitable_v =
        std::experimental::is_detected_v<impl::is_visitable_impl, T>;
}
