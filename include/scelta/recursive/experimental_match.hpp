// Copyright (c) 2017 Vittorio Romeo
// MIT License |  https://opensource.org/licenses/MIT
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "../meta/forward_like.hpp"
#include "../meta/replace_all.hpp"
#include "../meta/y_combinator.hpp"
#include "../traits/adt/alternatives.hpp"
#include "../traits/adt/is_visitable.hpp"
#include "../utils/fwd.hpp"
#include "../utils/overload.hpp"
#include "../utils/returns.hpp"
#include "../visitation/visit.hpp"
#include "./match.hpp"
#include "./original_type.hpp"
#include "./visit.hpp"
#include <type_traits>

namespace scelta::experimental::recursive
{
    namespace impl
    {
        // Tag type that when passed to `match` attempts to deduce the return
        // type by invoking the base case overload set with the first
        // alternative of the visitable.
        struct deduce_rt
        {
        };

        // SFINAE-friendly return type dispatch.
        // Not-deduced case: evaluate to `Return`.
        template <typename Return>
        struct return_type
        {
            template <typename...>
            using apply = Return;
        };

        // SFINAE-friendly return type dispatch.
        // Deduced case: evaluate to result of `F(Ts...)`.
        template <>
        struct return_type<deduce_rt>
        {
            template <typename F, typename... Ts>
            using apply = std::result_of_t<F(Ts...)>;
        };

        // SFINAE-friendly return type dispatch.
        // Alias for `return_type<Return>::apply<F, Ts...>`.
        template <typename Return, typename F, typename... Ts>
        using applied_return_type =
            typename return_type<Return>::template apply<F, Ts...>;

        template <typename T>
        using original_decay_t =
            ::scelta::recursive::impl::original_type_t<std::decay_t<T>>;

        template <typename T>
        using original_decay_first_alternative_t =
            ::scelta::traits::adt::first_alternative<original_decay_t<T>>;

        template <typename O>
        struct unresolved_visitor : O
        {
            template <typename OFwd>
            constexpr unresolved_visitor(OFwd&& o) noexcept(noexcept(O{FWD(o)}))
                : O{FWD(o)}
            {
            }

            template <typename Return>
            constexpr auto resolve() &
            {
                // clang-format off
                return meta::y_combinator(
                    // Recurse on this lambda via Y combinator to pass it back
                    // as part of the bound `recurse` argument.
                    [this](auto self, auto&&... xs) mutable->Return {
                        // Invoke the overload...
                        return static_cast<O&>(*this)(
                            // Passing a visitor that invokes `self` to the
                            // `recurse` argument.
                            [&self](auto&&... vs) -> Return {
                                return ::scelta::recursive::visit<Return>(
                                    self, FWD(vs)...);
                            },

                            // Forwarding the rest of the arguments.
                            FWD(xs)...);
                    });
                // clang-format on
            }
        };

        template <typename BCO>
        constexpr auto make_unresolved_visitor(BCO&& bco)
            SCELTA_RETURNS(unresolved_visitor<std::decay_t<BCO>>{FWD(bco)})


                template <typename Return, typename BCO>
                struct with_bound_base_cases
            : private BCO // "base case overload", EBO
        {
            template <typename BCOFwd>
            constexpr with_bound_base_cases(BCOFwd&& bco) noexcept(
                noexcept(BCO(FWD(bco))))
                : BCO(FWD(bco))
            {
            }

        private:
            template <typename... Xs>
            constexpr decltype(auto) do_non_recursive(Xs&&... xs)
            {
                return ::scelta::visit(static_cast<BCO&>(*this), FWD(xs)...);
            }

            template <typename... Xs>
            constexpr auto defer_recursive(Xs&&... xs)
            {
                // "Base case overload" with one extra argument (+1 arity).
                auto adapted_bco = [bco = static_cast<BCO&&>(*this)] //
                    (auto, auto&&... ys) mutable                     //
                    SCELTA_NOEXCEPT_AND_TRT(std::declval<BCO&&>()(FWD(ys)...))
                {
                    return bco(FWD(ys)...);
                };

                // Overload of "adapted BCO" and "recursive cases".
                auto o = overload(std::move(adapted_bco), FWD(xs)...);

                return [urv = make_unresolved_visitor(std::move(o))](
                    auto&&... vs) mutable->decltype(auto)
                {
                    using rt = applied_return_type<Return, BCO&,
                        original_decay_first_alternative_t<decltype(vs)>...>;

                    auto resolved_vis = urv.template resolve<rt>();
                    return ::scelta::recursive::visit<rt>(
                        resolved_vis, FWD(vs)...);
                };
            }

        public:
            template <typename... Xs>
            constexpr decltype(auto) operator()(Xs&&... xs)
            {
                constexpr bool has_recursive_cases =
                    !(::scelta::traits::adt::is_visitable_v<Xs&&> && ...);

                if constexpr(has_recursive_cases)
                {
                    return defer_recursive(FWD(xs)...);
                }
                else
                {
                    return do_non_recursive(FWD(xs)...);
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
