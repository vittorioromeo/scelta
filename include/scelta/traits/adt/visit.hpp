// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "../../meta/always_false.hpp"
#include "../dispatch.hpp"

namespace scelta::traits::adt
{
    template <typename>
    struct visit;

    template <typename T>
    inline constexpr traits::impl::dispatch<visit<T>> visit_v{};
}
