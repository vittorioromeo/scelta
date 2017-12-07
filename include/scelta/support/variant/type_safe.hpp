// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once
#ifndef SCELTA_SUPPORT_VARIANT_TYPE_SAFE_DISABLE

// clang-format off
#if __has_include(<type_safe/variant.hpp>) && \
    __has_include(<type_safe/visitor.hpp>)
// clang-format on

#include <cassert>
#include <type_safe/variant.hpp>
#include <type_safe/visitor.hpp>
#include "./enable/type_safe.hpp"

#endif
#endif
