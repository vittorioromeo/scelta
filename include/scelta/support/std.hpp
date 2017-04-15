// Copyright (c) 2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

// Usage of C++17: `__has_include`.
#if __has_include(<variant>)

// Usage of C++17: `<variant>`.
#include "../utils/variant_homogenizer.hpp"
#include <variant>

#define SCELTA_SUPPORT_STD_VARIANT 1

SCELTA_DEFINE_VARIANT_HOMOGENIZER(::std::variant, ::std::visit)

// Usage of C++17: nested `namespace`.
namespace scelta::support
{
    constexpr bool std{true};
}

#else

// Usage of C++17: nested `namespace`.
namespace scelta::support
{
    constexpr bool std{false};
}

#endif
