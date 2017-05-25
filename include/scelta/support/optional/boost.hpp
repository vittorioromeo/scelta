// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once
#ifndef SCELTA_SUPPORT_OPTIONAL_BOOST_DISABLE

// clang-format off
// Usage of C++17: `__has_include`.
#if __has_include(<boost/optional.hpp>) && __has_include(<boost/none.hpp>)
// clang-format on

#include "../../utils/optional_utils.hpp"
#include "../../traits/adt/visit.hpp"
#include <boost/optional.hpp>
#include <boost/none.hpp>

#define SCELTA_SUPPORT_OPTIONAL_BOOST 1

namespace scelta::traits::adt
{
    template <typename T>
    struct visit<::boost::optional<T>> : impl::visit_optional_t
    {
    };
}

#endif
#endif
