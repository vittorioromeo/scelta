// Copyright (c) 2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <utility>

#ifndef FWD
#define FWD(...) ::std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)
#endif
