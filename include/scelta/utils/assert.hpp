// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <cassert>
#include <utility>

#ifndef NDEBUG

/// @macro Implementation of `constexpr`-friendly assertion.
/// @details See
/// https://akrzemi1.wordpress.com/2017/05/18/asserts-in-constexpr-functions/.
#define SCELTA_CONSTEXPR_ASSERT(expr) \
    (__builtin_expect(expr, true) ? void(0) : [] { assert(!#expr); }())

#else

#define SCELTA_CONSTEXPR_ASSERT(...) void(0)

#endif
