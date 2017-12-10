// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./optional_monadic.hpp"

namespace scelta::infix
{
    namespace detail
    {
        template <typename F>
        struct infix_f : F
        {
            constexpr infix_f(F&& f) : F{FWD(f)}
            {
            }
        };
    }

    template <typename Optional, typename F>
    constexpr auto operator|(Optional&& o, detail::infix_f<F>&& f)
    {
        return f(FWD(o));
    }

#define SCELTA_DEFINE_INFIX(name)                                              \
    template <typename... Ts>                                                  \
    constexpr auto name(Ts&&... xs)                                            \
    {                                                                          \
                                                                               \
        return detail::infix_f{                                                \
            [&](auto&& o) SCELTA_RETURNS(::scelta::name(FWD(o), FWD(xs)...))}; \
    }

    SCELTA_DEFINE_INFIX(map_or_else)
    SCELTA_DEFINE_INFIX(map_or)
    SCELTA_DEFINE_INFIX(map)
    SCELTA_DEFINE_INFIX(and_then)
    SCELTA_DEFINE_INFIX(and_)
    SCELTA_DEFINE_INFIX(or_else)
    SCELTA_DEFINE_INFIX(or_)

#undef SCELTA_DEFINE_INFIX
}
