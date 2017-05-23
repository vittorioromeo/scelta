// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "../utils/fwd.hpp"
#include "../utils/returns.hpp"
#include <type_traits>

// Usage of C++17: nested `namespace`.
namespace scelta::meta
{
    namespace impl
    {
        /// @brief `constexpr`-friendly function object reference wrapper.
        template <typename F>
        struct fn_ref_wrapper
        {
            static_assert(std::is_reference_v<F>);

            F _f;

            constexpr fn_ref_wrapper(F f) noexcept : _f{FWD(f)}
            {
            }

            // clang-format off
            template <typename... Ts>
            constexpr auto operator()(Ts&&... xs) const
                SCELTA_RETURNS(
                    FWD(_f)(FWD(xs)...)
                )
            // clang-format on
        };

        template <typename F>
        struct y_combinator_wrapper : F
        {
            template <typename FFwd>
            constexpr y_combinator_wrapper(FFwd&& f) noexcept(
                noexcept(F(FWD(f))))
                : F(FWD(f))
            {
            }

#define DEFINE_CALL_OPERATOR(m_refq)                                      \
    template <typename... Ts>                                             \
    constexpr auto operator()(Ts&&... xs) m_refq SCELTA_NOEXCEPT_AND_TRT( \
        std::declval<F m_refq>()(                                         \
            std::declval<fn_ref_wrapper<y_combinator_wrapper m_refq>>(),  \
            FWD(xs)...))                                                  \
    {                                                                     \
        return static_cast<F m_refq>(*this)(                              \
            fn_ref_wrapper<y_combinator_wrapper m_refq>{                  \
                static_cast<y_combinator_wrapper m_refq>(*this)},         \
            FWD(xs)...);                                                  \
    }

            DEFINE_CALL_OPERATOR(&)
            DEFINE_CALL_OPERATOR(const&)
            DEFINE_CALL_OPERATOR(&&)

#undef DEFINE_CALL_OPERATOR
        };
    }

    // clang-format off
    template <typename F>
    auto y_combinator(F&& f)
        SCELTA_RETURNS(
            impl::y_combinator_wrapper<std::decay_t<F>>(FWD(f))
        )
    // clang-format on
}
