// Copyright (c) 2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

// Usage of C++17: `__has_include`.
#if __has_include(<boost/variant.hpp>)

#include "../../utils/homogenizer.hpp"
#include <boost/variant.hpp>

#define SCELTA_SUPPORT_VARIANT_BOOST 1
SCELTA_DEFINE_HOMOGENIZER_VARIANT(::boost::variant, ::boost::apply_visitor)

#endif
