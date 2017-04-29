// Copyright (c) 2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "../meta/forward_like.hpp"
#include "../meta/replace_all.hpp"
#include "../meta/y_combinator.hpp"
#include "../utils/fwd.hpp"
#include "../utils/returns.hpp"
#include "../visitation/visit.hpp"
#include <type_traits>

namespace scelta::recursive
{
    namespace impl
    {
        template <typename Visitor>
        struct recursor
        {
            Visitor& _visitor;

            constexpr recursor(Visitor& visitor) noexcept : _visitor{visitor}
            {
            }

            // clang-format off
            template <typename... Ts>
            constexpr auto operator()(Ts&&... xs) 
                SCELTA_RETURNS(
                    ::scelta::visit(_visitor, FWD(xs)...)
                )
            // clang-format on
        };

        template <typename F>
        struct recursive_visitor : F
        {
            using this_type = recursive_visitor<F>;
            using recursor_type = recursor<this_type>;

            template <typename FFwd>
            constexpr recursive_visitor(FFwd&& f) noexcept(noexcept(F{FWD(f)}))
                : F{FWD(f)}
            {
            }

            // TODO: type deduction fails here for boost::variant
            template <typename... Ts>
            constexpr auto operator()(Ts&&... xs) SCELTA_NOEXCEPT_AND_TRT(
                std::declval<F&>()(std::declval<recursor_type&&>(), FWD(xs)...))
            {
                return static_cast<F&>(*this)(recursor_type{*this}, FWD(xs)...);
            }

#define PRODUCE_ERROR_TODO(str)                \
    do                                         \
    {                                          \
        [](auto x) constexpr                   \
        {                                      \
            static_assert(decltype(x){}, str); \
        }                                      \
        (std::false_type{});                   \
    } while(false)


            constexpr auto operator()(...)
            {
                PRODUCE_ERROR_TODO(
                    "Could not deduce recursive visitation return type.");
            }
        };
    }

    // TODO: ?
    // clang-format off
    template <typename Visitor>
    constexpr auto visit(Visitor&& visitor) 
        SCELTA_RETURNS(FWD(visitor)())

    template <typename Visitor, typename Variant, typename... Variants>
    constexpr auto visit(
        Visitor&& visitor, Variant&& variant, Variants&&... variants)
        SCELTA_RETURNS(
            scelta::impl::visit_homogenizer(
                FWD(visitor), FWD(variant), FWD(variants)...)
        )

    template <typename... Fs>
    constexpr auto match(Fs&&... fs)
    {
        //return meta::y_combinator(
        // clang-format on


        auto o = overload(FWD(fs)...);
        auto rv = impl::recursive_visitor<decltype(o)>{std::move(o)};

        return [rv = std::move(rv)](auto&&... vs) mutable->decltype(auto)
        {
            return scelta::visit(rv, FWD(vs)...);
        };
    }
}