// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <utility>

#ifndef FWD
#define FWD(...) ::std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)
#endif
