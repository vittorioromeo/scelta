// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

// This header assumes that `boost::optional` was already properly included
// beforehand.

#include "../../../utils/optional_utils.hpp"
#include "../../../traits/adt/visit.hpp"

#define SCELTA_SUPPORT_OPTIONAL_BOOST 1

namespace scelta::traits::adt
{
    template <typename T>
    struct visit<::boost::optional<T>> : impl::visit_optional_t
    {
    };
}
