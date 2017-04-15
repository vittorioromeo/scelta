// Copyright (c) 2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#define SCELTA_RETURNS(...)                                \
    noexcept(noexcept(__VA_ARGS__))->decltype(__VA_ARGS__) \
    {                                                      \
        return __VA_ARGS__;                                \
    }
