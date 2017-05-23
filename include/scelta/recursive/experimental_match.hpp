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
#include "../traits.hpp"
#include <experimental/type_traits>
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
        using is_visitable_impl = decltype(::scelta::traits::adt::visit<std::decay_t<T>>{});

        template <typename T>
        inline constexpr auto is_visitable_v =
            std::experimental::is_detected_v<is_visitable_impl, T>;

template <typename T>
using original_decay_first_alternative = ::scelta::traits::adt::first_alternative<::scelta::recursive::impl::original_type_t<std::decay_t<T>>>;

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
                    [&o](auto self, auto&&... xs) mutable // TODO: potential dangling ref?
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
        struct with_bound_base_cases : private BCO // "base case overload"
        {
            template <typename BCOFwd>
            constexpr with_bound_base_cases(BCOFwd&& bco) noexcept(noexcept(BCO(FWD(bco)))) : BCO(FWD(bco)) { }

            template <typename... RecursiveCases>
            constexpr auto operator()(RecursiveCases&&... rcs)
            {
                if constexpr((!is_visitable_v<RecursiveCases&&> && ...)) // if there are no recursive cases
                {
                    // binary base case overload
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

                            // result of calling BCO& with the first alternative of each variant
                            decltype(std::declval<BCO&>()(std::declval<original_decay_first_alternative<decltype(vs)>>()...)),

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
    }

    // First `match` invocation takes a bunch of base cases.
    // TODO: deduce `Return` from base cases?
    // TODO: check if there are no recursive cases and prevent useless `()` syntax?
    // TODO: SCELTA_RETURNS
    template <typename Return = impl::deduce_rt, typename... BaseCases>
    constexpr auto match(BaseCases&&... bcs)
        SCELTA_RETURNS(
            impl::with_bound_base_cases<
                Return,
                std::decay_t<decltype(::scelta::overload(FWD(bcs)...))>
            >{::scelta::overload(FWD(bcs)...)}
        )
}

// TODO: complete
