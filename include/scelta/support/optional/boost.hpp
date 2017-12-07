// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once
#ifndef SCELTA_SUPPORT_OPTIONAL_BOOST_DISABLE

// clang-format off
#if __has_include(<boost/optional.hpp>) && __has_include(<boost/none.hpp>)
// clang-format on

#include <boost/optional.hpp>
#include <boost/none.hpp>
#include "./enable/boost.hpp"

#endif
#endif
