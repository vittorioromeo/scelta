// Copyright (c) 2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <type_traits>

#define SCELTA_NOEXCEPT_AND_TRT_ENABLE_IF(m_condition, ...) \
    noexcept(noexcept(__VA_ARGS__))                         \
        ->::std::enable_if_t<m_condition, decltype(__VA_ARGS__)>

#define SCELTA_NOEXCEPT_AND_TRT(...) \
    noexcept(noexcept(__VA_ARGS__))->decltype(__VA_ARGS__)

#define SCELTA_RETURNS_ENABLE_IF(m_condition, ...) \
    SCELTA_NOEXCEPT_AND_TRT_ENABLE_IF(__VA_ARGS__) \
    {                                              \
        return __VA_ARGS__;                        \
    }

#define SCELTA_RETURNS(...)              \
    SCELTA_NOEXCEPT_AND_TRT(__VA_ARGS__) \
    {                                    \
        return __VA_ARGS__;              \
    }

#define SCELTA_NOEXCEPT_AND_RETURN_BODY(...) \
    noexcept(noexcept(__VA_ARGS__))          \
    {                                        \
        return __VA_ARGS__;                  \
    }
