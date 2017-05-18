// Copyright (c) 2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

// clang-format off
// Usage of C++17: `__has_include`.
#if __has_include(<type_safe/variant.hpp>) && \
    __has_include(<type_safe/visitor.hpp>)
// clang-format on

#include "../../utils/homogenizer.hpp"
#include "../../traits/variant.hpp"
#include "../../utils/linear_overload.hpp"
#include "../../utils/overload.hpp"
#include <cassert>
#include <type_safe/variant.hpp>
#include <type_safe/visitor.hpp>

#define SCELTA_SUPPORT_VARIANT_TYPE_SAFE 1

namespace scelta::impl
{
    template <typename... Ts>
    using any_is_nullvar = std::disjunction<
        std::is_same<std::decay_t<Ts>, type_safe::nullvar_t>...>;

    template <typename... Ts>
    inline constexpr auto any_is_nullvar_v = any_is_nullvar<Ts...>::value;

    template <typename T>
    struct nullvar_ignorer
    {
        T _visitor;
        static_assert(std::is_reference_v<T>);

        template <typename TFwd>
        constexpr nullvar_ignorer(TFwd&& visitor) : _visitor{FWD(visitor)}
        {
        }

        template <typename... Ts,
            std::enable_if_t<any_is_nullvar_v<Ts...>>* = nullptr>
        constexpr void operator()(Ts&&...) const noexcept
        {
            assert(false);
            __builtin_unreachable();
        }

        template <typename... Ts,
            std::enable_if_t<!any_is_nullvar_v<Ts...>>* = nullptr>
        constexpr auto operator()(Ts&&... xs)
            SCELTA_RETURNS(FWD(_visitor)(FWD(xs)...))
    };

    template <typename T>
    constexpr auto make_nullvar_ignorer(T&& x)
        SCELTA_RETURNS(
            nullvar_ignorer<T&&>{FWD(x)}
        )
}

namespace scelta::impl
{
    template <typename VariantPolicy, typename... Alternatives>
    struct traits<::type_safe::basic_variant<VariantPolicy, Alternatives...>>
    {
        template <typename Tag, typename Visitor, typename... Ts>
        static constexpr auto visit(Tag, Visitor&& v, Ts&&... xs)
            SCELTA_RETURNS(
                ::type_safe::visit(
                    ::scelta::impl::make_nullvar_ignorer(FWD(v)),
                    FWD(xs)...
                )
            )

        template <typename... Variants>
        static constexpr auto valid_state(Variants&&...)
            SCELTA_RETURNS(
                // TODO: depends on the policy
                true
            )
    };
}

#endif
