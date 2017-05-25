// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once
#ifndef SCELTA_SUPPORT_OPTIONAL_STD_DISABLE

// clang-format off
// Usage of C++17: `__has_include`.
#if __has_include(<optional>)
// clang-format on

// Usage of C++17: `<optional>`.
#include "../../utils/optional_utils.hpp"
#include "../../traits/adt/visit.hpp"
#include <optional>

#define SCELTA_SUPPORT_OPTIONAL_STD 1

namespace scelta::traits::adt
{
    template <typename T>
    struct visit<::std::optional<T>> : impl::visit_optional_t
    {
    };
}

#endif
#endif
