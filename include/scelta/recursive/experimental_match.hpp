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
#include "../visitation/visit.hpp"
#include "./visit.hpp"
#include "./match.hpp"
#include "./original_type.hpp"
#include "../traits/adt/is_visitable.hpp"
#include "../traits/adt/alternatives.hpp"
#include <type_traits>

namespace scelta::experimental::recursive
{
    namespace impl
    {
        struct deduce_rt{};

        /*
        template <typename F>
        struct bco_adapter : F
        {
            template <typename FFwd>
            constexpr bco_adapter(FFwd&& f) noexcept(noexcept(F(FWD(f)))) : F(FWD(f)) { }

            template <typename T, typename... Ts>
            constexpr auto operator()(T&&, Ts&&... xs)
            {
            }
        };
        */

        template <typename T>
        using original_decay_t =
            ::scelta::recursive::impl::original_type_t<std::decay_t<T>>;

        template <typename T>
        using original_decay_first_alternative_t =
            ::scelta::traits::adt::first_alternative<original_decay_t<T>>;

        template <typename T>
        struct tw { using type = T ; };

        template <typename... Fs>
        constexpr auto make_recursive_visitor_2(Fs&&... fs)
        {
            // clang-format off
            return [o = overload(FWD(fs)...)](auto t) mutable
            {
                using Return = typename decltype(t)::type;

                return meta::y_combinator(
                    // Recurse on this lambda via Y combinator to pass it back
                    // as part of the bound `recurse` argument.
                    [o = std::move(o)](auto self, auto&&... xs) mutable
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
            };
            // clang-format on
        }

        template <typename Return, typename BCO>
        struct with_bound_base_cases : private BCO // "base case overload", EBO
        {
            template <typename BCOFwd>
            constexpr with_bound_base_cases(BCOFwd&& bco) noexcept(
                noexcept(BCO(FWD(bco))))
                : BCO(FWD(bco))
            {
            }

            template <typename... RecursiveCases>
            constexpr auto operator()(RecursiveCases&&... rcs)
            {
                constexpr bool has_recursive_cases = (::scelta::traits::adt::is_visitable_v<RecursiveCases&&> && ...);

                if constexpr(!has_recursive_cases)
                {
                    // base case overload with one extra argument (+1 arity)
                    auto adapted_bco = [bco = static_cast<BCO&&>(*this)]
                        (auto, auto&&... xs) mutable SCELTA_NOEXCEPT_AND_TRT(std::declval<BCO&&>()(FWD(xs)...)) {
                            return bco(FWD(xs)...);
                        };

                    return [rv = make_recursive_visitor_2( // unresolved recursive visitor
                            std::move(adapted_bco),
                            FWD(rcs)...
                        )](auto&&... vs) mutable
                    {

                        using rt = std::conditional_t<
                            std::is_same_v<Return, deduce_rt>,

                            // result of calling BCO& (unadapted overload) with the first alternative of each variant
                            std::result_of_t<BCO&(original_decay_first_alternative_t<decltype(vs)>...)>,
                            //decltype(std::declval<BCO&>()(std::declval<original_decay_first_alternative_t<decltype(vs)>>()...)),

                            // user-specified result
                            Return
                        >;

                        auto resolved_vis = rv(tw<rt>{});
                        return ::scelta::recursive::visit<rt>(resolved_vis, FWD(vs)...);
                    };
                }
                else
                {
                    return ::scelta::visit(static_cast<BCO&>(*this), FWD(rcs)...);
                }
            }
        };

        // clang-format off
        template <typename Return, typename... Fs>
        constexpr auto make_bound_base_cases(Fs&&... fs)
            SCELTA_RETURNS(
                with_bound_base_cases<
                    Return,
                    std::decay_t<decltype(::scelta::overload(FWD(fs)...))>
                >{::scelta::overload(FWD(fs)...)}
            )
        // clang-format on
    }

    // First `match` invocation takes one or more base cases.
    // Second invocation takes zero or more recursive cases.
    // Return type can be optionally specified explicitly (deduced by default).
    // clang-format off
    template <typename Return = impl::deduce_rt, typename... BaseCases>
    constexpr auto match(BaseCases&&... bcs)
        SCELTA_RETURNS(
            impl::make_bound_base_cases<Return>(FWD(bcs)...)
        )
    // clang-format on
}

// TODO: complete
