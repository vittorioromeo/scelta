// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "../meta/forward_like.hpp"
#include "../meta/replace_all.hpp"
#include "../meta/y_combinator.hpp"
#include "../utils/fwd.hpp"
#include "../utils/returns.hpp"
#include "../utils/overload.hpp"
#include "../nonrecursive/visit.hpp"
#include "./visit.hpp"
#include "./match.hpp"
#include "../traits.hpp"
#include <boost/callable_traits.hpp>
#include <experimental/type_traits>
#include <type_traits>

namespace scelta
{
    namespace impl
    {
        template <typename F>
        inline constexpr auto arity =
            std::tuple_size_v<boost::callable_traits::args_t<F>>;

        template <typename T>
        using is_not_overloaded_impl = decltype(&std::decay_t<T>::operator());

        template <typename T>
        inline constexpr auto is_overloaded_v =
            !std::experimental::is_detected_v<is_not_overloaded_impl, T>;

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
            return adapt_helper<is_overloaded_v<std::decay_t<F>>>{}(
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

    namespace impl
    {
        /*template <typename F>
        struct bco_adapter : F
        {
            template <typename FFwd>
            constexpr bco_adapter(FFwd&& f) noexcept(noexcept(F(FWD(f)))) : F(FWD(f)) { }

            template <typename T, typename... Ts>
            constexpr auto operator()(T&&, Ts&&... xs)
            {

            }
        };*/
        template <typename T>
        using is_visitable_impl = decltype(::scelta::traits::adt::visit<std::decay_t<T>>{});

        template <typename T>
        inline constexpr auto is_visitable_v =
            std::experimental::is_detected_v<is_visitable_impl, T>;


        template <typename Return, typename BaseCaseOverload>
        struct with_bound_base_cases : private BaseCaseOverload
        {
            template <typename BaseCaseOverloadFwd>
            constexpr with_bound_base_cases(BaseCaseOverloadFwd&& bco) noexcept(noexcept(BaseCaseOverload(FWD(bco)))) : BaseCaseOverload(FWD(bco)) { }

            template <typename... RecursiveCases>
            constexpr auto operator()(RecursiveCases&&... rcs)
            {
                if constexpr((!is_visitable_v<RecursiveCases&&> && ...))
                {
                    auto adapted_bco = [bco = static_cast<BaseCaseOverload&&>(*this)]
                        (auto, auto&&... xs) mutable SCELTA_NOEXCEPT_AND_TRT(std::declval<BaseCaseOverload&&>()(FWD(xs)...)) {
                            return bco(FWD(xs)...);
                        };

                    return [rv = ::scelta::recursive::impl::make_recursive_visitor<Return>(
                            std::move(adapted_bco),
                            FWD(rcs)...
                        )](auto&&... vs) mutable->Return
                    {
                        return ::scelta::recursive::visit<Return>(rv, FWD(vs)...);
                    };
                }
                else
                {
                    return ::scelta::nonrecursive::visit(static_cast<BaseCaseOverload&>(*this), FWD(rcs)...);
                }
            }
        };
    }

    // First `match` invocation takes a bunch of base cases.
    // TODO: deduce `Return` from base cases?
    // TODO: check if there are no recursive cases and prevent useless `()` syntax?
    // TODO: SCELTA_RETURNS
    template <typename Return, typename... BaseCases>
    constexpr auto match(BaseCases&&... bcs)
        SCELTA_RETURNS(
            impl::with_bound_base_cases<
                Return,
                std::decay_t<decltype(::scelta::overload(FWD(bcs)...))>
            >{::scelta::overload(FWD(bcs)...)}
        )
}

// TODO: complete, use CallableTraits?D
