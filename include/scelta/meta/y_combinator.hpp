// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "../utils/fwd.hpp"
#include "../utils/returns.hpp"
#include "./fn_ref_wrapper.hpp"
#include <type_traits>

namespace scelta::meta
{
    /// @brief Provides a Y combinator wrapper for a `FunctionObject`. The
    /// wrapper is `constexpr`/`noexcept`/SFINAE-friendly.
    /// @details See this proposal:
    /// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0200r0.html
    template <typename F>
    struct y_combinator : F
    {
        template <typename FFwd>
        constexpr y_combinator(FFwd&& f) noexcept(noexcept(F(FWD(f))))
            : F(FWD(f))
        {
        }

#define DEFINE_CALL_OPERATOR(m_refq)                                          \
    template <typename... Ts>                                                 \
    constexpr auto operator()(Ts&&... xs) m_refq -> decltype(     \
        std::declval<F m_refq>()(                                             \
            std::declval<fn_ref_wrapper<y_combinator m_refq>>(), FWD(xs)...)) \
    {                                                                         \
        return static_cast<F m_refq>(*this)(                                  \
            fn_ref_wrapper<y_combinator m_refq>{                              \
                static_cast<y_combinator m_refq>(*this)},                     \
            FWD(xs)...);                                                      \
    }

        DEFINE_CALL_OPERATOR(&)
        DEFINE_CALL_OPERATOR(const&)
        DEFINE_CALL_OPERATOR(&&)

#undef DEFINE_CALL_OPERATOR
    };

    template <typename FFwd>
    y_combinator(FFwd &&)->y_combinator<std::decay_t<FFwd>>;
}
