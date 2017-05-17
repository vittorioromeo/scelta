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
#include "./visit.hpp"
#include <experimental/type_traits>
#include <type_traits>

namespace scelta::experimental::recursive
{
    namespace impl
    {

        template <typename T>
        struct function_traits
            : public function_traits<decltype(&T::operator())>
        {
        };

        template <typename T, typename TReturn, typename... TArgs>
        struct function_traits<TReturn (T::*)(TArgs...) const>
        {
            static constexpr std::size_t arity = sizeof...(TArgs);

            using result_type = TReturn;

            template <std::size_t TI>
            using arg = std::tuple_element_t<TI, std::tuple<TArgs...>>;
        };

        template <typename T>
        using is_not_overloaded_impl = decltype(&std::decay_t<T>::operator());

        template <typename T>
        using is_not_overloaded =
            std::experimental::is_detected<is_not_overloaded_impl, T>;


        template <bool TOverloaded>
        struct adapt_helper;

        template <>
        struct adapt_helper<true>
        {
            template <typename TF>
            auto operator()(TF&& f)
            {
                return [f = FWD(f)](auto, auto&& x)->decltype(f(FWD(x)))
                {
                    return f(FWD(x));
                };
            }
        };

        template <>
        struct adapt_helper<false>
        {
            template <typename TF>
            auto operator()(TF&& f)
            {
                using argt = typename function_traits<
                    std::decay_t<decltype(f)>>::template arg<0>;

                return [f = FWD(f)](auto, argt x)->decltype(f(x))
                {
                    return f(x);
                };
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
                return make_recursive_visitor_2(
                    adapt(std::forward<BaseCases>(baseCases))...,
                    FWD(recursiveCases)...);
            };
        }
    }

    template <typename Return, typename... Fs>
    constexpr auto match(Fs&&... fs)
    {
        return [rv = ::scelta::experimental::recursive::impl::
                    make_recursive_visitor<Return>(FWD(fs)...)](
            auto&&... vs) mutable->Return
        {
            return ::scelta::recursive::visit<Return>(rv, FWD(vs)...);
        };
    }
}

// TODO: complete, use CallableTraits?D
