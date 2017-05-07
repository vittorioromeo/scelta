// Copyright (c) 2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
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
        template <typename F>
        struct fn_ref_wrapper
        {
            static_assert(std::is_reference_v<F>);

            F _f;

            template <typename FFwd>
            constexpr fn_ref_wrapper(FFwd&& f) noexcept : _f{FWD(f)}
            {
            }

            // clang-format off
            template <typename... Ts>
            constexpr auto operator()(Ts&&... xs) const
                SCELTA_RETURNS(
                    (_f)(FWD(xs)...)
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

#define DEFINE_CALL_OPERATOR(m_ref_qualifier)                                 \
    template <typename... Ts>                                                 \
    constexpr auto operator()(Ts&&... xs) m_ref_qualifier                     \
    SCELTA_NOEXCEPT_AND_TRT(std::declval<F m_ref_qualifier>()(                \
        std::declval<fn_ref_wrapper<y_combinator_wrapper m_ref_qualifier>>(), \
        FWD(xs)...))                                                          \
    {                                                                         \
        return static_cast<F m_ref_qualifier>(*this)(                         \
            fn_ref_wrapper<y_combinator_wrapper m_ref_qualifier>{             \
                static_cast<y_combinator_wrapper m_ref_qualifier>(*this)},    \
            FWD(xs)...);                                                      \
    }

            DEFINE_CALL_OPERATOR(&)
            DEFINE_CALL_OPERATOR(const&)
            DEFINE_CALL_OPERATOR(&&)
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