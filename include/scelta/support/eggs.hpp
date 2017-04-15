// Copyright (c) 2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

// clang-format off
// Usage of C++17: `__has_include`.
#if __has_include(<eggs/variant.hpp>)
// clang-format on

#include "../utils/variant_homogenizer.hpp"
#include <eggs/variant.hpp>

#define SCELTA_SUPPORT_EGGS_VARIANT 1

SCELTA_DEFINE_VARIANT_HOMOGENIZER(::eggs::variant, ::eggs::variants::apply)

// Usage of C++17: nested `namespace`.
namespace scelta::support
{
    constexpr bool eggs{true};
}

#else

// Usage of C++17: nested `namespace`.
namespace scelta::support
{
    constexpr bool eggs{false};
}

#endif