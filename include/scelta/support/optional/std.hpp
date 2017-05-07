// Copyright (c) 2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

// Usage of C++17: `__has_include`.
#if __has_include(<optional>)
// clang-format on

// Usage of C++17: `<optional>`.
#include "../../utils/optional_utils.hpp"
#include <optional>

#define SCELTA_SUPPORT_OPTIONAL_STD 1
SCELTA_DEFINE_HOMOGENIZER_OPTIONAL(::std::optional)

#endif
