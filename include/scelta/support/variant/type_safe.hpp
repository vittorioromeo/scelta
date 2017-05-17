// Copyright (c) 2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

// Usage of C++17: `__has_include`.
#if __has_include(<type_safe/variant.hpp>) && __has_include(<type_safe/visitor.hpp>)

#include "../../utils/homogenizer.hpp"
#include "../../utils/overload.hpp"
#include "../../utils/linear_overload.hpp"
#include <type_safe/variant.hpp>
#include <type_safe/visitor.hpp>
#include <cassert>

#define SCELTA_SUPPORT_VARIANT_TYPE_SAFE 1

namespace scelta::impl
{
    struct ts_variant_homogenizer
    {
        template <typename... Ts>
        constexpr decltype(auto) operator()(Ts&&... xs) const
            {return ::type_safe::visit(FWD(xs)...);}
    };

    template <typename VariantPolicy, typename... Ts>
    struct homogenizer_helper<::type_safe::basic_variant<VariantPolicy, Ts...>>
    {
        using type = ts_variant_homogenizer;
    };

    template <typename T>
    struct non_recursive_adapter
    {
        T&& _visitor;

        template <typename TFwd>
        non_recursive_adapter(TFwd&& visitor) : _visitor{FWD(visitor)}{}

        template <typename... Ts, typename = std::enable_if_t<
            (std::is_same_v<std::decay_t<Ts>, type_safe::nullvar_t> || ...)>
        >
        constexpr auto operator()(Ts&&...){ assert(false); }

        template <typename... Ts, typename = std::enable_if_t<
            !(std::is_same_v<std::decay_t<Ts>, type_safe::nullvar_t> || ...)>,
            typename Dummy = int
        >
        constexpr auto operator()(Ts&&... xs)
            SCELTA_RETURNS(
                _visitor(FWD(xs)...)
            )
    };


    template <typename R, typename T>
    struct recursive_adapter
    {
        T&& _visitor;

        template <typename TFwd>
        recursive_adapter(TFwd&& visitor) : _visitor{FWD(visitor)}{}

        template <typename... Ts, typename = std::enable_if_t<
            (std::is_same_v<std::decay_t<Ts>, type_safe::nullvar_t> || ...)>
        >
        constexpr auto operator()(Ts&&...){ assert(false); }

        template <typename... Ts, typename = std::enable_if_t<
            !(std::is_same_v<std::decay_t<Ts>, type_safe::nullvar_t> || ...)>,
            typename Dummy = int
        >
        constexpr auto operator()(Ts&&... xs)
            SCELTA_RETURNS(
                _visitor(FWD(xs)...)
            )
    };


    template <typename VariantPolicy, typename T, typename... Ts>
    struct visitor_adapter<::type_safe::basic_variant<VariantPolicy, T, Ts...>>
    {
        template <typename U>
        constexpr auto operator()   (::scelta::impl::non_recursive_tag, U&& x) const
        {
            return non_recursive_adapter<U&&>{FWD(x)};
        }

        template <typename R, typename U>
        constexpr auto operator()(::scelta::impl::recursive_tag<R>, U&& x) const
        {
            return recursive_adapter<R, U&&>{FWD(x)};
        }
    };
}

#endif
