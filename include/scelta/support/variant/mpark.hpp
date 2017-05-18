// Copyright (c) 2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

// clang-format off
// Usage of C++17: `__has_include`.
#if __has_include(<mpark/variant.hpp>)
// clang-format on

#include "../../utils/homogenizer.hpp"
#include <mpark/variant.hpp>

#define SCELTA_SUPPORT_VARIANT_MPARK 1
SCELTA_DEFINE_HOMOGENIZER_VARIANT(::mpark::variant, ::mpark::visit)

#endif
