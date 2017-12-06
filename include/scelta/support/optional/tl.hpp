// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once
#ifndef SCELTA_SUPPORT_OPTIONAL_TL_DISABLE

// clang-format off
#if __has_include(<tl/optional.hpp>)
// clang-format on

#include "../../utils/optional_utils.hpp"
#include "../../traits/adt/visit.hpp"
#include <tl/optional.hpp>

#define SCELTA_SUPPORT_OPTIONAL_TL 1

namespace scelta::traits::adt
{
    template <typename T>
    struct visit<::tl::optional<T>> : impl::visit_optional_t
    {
    };
}

#endif
#endif
