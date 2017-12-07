// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

// This header assumes that `tl::optional` was already properly included
// beforehand.

#include "../../../traits/adt/visit.hpp"
#include "../../../utils/optional_utils.hpp"

#define SCELTA_SUPPORT_OPTIONAL_TL 1

namespace scelta::traits::adt
{
    template <typename T>
    struct visit<::tl::optional<T>> : impl::visit_optional_t
    {
    };
}
