// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "../meta/forward_like.hpp"
#include "../meta/replace_all.hpp"
#include "../meta/y_combinator.hpp"
#include "../utils/fwd.hpp"
#include "../utils/returns.hpp"
#include "../visitation/visit.hpp"
#include "./visit.hpp"
#include <boost/callable_traits.hpp>
#include <experimental/type_traits>
#include <type_traits>

namespace scelta::experimental::recursive
{
    namespace impl
    {
        template <typename F>
        inline constexpr auto arity =
            std::tuple_size_v<boost::callable_traits::args_t<F>>;

        template <typename T>
        using is_not_overloaded_impl = decltype(&std::decay_t<T>::operator());

        template <typename T>
        using is_not_overloaded =
            std::experimental::is_detected<is_not_overloaded_impl, T>;

        template <bool Overloaded>
        struct adapt_helper
        {
            template <typename F>
            constexpr auto operator()(F&& f) noexcept
            {
                return [f = FWD(f)](auto, auto&& x)
                    SCELTA_RETURNS(std::forward<F>(f)(FWD(x)));
            }
        };

        template <typename>
        struct adapt_helper2;

        template <template <typename...> class T, typename... Ts>
        struct adapt_helper2<T<Ts...>>
        {
            template <typename F>
            constexpr auto do_it(F&& f)
            {
                return [f = FWD(f)](auto, Ts... xs)
                    SCELTA_RETURNS(std::forward<F>(f)(FWD(xs)...));
            }
        };

        template <>
        struct adapt_helper<false>
        {
            template <typename F>
            constexpr auto operator()(F&& f)
            {
                using arg_tuple =
                    boost::callable_traits::args_t<std::decay_t<F>>;

                return adapt_helper2<arg_tuple>{}.do_it(FWD(f));
            }
        };


        template <typename F>
        auto adapt(F&& f)
        {
            return adapt_helper<(!is_not_overloaded<std::decay_t<F>>::value)>{}(
                FWD(f));
        }


        template <typename Return, typename... Fs>
        constexpr auto make_recursive_visitor_2(Fs&&... fs)
        {
            // clang-format off
            return meta::y_combinator(
                // Recurse on this lambda via Y combinator to pass it back
                // as part of the bound `recurse` argument.
                [o = overload(FWD(fs)...)](auto self, auto&&... xs) mutable
                    -> Return
                {
                    // Invoke the overload...
                    return o(

                        // Passing a visitor that invokes `self` to the
                        // `recurse` argument.
                        [&self](auto&&... vs) -> Return {
                            return ::scelta::recursive::visit<Return>(
                                self, FWD(vs)...);
                        },

                        // Forwarding the rest of the arguments.
                        FWD(xs)...);
                }
            );
            // clang-format on
        }

        template <typename Return, typename... BaseCases>
        constexpr auto make_recursive_visitor(BaseCases&&... baseCases)
        {
            return [&baseCases...](auto&&... recursiveCases) {
                return make_recursive_visitor_2<Return>(
                    adapt(std::forward<BaseCases>(baseCases))...,
                    FWD(recursiveCases)...);
            };
        }
    }

    template <typename Return, typename... Fs>
    constexpr auto match(Fs&&... fs)
    {
        // TODO: broken
        return [rv = ::scelta::experimental::recursive::impl::
                    make_recursive_visitor<Return>(FWD(fs)...)](
            auto&&... vs) mutable->Return
        {
            return ::scelta::recursive::visit<Return>(rv, FWD(vs)...);
        };
    }
}

// TODO: complete, use CallableTraits?D
